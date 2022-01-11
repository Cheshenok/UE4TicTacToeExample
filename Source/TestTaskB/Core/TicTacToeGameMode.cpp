// Fill out your copyright notice in the Description page of Project Settings.


#include "TicTacToeGameMode.h"
#include "Containers/Array.h"
#include "TicTacToePlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "TestTaskB/Figure/CircleFigureActor.h"
#include "TestTaskB/Figure/CrossFigureActor.h"


void ATicTacToeGameMode::ResetGameState()
{
	if (IsValid((GameData)))
	{
		GameData->Reset();
	}
	
}

EFigure ATicTacToeGameMode::GetFigureOnPosition(UFieldData *GameFieldData, FVector2D Position)
{
	const int32 Row = FMath::FloorToInt(Position.X);
	const int32 Col = FMath::FloorToInt(Position.Y);

	return GameFieldData->Lines[Row].LineContent[Col];

}

void ATicTacToeGameMode::SetFigureOnPosition(UFieldData *GameFieldData, FVector2D Position, EFigure Figure)
{
	const int32 Row = FMath::FloorToInt(Position.X);
	const int32 Col = FMath::FloorToInt(Position.Y);
	if (GameFieldData != nullptr)
	{
		GameFieldData->Lines[Row].LineContent[Col] = Figure;
	}
}

ATicTacToeGameMode::ATicTacToeGameMode()
{
	PlayerControllerClass = ATicTacToePlayerController::StaticClass();
	DefaultPawnClass = ATicTacToePawn::StaticClass();
	CrossFigureActor = ACrossFigureActor::StaticClass();
    CircleFigureActor = ACircleFigureActor::StaticClass();


	
}

ATicTacToeGameMode::~ATicTacToeGameMode()
{
}

void ATicTacToeGameMode::BeginPlay()
{
	
	Super::BeginPlay();
	//GameStart();

	
}

void ATicTacToeGameMode::GameStart(int32 PlayerIndex)
{


	GameData = NewObject<UFieldData>();


	FieldSize = 3;

	ResetGameState();
	
	IsGameOver = false;
	//спавн ИИ игрока
	ATicTacToePawn* AI = GetWorld()->SpawnActor<ATicTacToePawn>(FVector(), FRotator());
	Players.Add(AI);

	//назначаем веса для победы и поражений в зависимости от того на какой стороне игрок, необходимы для выявления выигрышных ходов ИИ игрока
	WeightMap.Add(EFigure::Empty, 0);
	WeightMap.Add(EFigure::Draw, 0);
	WeightMap.Add(EFigure::Circle, 0 == PlayerIndex ? 100 : -100);
	WeightMap.Add(EFigure::Cross, 0 == PlayerIndex ? -100 : 100);
	

	
	//назначаем крестик или нолик
	CurrentPlayerIndex = PlayerIndex;
	UserSign = 0 == PlayerIndex ? EFigure::Cross : EFigure::Circle;
	ComputerSign = 1 == PlayerIndex ? EFigure::Cross : EFigure::Circle;
	for (int32 i = 0; i < Players.Num(); i++)
	{
		Players[i]->PlayerIndex = i;
		Players[i]->Sign = i == CurrentPlayerIndex ? EFigure::Cross : EFigure::Circle;
	}
	
	//создание игрового поля
	GameField = GetWorld()->SpawnActor<AGameField>(FVector(0), FRotator(0, 0, 0), FActorSpawnParameters());

	//даем первый ход
	Players[CurrentPlayerIndex]->OnTurn();
}

APawn* ATicTacToeGameMode::SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer, const FTransform& SpawnTransform)
{
	//переназначаем метод для спавна пешки по умолчанию 
	ATicTacToePawn* Pawn = GetWorld()->SpawnActor<ATicTacToePawn>(SpawnTransform.GetLocation(), SpawnTransform.Rotator());
	Pawn->Camera->SetWorldLocationAndRotation(FVector(0, 0, 2000.0f), FRotationMatrix::MakeFromX(FVector(0, 0, -1)).Rotator());
	NewPlayer->Possess(Pawn);
	Players.Add(Pawn);
	return Pawn;
}

void ATicTacToeGameMode::MakeTurn(const int32 PlayerNumber, const FVector2D& Position)
{
	//Ход, если позиция пустая назначаем на это место фигуру соотвествующую игроку
	if (!IsGameOver && GetFigureOnPosition(GameData, Position) == EFigure::Empty)
	{
		UClass* SignActor = Players[CurrentPlayerIndex]->Sign == EFigure::Cross ? CrossFigureActor : CircleFigureActor;
		const FVector Location = GameField->GetActorLocation() + GameField->GetRelativeLocationByPosition(Position) + FVector(0, 0, 50);
		GetWorld()->SpawnActor(SignActor, &Location);

		SetFigureOnPosition(GameData, Position,Players[PlayerNumber]->Sign );
		CheckForWin(GameData, true);
		TurnNextPlayer();
	}


	
}

int32 ATicTacToeGameMode::GetNextPlayer(int32 Player)
{
	Player++;
	if (!Players.IsValidIndex(Player))
	{
		Player = 0;
	}
	return Player;
}

void ATicTacToeGameMode::TurnNextPlayer()
{
	CurrentPlayerIndex = GetNextPlayer(CurrentPlayerIndex);
	Players[CurrentPlayerIndex]->OnTurn();
}
//проверка выиграл ли игрок, нужна для алгоритма минимакс
bool ATicTacToeGameMode::CheckIsPlayerWin(UFieldData *GameFieldData, EFigure PlayerSign)
{
	for (int32 i = 0; i<GameFieldData->Lines.Num(); i ++)
	{
		//Проверка горизонтальных линий на победу
		bool isWiningFound = GetFigureOnPosition(GameFieldData, FVector2D(i, 0)) ==  GetFigureOnPosition(GameFieldData,FVector2D(i, 1)) &&
			GetFigureOnPosition(GameFieldData,FVector2D(i, 0)) ==  GetFigureOnPosition(GameFieldData,FVector2D(i, 2)) &&
				GetFigureOnPosition(GameFieldData,FVector2D(i, 0)) != EFigure::Empty;
		if (isWiningFound && GetFigureOnPosition(GameFieldData, FVector2D(i, 0)) == PlayerSign)
		{
			return true;
		}
		//Проверка вертикальных линий на победу
		isWiningFound = GetFigureOnPosition(GameFieldData,FVector2D(0, i)) ==  GetFigureOnPosition(GameFieldData,FVector2D(1, i)) &&
			GetFigureOnPosition(GameFieldData,FVector2D(0, i)) ==  GetFigureOnPosition(GameFieldData,FVector2D(2, i)) &&
				GetFigureOnPosition(GameFieldData,FVector2D(0, i)) != EFigure::Empty;
		if (isWiningFound && GetFigureOnPosition(GameFieldData, FVector2D(0, i)) == PlayerSign)
		{
			return true;
		}
		//Проверка диагональных линий на победу
		isWiningFound = GetFigureOnPosition(GameFieldData,FVector2D(0, 0)) ==  GetFigureOnPosition(GameFieldData,FVector2D(1, 1)) &&
			GetFigureOnPosition(GameFieldData,FVector2D(0, 0)) ==  GetFigureOnPosition(GameFieldData,FVector2D(2, 2)) &&
				GetFigureOnPosition(GameFieldData,FVector2D(1, 1)) != EFigure::Empty;
		if (isWiningFound && GetFigureOnPosition(GameFieldData, FVector2D(1, 1)) == PlayerSign)
		{
			return true;
		}
		isWiningFound = GetFigureOnPosition(GameFieldData,FVector2D(0, 2)) ==  GetFigureOnPosition(GameFieldData,FVector2D(1, 1)) &&
			GetFigureOnPosition(GameFieldData,FVector2D(2, 0)) ==  GetFigureOnPosition(GameFieldData,FVector2D(0, 2)) &&
				GetFigureOnPosition(GameFieldData,FVector2D(1, 1)) != EFigure::Empty;
		if (isWiningFound && GetFigureOnPosition(GameFieldData, FVector2D(1, 1)) == PlayerSign)
		{
			return true;
		}
	}
	return false;
}

//проверка состояния стола на ничью,
bool ATicTacToeGameMode::CheckIsDraw(UFieldData* GameFieldData)
{
	int32 AvailableTurns = 9;
	for (int32 i = 0; i < FieldSize; i++)
	{
		for (int32 j = 0; j < FieldSize; j++)
		{
			if (GetFigureOnPosition(GameFieldData,FVector2D(i, j)) != EFigure::Empty)
			{
				AvailableTurns--;
			}
		}
	}
	if (AvailableTurns == 0)
	{
		return  true;
	}
	else
	{
		return  false;
	}
}


//проверка состояния стола на победу, с возвратом победившей фигуры
EFigure ATicTacToeGameMode::CheckForWin(UFieldData *GameFieldData, bool IsOriginalGameField)
{
	bool isWiningFound = false;
	for (int32 i = 0; i<GameFieldData->Lines.Num(); i ++)
	{
		//Проверка горизонтальных линий на победу
		isWiningFound = GetFigureOnPosition(GameFieldData, FVector2D(i, 0)) ==  GetFigureOnPosition(GameFieldData,FVector2D(i, 1)) &&
			GetFigureOnPosition(GameFieldData,FVector2D(i, 0)) ==  GetFigureOnPosition(GameFieldData,FVector2D(i, 2)) &&
				GetFigureOnPosition(GameFieldData,FVector2D(i, 0)) != EFigure::Empty;
		if (isWiningFound)
		{
			if(IsOriginalGameField)
			{
				//если доска не расчетная и найдена выигрышная комбинация то останавливаем игру
				OnWin(GetFigureOnPosition(GameFieldData, FVector2D(i, 0)));
			}
			return GetFigureOnPosition(GameFieldData, FVector2D(i, 0));
		}
		//Проверка вертикальных линий на победу
		isWiningFound = GetFigureOnPosition(GameFieldData,FVector2D(0, i)) ==  GetFigureOnPosition(GameFieldData,FVector2D(1, i)) &&
			GetFigureOnPosition(GameFieldData,FVector2D(0, i)) ==  GetFigureOnPosition(GameFieldData,FVector2D(2, i)) &&
				GetFigureOnPosition(GameFieldData,FVector2D(0, i)) != EFigure::Empty;
		if (isWiningFound)
		{
			if(IsOriginalGameField)
			{
				//если доска не расчетная и найдена выигрышная комбинация то останавливаем игру
				OnWin(GetFigureOnPosition(GameFieldData,FVector2D(0, i)));
			}
			return GetFigureOnPosition(GameFieldData,FVector2D(0, i));
		}
		//Проверка диагональных линий на победу
		isWiningFound = GetFigureOnPosition(GameFieldData,FVector2D(0, 0)) ==  GetFigureOnPosition(GameFieldData,FVector2D(1, 1)) &&
			GetFigureOnPosition(GameFieldData,FVector2D(0, 0)) ==  GetFigureOnPosition(GameFieldData,FVector2D(2, 2)) &&
				GetFigureOnPosition(GameFieldData,FVector2D(1, 1)) != EFigure::Empty;
		if (isWiningFound)
		{
			if(IsOriginalGameField)
			{
				//если доска не расчетная и найдена выигрышная комбинация то останавливаем игру
				OnWin(GetFigureOnPosition(GameFieldData,FVector2D(1, 1)));
			}
			return GetFigureOnPosition(GameFieldData,FVector2D(1, 1));
		}
		isWiningFound = GetFigureOnPosition(GameFieldData,FVector2D(0, 2)) ==  GetFigureOnPosition(GameFieldData,FVector2D(1, 1)) &&
			GetFigureOnPosition(GameFieldData,FVector2D(2, 0)) ==  GetFigureOnPosition(GameFieldData,FVector2D(0, 2)) &&
				GetFigureOnPosition(GameFieldData,FVector2D(1, 1)) != EFigure::Empty;
		if (isWiningFound)
		{
			if(IsOriginalGameField)
			{
				//если доска не расчетная и найдена выигрышная комбинация то останавливаем игру
				OnWin(GetFigureOnPosition(GameFieldData,FVector2D(1, 1)));
			}
			return GetFigureOnPosition(GameFieldData,FVector2D(1, 1));
		}
	}
	
	return EFigure::Empty;
}

void ATicTacToeGameMode::OnWin(EFigure WinnerFigure)
{
	IsGameOver = true;

	//устанавливаем победное свечение для фигур победителя!
	TArray<AActor*> WinnerFigures;

	//выбираем какой класс фигур искать для подсвечивания
	TSubclassOf<AActor> WinnerFigureSublass = WinnerFigure == EFigure::Cross ? ACrossFigureActor::StaticClass() : ACircleFigureActor::StaticClass(); 

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), WinnerFigureSublass, WinnerFigures);
	for (int i = 0; i < WinnerFigures.Num(); i++)
	 	{
	 		Cast<AFigureBase>(WinnerFigures[i])->Mesh->SetMaterial(1, Cast<AFigureBase>(WinnerFigures[i])->WinFigureMaterial);
		}
	
}

void ATicTacToeGameMode::ResetGame()
{
	//сброс игры, очищаем поле, удаляем все фигуры на поле
	ResetGameState();
	TArray<AActor*> figures;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AFigureBase::StaticClass(), figures);
	
	if (figures.IsValidIndex(0))
	{
		for (int i = 0; i < figures.Num(); i++)
		{
			figures[i]->Destroy();
		}
	
	}

	IsGameOver = false;
	if (Players[CurrentPlayerIndex]->Sign == EFigure::Cross)
	{
		Players[CurrentPlayerIndex]->OnTurn();
	}
	else
	{
		TurnNextPlayer();
	}
}



// Fill out your copyright notice in the Description page of Project Settings.


#include "TestTaskB/Core/AITicTacToeController.h"
#include "TicTacToeGameMode.h"

AAITicTacToeController::AAITicTacToeController()
{
	bStartAILogicOnPossess = true;
}

AAITicTacToeController::~AAITicTacToeController()
{
}

void AAITicTacToeController::BeginPlay()
{
	Super::BeginPlay();
	GameMode = StaticCast<ATicTacToeGameMode*>(GetWorld()->GetAuthGameMode());
}


void AAITicTacToeController::MakeAiTurn()
{
	
	//UE_LOG(LogTemp, Error, TEXT("AI turn begins!"));
	//вне расчетов алгоритма Минимакс занимаем центральную точку если она свободна
	 if (GameMode->GetFigureOnPosition(GameMode->GameData , FVector2D(1,1)) == EFigure::Empty)
	 {
	 	GameMode->MakeTurn(GameMode->CurrentPlayerIndex, FVector2D(1,1));
	 	return;
	 }
	
	int32 BestWeight = TNumericLimits<int32>::Min(); //Min
	
	//создаем копию игровой доски для расчетов
	UFieldData* GameDataCopy = GameMode->GameData;
	FVector2D BestTurn = FVector2D();
	//проходим по расчетной доске и находим лучший ход по алгоритму минимакс
	for (int32 i = 0; i < GameMode->FieldSize; i++)
	{
		for (int32 j = 0; j < GameMode->FieldSize; j++)
		{
			
			if (GameMode->GetFigureOnPosition(GameDataCopy , FVector2D(i,j)) == EFigure::Empty)
			{
				GameMode->SetFigureOnPosition(GameDataCopy, FVector2D(i,j), GameMode->ComputerSign);
				int32 Weight = Minimax(GameDataCopy,false); 
				GameMode->SetFigureOnPosition(GameDataCopy, FVector2D(i,j), EFigure::Empty);
				if (Weight > BestWeight)
				{
					BestWeight = Weight;
					BestTurn = FVector2D(i,j);
				}
			}
		}
	}
	FString output = BestTurn.ToString(); 
	//UE_LOG(LogTemp, Error, TEXT("AI best turn %s!"), *output);
	GameMode->MakeTurn(GameMode->CurrentPlayerIndex, BestTurn);
}


int32 AAITicTacToeController::Minimax(UFieldData *GameFieldData, bool IsMax)
{
	
	//Реализация алгоритма Минимаrc свойственного для игр с открытым полем
	
	//прежде всего проверяем не будет ли комбинация выигрышной
	if (GameMode->CheckIsPlayerWin(GameFieldData, GameMode->ComputerSign))
	{
		int32* ReturnValue = GameMode->WeightMap.Find(GameMode->ComputerSign);
		return *ReturnValue;
	}
	if (GameMode->CheckIsPlayerWin(GameFieldData, GameMode->UserSign))
	{
		int32* ReturnValue = GameMode->WeightMap.Find(GameMode->UserSign);
		return *ReturnValue;
	}
	if (GameMode->CheckIsDraw(GameFieldData))
	{
		int32* ReturnValue = GameMode->WeightMap.Find(EFigure::Draw);
		return *ReturnValue;
	}

	//выбор какой ход будет максимально полезным для ИИ
	if (IsMax)
	{
		
		int32 BestScore = TNumericLimits<int32>::Min();
		for (int32 r = 0; r < GameMode->FieldSize; r++)
		{
			for (int32 c = 0; c < GameMode->FieldSize; c++)
			{
				if (GameMode->GetFigureOnPosition(GameFieldData, FVector2D(r,c)) == EFigure::Empty)
				{
					GameMode->SetFigureOnPosition(GameFieldData, FVector2D(r,c), GameMode->ComputerSign);
							
					int Score = Minimax(GameFieldData, false); 
					GameMode->SetFigureOnPosition(GameFieldData, FVector2D(r,c), EFigure::Empty);
					BestScore = FMath::Max(Score, BestScore);
					//UE_LOG(LogTemp, Warning, TEXT("Best Score on %d %d is %d"), r, c, BestScore );
				}
			}
		}
		return BestScore;
	}
	// противник выбирает ход который нам не выгоден
	else
	{
		
		int32 BestScore = TNumericLimits<int32>::Max();
		
		for (int32 r = 0; r < GameMode->FieldSize; r++)
		{
			for (int32 c = 0; c < GameMode->FieldSize; c++)
			{
				if (GameMode->GetFigureOnPosition(GameFieldData, FVector2D(r,c)) == EFigure::Empty)
				{
					
					GameMode->SetFigureOnPosition(GameFieldData, FVector2D(r,c), GameMode->UserSign);
					int32 Score = Minimax(GameFieldData, true);
					GameMode->SetFigureOnPosition(GameFieldData, FVector2D(r,c), EFigure::Empty);
					BestScore = FMath::Min(Score, BestScore);
					//UE_LOG(LogTemp, Warning, TEXT("Best Score on %d %d is %d"), r, c, BestScore );
				}
			}
		}
		return  BestScore;
	}
	
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "TestTaskB/Pawns/TicTacToePawn.h"
#include "TestTaskB/Core/AITicTacToeController.h"
#include "Kismet/GameplayStatics.h"
#include "TestTaskB/Core/TicTacToeGameMode.h"
#include "TestTaskB/Core/TicTacToePlayerController.h"

// Sets default values
ATicTacToePawn::ATicTacToePawn() 
{
	AIControllerClass = AAITicTacToeController::StaticClass();
	AutoPossessAI = EAutoPossessAI::Spawned;
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;



	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->AttachToComponent(RootComponent,FAttachmentTransformRules::KeepRelativeTransform,"None");
}

// Called when the game starts or when spawned
void ATicTacToePawn::BeginPlay()
{
	Super::BeginPlay();

	if (Cast<ATicTacToePlayerController>(GetController()))
		{
		IsAIControlled = false;
		}
	else
		{
		IsAIControlled = true;
		AiController = Cast<AAITicTacToeController>(GetController());
		}
	GameMode = StaticCast<ATicTacToeGameMode*>(GetWorld()->GetAuthGameMode());
}

// Called every frame
void ATicTacToePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATicTacToePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	//основное действие, по умолчанию Левая кнопка мыши
	
	InputComponent->BindAction("Click", IE_Pressed, this, &ATicTacToePawn::OnClick);
	//Сброс, по умолчанию правая кнопка мыши
	InputComponent->BindAction("Reset", IE_Pressed, this, &ATicTacToePawn::OnResetClick);
}

void ATicTacToePawn::OnTurn()
{
	//если ии то позволяем уме сделать ход
	if (IsAIControlled)
	{
		AiController->MakeAiTurn();
	}

}



void ATicTacToePawn::OnClick()
{
	FHitResult Hit = FHitResult(ForceInit);
	GetWorld()->GetFirstPlayerController()->GetHitResultUnderCursor(ECollisionChannel::ECC_Pawn, true, Hit);
	
	if (Hit.bBlockingHit)
	{
		if (AGameField* Field = Cast<AGameField>(Hit.GetActor()))
		{
			const FVector2D Position = Field->GetPosition(Hit);
			GameMode->MakeTurn(PlayerIndex, Position);
		}
	}
}

void ATicTacToePawn::OnResetClick()
{
	
	GameMode->ResetGame();
}


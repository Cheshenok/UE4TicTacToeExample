// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TestTaskB/Core/TicTacToeGameState.h"

#include "TestTaskB/Field/GameField.h"
//#include "TestTaskB/Pawns/TicTacToePawn.h"
#include "TicTacToeGameMode.generated.h"

/**
 * 
 */


UCLASS()
class TESTTASKB_API ATicTacToeGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	bool IsGameOver;

	
	UPROPERTY(BlueprintReadOnly)
	AGameField* GameField;

	UPROPERTY(BlueprintReadOnly)
	TArray<ATicTacToePawn*> Players;

	UPROPERTY(BlueprintReadOnly)
	EFigure UserSign;

	UPROPERTY(BlueprintReadOnly)
	EFigure ComputerSign;

	UPROPERTY(BlueprintReadOnly)
	int32 CurrentPlayerIndex;

	UPROPERTY(BlueprintReadOnly)
	int32 FieldSize;

	UPROPERTY(BlueprintReadOnly)
	TSubclassOf<class AActor> CrossFigureActor;

	UPROPERTY(BlueprintReadOnly)
	TSubclassOf<class AActor> CircleFigureActor;

	UPROPERTY()
	UFieldData* GameData;

	UPROPERTY()
	TMap<EFigure, int32> WeightMap;

	UFUNCTION()
	void ResetGameState();

	UFUNCTION()
	EFigure GetFigureOnPosition(UFieldData *GameFieldData, FVector2D Position);

	UFUNCTION()
	void SetFigureOnPosition(UFieldData *GameFieldData, FVector2D Position, EFigure Figure);

	ATicTacToeGameMode();

	~ATicTacToeGameMode();

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void GameStart(int32 PlayerIndex);
	
	UFUNCTION()
	virtual  APawn* SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer, const FTransform& SpawnTransform) override;

	UFUNCTION()
	void MakeTurn(const int32 PlayerNumber, const FVector2D& Position);

	UFUNCTION()
	int32 GetNextPlayer(int32 Player);

	UFUNCTION()
	void TurnNextPlayer();

	UFUNCTION()
	EFigure CheckForWin(UFieldData *GameFieldData, bool IsOriginalGameField);
	
	UFUNCTION()
	bool CheckIsPlayerWin(UFieldData *GameFieldData, EFigure PlayerSign);

	UFUNCTION()
	bool CheckIsDraw(UFieldData *GameFieldData);

	UFUNCTION()
	void OnWin(EFigure WinnerFigure);
	
	UFUNCTION(BlueprintCallable)
	void ResetGame();

};




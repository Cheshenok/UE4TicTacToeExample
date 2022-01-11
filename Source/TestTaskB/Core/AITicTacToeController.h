// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "TestTaskB/Field/FieldData.h"
#include "AITicTacToeController.generated.h"

class ATicTacToeGameMode;
/**
 * 
 */
UCLASS()
class TESTTASKB_API AAITicTacToeController : public AAIController
{
	GENERATED_BODY()

	AAITicTacToeController();
	~AAITicTacToeController();
	public:
	
	UPROPERTY()
	ATicTacToeGameMode* GameMode;

	//UPROPERTY()
	//FVector2D BestTurn;
	
	UFUNCTION()
	void MakeAiTurn();

	protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	private:


	
	UFUNCTION()
	int32 Minimax(UFieldData *GameFieldData, bool IsMax);
};

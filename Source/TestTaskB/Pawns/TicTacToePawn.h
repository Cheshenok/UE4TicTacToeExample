// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "TestTaskB/Field/GameField.h"
#include "TestTaskB/Field/FieldData.h"
#include "GameFramework/Pawn.h"

#include "TestTaskB/Core/AITicTacToeController.h"
#include "TicTacToePawn.generated.h"


UCLASS()
class TESTTASKB_API ATicTacToePawn : public APawn
{
	GENERATED_BODY()

public:
	UPROPERTY()
	int32 PlayerIndex;

	//фигура игрока: крестик или нолик
	UPROPERTY()
	EFigure Sign;

	UPROPERTY(BlueprintReadOnly, VisibleInstanceOnly)
	bool IsAIControlled;
	
	UPROPERTY()
	UCameraComponent* Camera;

	UPROPERTY()
	AAITicTacToeController* AiController;

	UPROPERTY()
	ATicTacToeGameMode* GameMode;
	
	// Sets default values for this pawn's properties
	ATicTacToePawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//Вызывается при ходе игрока
	virtual void OnTurn();

	//клик вызывает ход игрока
	UFUNCTION()
	void OnClick();

	//клик вызывает сброс игры в исходное состояние
	UFUNCTION()
	void OnResetClick();
};

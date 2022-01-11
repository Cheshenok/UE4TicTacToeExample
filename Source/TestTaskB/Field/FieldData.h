// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "FieldData.generated.h"

UENUM()
enum class EFigure : uint8
{
	Empty,
	Cross,
	Circle,
	Draw, //состояние ничьи тоже записываем сюда
};


USTRUCT()
struct FLineData
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<EFigure> LineContent;

	FLineData()
	{
		LineContent = {EFigure::Empty, EFigure::Empty, EFigure::Empty};
	}
};
/**
 * 
 */
UCLASS()
class TESTTASKB_API UFieldData : public UObject
{
	GENERATED_BODY()

	public:
	UPROPERTY()
	TArray<FLineData> Lines;

	UFUNCTION()
	void Reset();
	
	UFieldData();
	~UFieldData();
};

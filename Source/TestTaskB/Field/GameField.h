// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameField.generated.h"

/*
struct FPosition
{
	int32 X;
	int32 Y;

	FPosition()
	{
		X = 0;
		Y = 0;
	}

	FPosition(int32 TargetX, int32 TargetY)
	{
		X = TargetX;
		Y = TargetY;
	}

	const bool operator==(const FPosition& CheckPos) const {
		return X == CheckPos.X && Y == CheckPos.Y;
	}
};
*/

UCLASS()
class TESTTASKB_API AGameField : public AActor
{
	GENERATED_BODY()
	
public:

	TMap<UPrimitiveComponent*, FVector2D> Positions;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 Size;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UStaticMesh* TileStaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UMaterial* TileMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float TilePadding;

	virtual void OnConstruction(const FTransform& Transform) override;
	// Sets default values for this actor's properties
	AGameField();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void GenerateField();

	FVector2D GetPosition(const FHitResult& Hit);

	FVector GetRelativeLocationByPosition(const FVector2D& Position);

};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Materials/MaterialInstanceConstant.h"
#include "FigureBase.generated.h"

UCLASS()
class TESTTASKB_API AFigureBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFigureBase();

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Mesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UStaticMesh* FigureStaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UMaterialInstanceConstant* WinFigureMaterial;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

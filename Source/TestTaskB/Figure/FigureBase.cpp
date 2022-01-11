// Fill out your copyright notice in the Description page of Project Settings.


#include "TestTaskB/Figure/FigureBase.h"

// Sets default values
AFigureBase::AFigureBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	Mesh = NewObject<UStaticMeshComponent>(this, "Mesh");
	Mesh->AttachToComponent(GetRootComponent(),FAttachmentTransformRules::KeepRelativeTransform);
	Mesh->SetSimulatePhysics(true);

	static ConstructorHelpers::FObjectFinder<UMaterialInstanceConstant> FoundMaterialInstance(TEXT("MaterialInstanceConstant'/Game/Figures/Materials/Mi_Emision_Green.Mi_Emision_Green'"));
	WinFigureMaterial = FoundMaterialInstance.Object;
	
	

}

// Called when the game starts or when spawned
void AFigureBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFigureBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


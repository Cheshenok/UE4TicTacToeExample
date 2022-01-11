// Fill out your copyright notice in the Description page of Project Settings.


#include "TestTaskB/Field/GameField.h"

void AGameField::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	//Генерируем поле только если еще не сгенерировано
	if(!GetRootComponent()->GetNumChildrenComponents())
	{
		GenerateField();
	}
}

// Sets default values
AGameField::AGameField()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	Size = 3;
	TilePadding = 5;
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> FoundTileStaticMesh(TEXT("StaticMesh'/Game/Figures/SM_Tile.SM_Tile'"));
	TileStaticMesh = FoundTileStaticMesh.Object;



}

// Called when the game starts or when spawned
void AGameField::BeginPlay()
{
	Super::BeginPlay();
	GenerateField();
}

// Called every frame
void AGameField::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGameField::GenerateField()
{
	while (GetRootComponent()->GetNumChildrenComponents())
	{
		USceneComponent* c = GetRootComponent()->GetChildComponent(GetRootComponent()->GetNumChildrenComponents() - 1);
		c->DestroyComponent();
	}
	Positions.Empty();

	for (int32 x = 0; x < Size; x++)
		for (int32 y = 0; y < Size; y++)
		{
			FName name = FName(*FString::Printf(TEXT("Cell[%d,%d]"), x, y));

			UStaticMeshComponent* mesh = NewObject<UStaticMeshComponent>(this, name);
			mesh->AttachToComponent(GetRootComponent(),FAttachmentTransformRules::KeepRelativeTransform);
			mesh->RegisterComponentWithWorld(GetWorld());
			mesh->SetStaticMesh(TileStaticMesh);
			mesh->SetMaterial(0, TileMaterial);
			mesh->SetRelativeLocation(GetRelativeLocationByPosition(FVector2D(x, y)));
			Positions.Add(mesh, FVector2D(x, y));
		}
}

FVector2D AGameField::GetPosition(const FHitResult& Hit)
{
	return Positions[Hit.Component.Get()];
}

FVector AGameField::GetRelativeLocationByPosition(const FVector2D& Position)
{
	return FVector(Position.X - (Size - 1) / 2.0, Position.Y - (Size - 1) / 2.0, 0) * (TileStaticMesh->ExtendedBounds.BoxExtent + TilePadding) * 2 - TileStaticMesh->ExtendedBounds.Origin;
}




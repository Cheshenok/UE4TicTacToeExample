// Fill out your copyright notice in the Description page of Project Settings.


#include "TestTaskB/Figure/CrossFigureActor.h"

ACrossFigureActor::ACrossFigureActor() : Super()
{
	
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> FoundStaticMesh(TEXT("StaticMesh'/Game/Figures/SM_Cross.SM_Cross'"));
	FigureStaticMesh = FoundStaticMesh.Object;

	//static ConstructorHelpers::FObjectFinder<UMaterial> FoundMaterial(TEXT("Material'/Game/Figures/M_Figure_Black.M_Figure_Black'"));
	//FigureMaterial = FoundMaterial.Object;

	Mesh->SetStaticMesh(FigureStaticMesh);
	//Mesh->SetMaterial(0, FigureMaterial);

	Mesh->AttachToComponent(RootComponent,FAttachmentTransformRules::KeepRelativeTransform,"None");
}

ACrossFigureActor::~ACrossFigureActor()
{
}

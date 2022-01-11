// Fill out your copyright notice in the Description page of Project Settings.


#include "TestTaskB/Figure/CircleFigureActor.h"

ACircleFigureActor::ACircleFigureActor() : Super()
{
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> FoundStaticMesh(TEXT("StaticMesh'/Game/Figures/SM_Circle.SM_Circle'"));
	FigureStaticMesh = FoundStaticMesh.Object;

	Mesh->SetStaticMesh(FigureStaticMesh);
	Mesh->AttachToComponent(RootComponent,FAttachmentTransformRules::KeepRelativeTransform,"None");
}

ACircleFigureActor::~ACircleFigureActor()
{
}

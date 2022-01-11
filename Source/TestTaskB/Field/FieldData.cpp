// Fill out your copyright notice in the Description page of Project Settings.


#include "TestTaskB/Field/FieldData.h"

void UFieldData::Reset()
{
	if (Lines.IsValidIndex(0))
	{
		for (int32 l = 0 ; l < Lines.Num(); l++	)
		{
			Lines[l] = FLineData();
		}
	}
}

UFieldData::UFieldData()
{
	Lines = {FLineData(), FLineData(), FLineData()};
}

UFieldData::~UFieldData()
{
}

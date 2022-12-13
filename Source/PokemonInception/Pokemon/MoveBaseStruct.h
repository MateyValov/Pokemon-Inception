// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "TypeStruct.h"
#include "MoveBaseStruct.generated.h"


USTRUCT(BlueprintType)
struct FMoveBaseStruct : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Name;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//FTypeStruct Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int PowerPoints;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "IconMappingStyle.generated.h"

/**
 * 
 */
UCLASS(Abstract, Blueprintable, MinimalAPI, ClassGroup = UI, meta = (Category = "File Dialog"))
class UIconMappingStyle : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Properties")
	TMap<FString, FSlateBrush> IconMappings;
};

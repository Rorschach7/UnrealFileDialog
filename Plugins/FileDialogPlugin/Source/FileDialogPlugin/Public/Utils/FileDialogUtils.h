// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "FileDialogUtils.generated.h"

/**
 * 
 */
UCLASS()
class FILEDIALOGPLUGIN_API UFileDialogUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	static TArray<FString> GetDrives(bool bLetterOnly);
	
	static FString GetDirectoryName(const FString& Path);
};

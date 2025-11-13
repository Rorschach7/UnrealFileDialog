// Fill out your copyright notice in the Description page of Project Settings.


#include "Utils/FileDialogUtils.h"

TArray<FString> UFileDialogUtils::GetDrives()
{
	TArray<FString> Drives;
	FString Letters = TEXT("CDEFGHIJKLMNOPQRSTUVWXYZ");
	FString Drive;

	for (int32 i = 0; i < Letters.Len(); i++)
	{
		Drive = FString(1, (&Letters.GetCharArray()[i]));
		Drive.Append(TEXT(":/"));
		
		const FString FullPath = FPaths::GetCleanFilename(Drive);
		const FFileStatData Data = FPlatformFileManager::Get().GetPlatformFile().GetStatData(*Drive);
		if (Data.bIsDirectory)
		{
			Drives.Add(Drive);
		}
	}
	return Drives;
}

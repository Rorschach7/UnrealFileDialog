// Fill out your copyright notice in the Description page of Project Settings.


#include "Utils/FileDialogUtils.h"

TArray<FString> UFileDialogUtils::GetDrives(bool bLetterOnly)
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
			if (bLetterOnly)
			{
				FString DriveLetter = Drive.Left(1);
				Drives.Add(DriveLetter);
			}
			else
			{
				Drives.Add(Drive);				
			}
		}
	}
	return Drives;
}

FString UFileDialogUtils::GetDirectoryName(const FString& Path)
{
	int32 LastSlashIndex;

	if (Path.FindLastChar(TEXT('/'), LastSlashIndex) || Path.FindLastChar(TEXT('\\'), LastSlashIndex))
	{
		return Path.Mid(LastSlashIndex + 1);
	}

	return Path; // If no separator is found, return the whole path
}

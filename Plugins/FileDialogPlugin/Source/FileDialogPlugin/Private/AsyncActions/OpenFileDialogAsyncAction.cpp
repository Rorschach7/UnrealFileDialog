﻿// Copyright Kevin Lichtenberg. All Rights Reserved. kevlicht@gmail.com

#include "AsyncActions/OpenFileDialogAsyncAction.h"

#include "RHITransientResourceAllocator.h"
#include "Widgets/FileDialogWidget.h"

UOpenFileDialogAsyncAction* UOpenFileDialogAsyncAction::ShowOpenFileDialogAsync(UObject* WorldContextObject,
	UFileDialogWidget* FileDialogWidget, const FText& WindowTitle, const FText& ConfirmText, const FText& CancelText,
	const FString& StartLocation, TArray<FString> FileExtensions)
{
	if (!FileDialogWidget)
	{
		UE_LOG(LogTemp, Error, TEXT("File Dialog did not open: FileDialogWidget is null!"));
		return nullptr;
	}
	UOpenFileDialogAsyncAction* Task = NewObject<UOpenFileDialogAsyncAction>(WorldContextObject);
	FileDialogWidget->OnDialogClosed().AddUObject(Task, &UOpenFileDialogAsyncAction::HandleFileDialogCompleted);
	const bool bOpenedDialog = FileDialogWidget->SetAsOpenFileDialog(WindowTitle, ConfirmText, CancelText,
	                                                                 StartLocation, FileExtensions);
	if (!bOpenedDialog)
	{
		return Task;
	}

	if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		Task->WorldContext = World;
		Task->DialogWidget = FileDialogWidget;
		Task->RegisterWithGameInstance(World);
	}

	return Task;
}

UOpenFileDialogAsyncAction* UOpenFileDialogAsyncAction::ShowOpenDirectoryDialogAsync(UObject* WorldContextObject,
	UFileDialogWidget* FileDialogWidget, const FText& WindowTitle, const FText& ConfirmText, const FText& CancelText,
	const FString& StartLocation)
{
	if (!FileDialogWidget)
	{
		UE_LOG(LogTemp, Error, TEXT("File Dialog did not open: FileDialogWidget is null!"));
		return nullptr;
	}
	UOpenFileDialogAsyncAction* Task = NewObject<UOpenFileDialogAsyncAction>(WorldContextObject);
	FileDialogWidget->OnDialogClosed().AddUObject(Task, &UOpenFileDialogAsyncAction::HandleFileDialogCompleted);
	const bool bOpenedDialog = FileDialogWidget->SetAsOpenDirectoryDialog(WindowTitle, ConfirmText, CancelText,
	                                                                      StartLocation);
	if (!bOpenedDialog)
	{
		return Task;
	}

	if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		Task->WorldContext = World;
		Task->DialogWidget = FileDialogWidget;
		Task->RegisterWithGameInstance(World);
	}

	return Task;
}

UOpenFileDialogAsyncAction* UOpenFileDialogAsyncAction::ShowSaveFileDialogAsync(UObject* WorldContextObject,
	UFileDialogWidget* FileDialogWidget, const FText& WindowTitle, const FText& ConfirmText, const FText& CancelText,
	const FString& StartLocation, const FString& FileNameSuggestion, const FString& InFileExtension)
{
	if (!FileDialogWidget)
	{
		UE_LOG(LogTemp, Error, TEXT("File Dialog did not open: FileDialogWidget is null!"));
		return nullptr;
	}
	UOpenFileDialogAsyncAction* Task = NewObject<UOpenFileDialogAsyncAction>(WorldContextObject);
	FileDialogWidget->OnDialogClosed().AddUObject(Task, &UOpenFileDialogAsyncAction::HandleFileDialogCompleted);
	const bool bOpenedDialog = FileDialogWidget->SetAsSaveFileDialog(WindowTitle, ConfirmText, CancelText,
	                                                                 StartLocation, FileNameSuggestion,
	                                                                 InFileExtension);
	if (!bOpenedDialog)
	{
		return Task;
	}

	if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		Task->WorldContext = World;
		Task->DialogWidget = FileDialogWidget;
		Task->RegisterWithGameInstance(World);
	}

	return Task;
}

void UOpenFileDialogAsyncAction::Activate()
{
	Super::Activate();
}

void UOpenFileDialogAsyncAction::HandleFileDialogCompleted(bool bCanceled, FString FilePath)
{
	OnFileDialogCompleted.Broadcast(bCanceled, FilePath);
	if (DialogWidget)
	{
		DialogWidget->OnDialogClosed().RemoveAll(this);
	}
	SetReadyToDestroy();
}

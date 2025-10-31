// Copyright Kevin Lichtenberg. All Rights Reserved. kevlicht@gmail.com


#include "Widgets/FileDialogWidget.h"

#include "CommonButtonBase.h"
#include "CommonTextBlock.h"
#include "Components/EditableTextBox.h"
#include "DSP/Encoders/OpusEncoder.h"
#include "Widgets/FileDialogButton.h"
#include "Widgets/FileDialogEntryWidget.h"

void UFileDialogWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (DirBackBtn)
	{
		DirBackBtn->OnClicked().AddUObject(this, &UFileDialogWidget::GoPrevDir);
	}
	if (DirFwdBtn)
	{
		DirFwdBtn->OnClicked().AddUObject(this, &UFileDialogWidget::GoFwdDir);
	}
	if (DirUpBtn)
	{
		DirUpBtn->OnClicked().AddUObject(this, &UFileDialogWidget::GoDirUp);
	}
	if (RefreshBtn)
	{
		RefreshBtn->OnClicked().AddUObject(this, &UFileDialogWidget::Refresh);
	}
	if (ConfirmBtn)
	{
		ConfirmBtn->OnClicked().AddUObject(this, &UFileDialogWidget::HandleConfirm);
	}
	if (CancelBtn)
	{
		CancelBtn->OnClicked().AddUObject(this, &UFileDialogWidget::HandleCancel);
	}
	if (DirBox)
	{
		DirBox->OnTextCommitted.AddDynamic(this, &UFileDialogWidget::OnDirCommited);
	}
	if (FileNameBox)
	{
		FileNameBox->OnTextCommitted.AddDynamic(this, &UFileDialogWidget::OnFileCommited);
	}

	ItemsListView->OnItemSelectionChanged().AddUObject(this, &UFileDialogWidget::HandleItemClicked);	
	ItemsListView->OnItemDoubleClicked().AddUObject(this, &UFileDialogWidget::HandleItemDoubleClicked);

	ResetDialog();
}

void UFileDialogWidget::ResetDialog()
{
	LocationHistory.Empty();
	CurrentLocation.Empty();
	FileNameBox->SetText(FText::GetEmpty());
	DirBox->SetText(FText::GetEmpty());
	SelectedItem = nullptr;
	CurrentIndex = -1;
}

bool UFileDialogWidget::NavigateTo(const FString& InLocation, bool bAdvanceIndex)
{
	IPlatformFile& FileManager = FPlatformFileManager::Get().GetPlatformFile();

	// Clean up path
	FString Location = RemoveTrailingSlashes(InLocation);
	FPaths::MakeStandardFilename(Location);

	if (!FileManager.DirectoryExists(*Location))
	{
		UE_LOG(LogTemp, Error, TEXT("Directory does not exist: %s"), *Location);
		return false;
	}
	SelectedItem = nullptr;

	if (bAdvanceIndex)
	{
		LocationHistory.Add(Location);
		CurrentIndex = LocationHistory.Num() - 1;
	}

	CurrentLocation = Location;
	FString PlatformLocation = CurrentLocation;
	FPaths::MakePlatformFilename(PlatformLocation);;
	DirBox->SetText(FText::FromString(*PlatformLocation));
	ListDirContents();

	return true;
}

void UFileDialogWidget::ListDirContents()
{
	ItemsListView->ClearListItems();
	Items.Empty();

	int32 RowIndex = 0;
	auto CreateItemsForDirectory = [this, &RowIndex](const TCHAR* VisitFilename, bool VisitIsDir)
	{
		// Filter files by type/extension
		bool bFilterPassed = true;
		if (!FileTypeFilter.IsEmpty() && FPaths::FileExists(VisitFilename))
		{
			const FString Ending = FPaths::GetExtension(VisitFilename);
			bFilterPassed = FileTypeFilter.Contains(Ending);
		}

		// Filter out files
		if (FileDialogMode == EFileDialogMode::OpenDirectory && !VisitIsDir)
		{
			bFilterPassed = false;
		}

		if (bFilterPassed)
		{
			UFileDialogItem* Item = NewObject<UFileDialogItem>(this);
			Item->Init(FString(VisitFilename), !VisitIsDir, RowIndex);

			ItemsListView->AddItem(Item);
			RowIndex = RowIndex + 1;
		}
		return true;
	};

	ItemsListView->RegenerateAllEntries();
	IPlatformFile& FileManager = FPlatformFileManager::Get().GetPlatformFile();
	FileManager.IterateDirectory(*CurrentLocation, CreateItemsForDirectory);
}

void UFileDialogWidget::GoPrevDir()
{
	if (LocationHistory.IsValidIndex(CurrentIndex - 1))
	{
		CurrentIndex--;
		NavigateTo(LocationHistory[CurrentIndex], false);
	}
}

void UFileDialogWidget::GoFwdDir()
{
	if (LocationHistory.IsValidIndex(CurrentIndex + 1))
	{
		CurrentIndex++;
		NavigateTo(LocationHistory[CurrentIndex], false);
	}
}

void UFileDialogWidget::GoDirUp()
{
	FString ParentPath = FPaths::GetPath(CurrentLocation);
	if (ParentPath.Len() <= 3) // Only volume letter
	{
		// Try to go to disk root
		FStringView Volume;
		FStringView Temp;
		FPathViews::SplitVolumeSpecifier(ParentPath, Volume, Temp);
		ParentPath = FString::Printf(TEXT("%c:\\"), *Volume.GetData());		
	}
	NavigateTo(ParentPath);
}

void UFileDialogWidget::Refresh()
{
	NavigateTo(CurrentLocation, false);
}

void UFileDialogWidget::HandleConfirm()
{	
	// Get selected item
	if (SelectedItem)
	{
		if (FileDialogMode == EFileDialogMode::OpenDirectory || SelectedItem->IsFile())
		{
			ConfirmFilePath(SelectedItem->GetFullPath());
			return;
		}
		
		// Open dir
		NavigateTo(SelectedItem->GetFullPath());
	}

	// Default 
	if (FileDialogMode == EFileDialogMode::OpenDirectory)
	{
		ConfirmFilePath(CurrentLocation);
	}
}

void UFileDialogWidget::HandleCancel()
{
	OnDialogClosedEvent.Broadcast(true, FString(""));
	OnFileDialogClosed.Broadcast(true, FString(""));
	DeactivateWidget();	
}

void UFileDialogWidget::HandleItemClicked(UObject* Object)
{
	if (Object)
	{
		SelectedItem = Cast<UFileDialogItem>(Object);
		if (SelectedItem->IsFile())
		{
			FileNameBox->SetText(FText::FromString(SelectedItem->GetItemName()));
		}
	}
}

void UFileDialogWidget::HandleItemDoubleClicked(UObject* Object)
{
	UE_LOG(LogTemp, Display, TEXT("Double Clicked"));
	
	UFileDialogItem* Item = Cast<UFileDialogItem>(Object);	
	if (Item->IsFile())
	{
		ConfirmFilePath(Item->GetFullPath());
		return;
	}
	
	// Navigate to dir
	NavigateTo(Item->GetFullPath());
}

void UFileDialogWidget::ConfirmFilePath(const FString& FilePath)
{
	if (FileDialogMode == EFileDialogMode::OpenFile)
	{
		if (FPaths::FileExists(FilePath))
		{
			OnDialogClosedEvent.Broadcast(false, FilePath);
			OnFileDialogClosed.Broadcast(false, FilePath);
			DeactivateWidget();			
			return;
		}
		UE_LOG(LogTemp, Error, TEXT("File does not exist at %s"), *FilePath);
	}
	else if (FileDialogMode == EFileDialogMode::OpenDirectory)
	{
		if (FPaths::DirectoryExists(FilePath))
		{
			OnDialogClosedEvent.Broadcast(false, FilePath);
			OnFileDialogClosed.Broadcast(false, FilePath);
			DeactivateWidget();			
			return;
		}
		UE_LOG(LogTemp, Error, TEXT("Directory does not exist at %s"), *FilePath);
	}
	else 
	{
		// Save file
		// Maybe handle overwrite?
		OnDialogClosedEvent.Broadcast(false, FilePath);
		OnFileDialogClosed.Broadcast(false, FilePath);
		DeactivateWidget();		
	}
}

void UFileDialogWidget::OnDirCommited(const FText& Text, ETextCommit::Type CommitMethod)
{
	if (CommitMethod != ETextCommit::Type::OnCleared)
	{
		FString Path = Text.ToString();
		FPaths::MakeStandardFilename(Path);

		UE_LOG(LogTemp, Display, TEXT("Dir entered %s"), *Path);

		NavigateTo(Path);
	}
}

void UFileDialogWidget::OnFileCommited(const FText& Text, ETextCommit::Type CommitMethod)
{
	FString FileName = Text.ToString();
	FPaths::MakeStandardFilename(FileName);

	// Make sure the file extension is correct
	if (FileDialogMode == EFileDialogMode::SaveFile)
	{
		// Save file
		// Check the file ending
		FString Extension = FPaths::GetExtension(FileName);
		if (Extension.IsEmpty() || !Extension.Equals(FileExtension))
		{
			FPaths::SetExtension(FileName, FileExtension);
			FileNameBox->SetText(FText::FromString(FileName));
		}
	}

	const FString FilePath = FPaths::Combine(CurrentLocation, FileName);
	ConfirmFilePath(FilePath);
}

void UFileDialogWidget::InitText(const FText& InTitle, const FText& ConfirmText, const FText& CancelText)
{
	if (ConfirmBtn)
	{
		ConfirmBtn->SetButtonText(ConfirmText);
	}
	if (CancelBtn)
	{
		CancelBtn->SetButtonText(CancelText);
	}
	TitleText->SetText(InTitle);
}

FString UFileDialogWidget::RemoveTrailingSlashes(const FString& Path)
{
	int32 EndIndex = Path.Len() - 1;
	while (EndIndex > 3) // Do not remove from c:/
	{
		TCHAR LastChar = Path[EndIndex];
		if (LastChar == TEXT('/') || LastChar == TEXT('\\'))
		{
			EndIndex--;
		}
		else
		{
			break;
		}
	}
	return Path.Left(EndIndex + 1);
}

bool UFileDialogWidget::SetAsOpenFileDialog(const FText& InTitle, const FText& ConfirmText, const FText& CancelText,
                                            const FString& StartLocation, const TArray<FString>& InFileExtensionFilter)
{
	FileTypeFilter = InFileExtensionFilter;
	for (FString TypeFilter : FileTypeFilter)
	{
		if (TypeFilter.IsEmpty())
		{
			UE_LOG(LogTemp, Error, TEXT("FilterTypes contains empty string!"))
			return false;
		}
	}
	InitText(InTitle, ConfirmText, CancelText);
	FileDialogMode = EFileDialogMode::OpenFile;
	FileNameBox->SetIsEnabled(true);
	ResetDialog();
	ActivateWidget();

	NavigateTo(StartLocation);
	return true;
}

bool UFileDialogWidget::SetAsOpenDirectoryDialog(const FText& InTitle, const FText& ConfirmText,
	const FText& CancelText, const FString& StartLocation)
{	
	InitText(InTitle, ConfirmText, CancelText);
	FileDialogMode = EFileDialogMode::OpenDirectory;
	FileNameBox->SetIsEnabled(false);
	ResetDialog();
	ActivateWidget();

	NavigateTo(StartLocation);
	return true;
}

bool UFileDialogWidget::SetAsSaveFileDialog(const FText& InTitle, const FText& ConfirmText, const FText& CancelText,
                                            const FString& StartLocation, const FString& FileNameSuggestion,
                                            const FString& InFileExtension)
{
	FileExtension = InFileExtension;
	if (FileExtension.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("Extension string is empty!"))
		return false;
	}
	InitText(InTitle, ConfirmText, CancelText);
	FileNameBox->SetText(FText::FromString(FPaths::SetExtension(FileNameSuggestion, InFileExtension)));
	FileDialogMode = EFileDialogMode::SaveFile;
	FileNameBox->SetIsEnabled(true);
	ResetDialog();
	ActivateWidget();

	NavigateTo(StartLocation);
	return true;
}

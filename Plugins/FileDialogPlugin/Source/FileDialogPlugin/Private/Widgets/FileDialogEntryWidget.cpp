// Copyright Kevin Lichtenberg. All Rights Reserved. kevlicht@gmail.com


#include "Widgets/FileDialogEntryWidget.h"

#include "CommonTextBlock.h"
#include "Components/Border.h"
#include "Components/Image.h"


void UFileDialogEntryWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	SetVisibility(ESlateVisibility::Visible);
	if (EntryBorder)
	{			
		EntryBorder->SetBrush(EvenRowStyle);
	}
}

void UFileDialogEntryWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (IconImage)
	{
		IconSize = IconImage->GetBrush().GetImageSize();		
	}
}

void UFileDialogEntryWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);

	if (EntryBorder)
	{
		EntryBorder->SetBrush(bIsSelected ? SelectedHoveredStyle : HoveredStyle);		
	}
}

void UFileDialogEntryWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);

	UpdateBorder();
}

void UFileDialogEntryWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);

	Entry = Cast<UFileDialogItem>(ListItemObject);
	checkf(Entry, TEXT("Fatal Error"));

	bIsSelected = false;
	if (EntryText)
	{
		EntryText->SetText(FText::FromString(Entry->GetItemName()));		
	}
	UpdateIcon();	
	UpdateBorder();
}

void UFileDialogEntryWidget::NativeOnItemSelectionChanged(bool bInIsSelected)
{
	bIsSelected = bInIsSelected;
	UpdateBorder();	
}

void UFileDialogEntryWidget::UpdateBorder()
{
	if (!EntryBorder)
	{
		return;
	}
	
	if (bIsSelected)
	{
		EntryBorder->SetBrush(SelectedBrush);
		return;
	}
	
	if (Entry && Entry->GetRowIndex() % 2 == 0)
	{
		EntryBorder->SetBrush(EvenRowStyle);
	}
	else
	{
		EntryBorder->SetBrush(UnevenRowStyle);
	}
}

void UFileDialogEntryWidget::UpdateIcon()
{
	if (!IconImage)
	{
		return;
	}
	if (Entry && Entry->IsFile())
	{
		if (FileTypeMapping.Contains(Entry->GetFileType()))
		{
			IconImage->SetBrush(FileTypeMapping[Entry->GetFileType()]);
		} else
		{
			IconImage->SetBrush(DefaultFileBrush);
		}
	} else
	{
		IconImage->SetBrush(DirectoryBrush);
	}
	IconImage->SetDesiredSizeOverride(IconSize);
}

// ==================================================================================

FString UFileDialogItem::GetDirectoryName(const FString& Path)
{	
	int32 LastSlashIndex;

	if (Path.FindLastChar(TEXT('/'), LastSlashIndex) || Path.FindLastChar(TEXT('\\'), LastSlashIndex))
	{
		return Path.Mid(LastSlashIndex + 1);
	}

	return Path; // If no separator is found, return the whole path
}

void UFileDialogItem::Init(const FString& InFullPath, bool bInIsFile, int32 InRowIndex)
{	
	bIsFile = bInIsFile;
	FullPath = InFullPath;
	RowIndex = InRowIndex;

	FPaths::MakeStandardFilename(FullPath);

	//UE_LOG(LogTemp, Display, TEXT("FullPath: %s"), *FullPath);

	if (bIsFile)
	{
		FString ParentPath;		
		FPaths::Split(FullPath, ParentPath, ItemName, FileType);
		
		//UE_LOG(LogTemp, Display, TEXT("FileType: %s"), *FileType);
	} else
	{
		// Get Directory name		
		ItemName = GetDirectoryName(FullPath);		
	}	
	
}

FString UFileDialogItem::GetItemName(bool bIncludeType)
{
	if (bIsFile)
	{
		return bIncludeType ? FString(ItemName).Append(".").Append(FileType) : ItemName;
	}
	return ItemName;
}

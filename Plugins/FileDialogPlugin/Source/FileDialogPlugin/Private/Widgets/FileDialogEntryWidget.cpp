// Copyright Kevin Lichtenberg. All Rights Reserved. kevlicht@gmail.com


#include "Widgets/FileDialogEntryWidget.h"

#include "Misc/Paths.h"
#include "CommonTextBlock.h"
#include "Components/Border.h"
#include "Components/Image.h"
#include "Utils/FileDialogUtils.h"
#include "Widgets/IconMappingStyle.h"


void UFileDialogEntryWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	SetVisibility(ESlateVisibility::Visible);
	if (EntryBorder)
	{			
		EntryBorder->SetBrush(EvenRowStyle);
	}
	UpdateIcon();
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

	ItemType = Entry->GetFileType();
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
	const UIconMappingStyle* Style = GetIconMapping();
	if (Style && Style->IconMappings.Contains(ItemType))
	{
		IconImage->SetBrush(Style->IconMappings[ItemType]);
	}
	else
	{
		IconImage->SetBrush(DefaultFileBrush);
	}
	
	IconImage->SetDesiredSizeOverride(IconSize);
}

const UIconMappingStyle* UFileDialogEntryWidget::GetIconMapping()
{
	if (IconMappingStyle)
	{
		if (const UIconMappingStyle* MappingStyle = Cast<UIconMappingStyle>(IconMappingStyle->GetDefaultObject(false)))
		{
			return MappingStyle;
		}
	}
	return nullptr;
}

// ==================================================================================

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
		FPaths::Split(FullPath, ParentPath, ItemNameStr, FileType);
		
		//UE_LOG(LogTemp, Display, TEXT("FileType: %s"), *FileType);
	} else
	{
		// Get Directory name		
		ItemNameStr = UFileDialogUtils::GetDirectoryName(FullPath);
		FileType = TEXT("#dir");
	}	
	ItemName = FText::FromString(ItemNameStr);
	
}

void UFileDialogItem::InitShortcut(const FText& InName, const FString& InFullPath, const FString& InFileType)
{
	ItemName = InName;
	ItemNameStr = InName.ToString();
	FullPath = InFullPath;
	FileType = InFileType;
	RowIndex = 0;
	bIsFile = false;
}

FString UFileDialogItem::GetItemName(bool bIncludeType)
{
	if (bIsFile)
	{
		return bIncludeType ? FString(ItemNameStr).Append(".").Append(FileType) : ItemNameStr;
	}
	return ItemNameStr;
}

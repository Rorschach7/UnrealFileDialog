// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/FileDialogShortcutEntryWidget.h"

#include "CommonTextBlock.h"
#include "Components/Border.h"
#include "Components/Image.h"
#include "Widgets/IconMappingStyle.h"

void UFileDialogShortcutEntryWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	SetVisibility(ESlateVisibility::Visible);
	if (EntryBorder)
	{			
		EntryBorder->SetBrush(RowStyle);
	}
	if (IconImage)
	{
		UpdateIcon();
	}
}

void UFileDialogShortcutEntryWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	if (IconImage)
	{
		IconSize = IconImage->GetBrush().GetImageSize();		
	}
}

void UFileDialogShortcutEntryWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);

	if (EntryBorder)
	{
		EntryBorder->SetBrush(bIsSelected ? SelectedHoveredStyle : HoveredStyle);		
	}
}

void UFileDialogShortcutEntryWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
	UpdateBorder();
}

void UFileDialogShortcutEntryWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	Entry = Cast<UFileDialogShortcutItem>(ListItemObject);
	checkf(Entry, TEXT("Fatal Error"));

	bIsSelected = false;
	ItemType = Entry->GetFileType();
	if (EntryText)
	{
		EntryText->SetText((Entry->GetItemName(false)));		
	}
	UpdateIcon();	
	UpdateBorder();
}

void UFileDialogShortcutEntryWidget::NativeOnItemSelectionChanged(bool bInIsSelected)
{
	bIsSelected = bInIsSelected;
	UpdateBorder();
}

void UFileDialogShortcutEntryWidget::UpdateBorder()
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

	EntryBorder->SetBrush(RowStyle);
}

void UFileDialogShortcutEntryWidget::UpdateIcon()
{
	if (!IconImage)
	{
		return;
	}
	const UIconMappingStyle* Style = GetIconMapping();
	if (Style)
	{
		if (Style->IconMappings.Contains(ItemType))
		{
			IconImage->SetBrush(Style->IconMappings[ItemType]);
		}
	}
	
	IconImage->SetDesiredSizeOverride(IconSize);
}

const UIconMappingStyle* UFileDialogShortcutEntryWidget::GetIconMapping()
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

void UFileDialogShortcutEntryWidget::SetIconType(const FName& InIconType)
{
	
}

void UFileDialogShortcutItem::InitShortcut(const FText& InName, const FString& InFullPath, const FString& InFileType)
{
	ItemName = InName;
	FullPath = InFullPath;
	FileType = InFileType;
}

FText UFileDialogShortcutItem::GetItemName(bool bIncludeType)
{
	if (bIncludeType)
	{
		
	}
	
	return ItemName;
}

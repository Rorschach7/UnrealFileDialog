// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/FileDialogShortcutWidget.h"
#include "CommonTextBlock.h"
#include "Components/ListView.h"
#include "Widgets/FileDialogEntryWidget.h"

void UFileDialogShortcutWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (TitleText)
	{
		TitleText->SetText(Title);
	}
}

void UFileDialogShortcutWidget::AddEntry(const FText& Text, const FString& Location, const FString& Type)
{
	if (ShortcutList)
	{
		UFileDialogItem* Item = NewObject<UFileDialogItem>(this);
		Item->InitShortcut(Text, Location, Type);
		ShortcutList->AddItem(Item);
	}
}

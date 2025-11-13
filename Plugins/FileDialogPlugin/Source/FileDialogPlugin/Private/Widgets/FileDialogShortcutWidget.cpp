// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/FileDialogShortcutWidget.h"
#include "CommonTextBlock.h"
#include "Components/ListView.h"
#include "Widgets/FileDialogShortcutEntryWidget.h"

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
	UFileDialogShortcutItem* Item = NewObject<UFileDialogShortcutItem>(this);
	Item->InitShortcut(Text, Location, Type);
	if (ShortcutList)
	{
		ShortcutList->AddItem(Item);
	}
}

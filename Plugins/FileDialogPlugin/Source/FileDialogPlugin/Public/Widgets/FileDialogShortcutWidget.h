// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "FileDialogShortcutWidget.generated.h"

class UCommonTextBlock;
class UListView;
/**
 * 
 */
UCLASS()
class FILEDIALOGPLUGIN_API UFileDialogShortcutWidget : public UCommonUserWidget
{
	GENERATED_BODY()

protected:

	UPROPERTY(meta = (BindWidget, OptionalWidget))
	UCommonTextBlock* TitleText;
	UPROPERTY(meta = (BindWidget, OptionalWidget))
	UListView* ShortcutList;

	UPROPERTY(EditAnywhere, Category="File Dialog Shortcut")
	FText Title;

	virtual void NativePreConstruct() override;

public:
	virtual void AddEntry(const FText& Text,const FString& Location,const FString& Type);
};

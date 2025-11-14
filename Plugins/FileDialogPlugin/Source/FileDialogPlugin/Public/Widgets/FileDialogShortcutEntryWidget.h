// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "Blueprint/UserWidget.h"
#include "FileDialogShortcutEntryWidget.generated.h"

class UFileDialogShortcutItem;
class UIconMappingStyle;
class UCommonTextBlock;
class UImage;
class UBorder;
/**
 * 
 */
UCLASS()
class FILEDIALOGPLUGIN_API UFileDialogShortcutEntryWidget : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

protected:

	UPROPERTY(meta = (BindWidgetOptional))
	UBorder* EntryBorder;
	UPROPERTY(meta = (BindWidgetOptional))
	UImage* IconImage;
	UPROPERTY(meta = (BindWidgetOptional))
	UCommonTextBlock* EntryText;

	UPROPERTY(EditAnywhere, Category="Styling")
	TSubclassOf<UIconMappingStyle> IconMappingStyle;
	UPROPERTY(EditAnywhere, Category="Styling")
	FSlateBrush RowStyle;	
	UPROPERTY(EditAnywhere, Category="Styling")
	FSlateBrush HoveredStyle;
	UPROPERTY(EditAnywhere, Category="Styling")
	FSlateBrush SelectedBrush;
	UPROPERTY(EditAnywhere, Category="Styling")
	FSlateBrush SelectedHoveredStyle;
	
	/**
	 * The type of the item as a string. In the case of a file, this is the file extension.
	 * In the case of a directory this is a #dir or a more specialized type that can display a special icon 
	 */
	UPROPERTY(EditAnywhere, Category="Styling")
	FString ItemType;

	UFileDialogShortcutItem* Entry = nullptr;
	bool bIsSelected = false;
	FVector2D IconSize;

	virtual void NativePreConstruct() override;
	virtual void NativeOnInitialized() override;
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;	
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	virtual void NativeOnItemSelectionChanged(bool bInIsSelected) override;

	virtual void UpdateBorder();
	virtual void UpdateIcon();		

	virtual const UIconMappingStyle* GetIconMapping();

public:

	virtual void SetIconType(const FName& InIconType);
};

UCLASS()
class FILEDIALOGPLUGIN_API UFileDialogShortcutItem : public UObject
{
	GENERATED_BODY()

protected:
	FText ItemName;
	FString FileType = FString();
	FString FullPath;

public:

	virtual void InitShortcut(const FText& InName, const FString& InFullPath, const FString& InFileType);

	virtual FText GetItemName(bool bIncludeType = true);

	virtual FString GetFileType() const { return FileType; }
	virtual FString GetFullPath() const { return FullPath; }
};


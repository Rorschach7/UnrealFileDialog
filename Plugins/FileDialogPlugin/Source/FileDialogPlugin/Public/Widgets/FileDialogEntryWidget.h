// Copyright Kevin Lichtenberg. All Rights Reserved. kevlicht@gmail.com

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "Blueprint/UserWidget.h"
#include "FileDialogEntryWidget.generated.h"

class UImage;
class UFileDialogItem;
class UCommonTextBlock;
class UBorder;
/**
 * 
 */
UCLASS()
class FILEDIALOGPLUGIN_API UFileDialogEntryWidget : public UUserWidget, public IUserObjectListEntry
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
	FSlateBrush EvenRowStyle;
	UPROPERTY(EditAnywhere, Category="Styling")
	FSlateBrush UnevenRowStyle;
	UPROPERTY(EditAnywhere, Category="Styling")
	FSlateBrush HoveredStyle;
	UPROPERTY(EditAnywhere, Category="Styling")
	FSlateBrush SelectedBrush;
	UPROPERTY(EditAnywhere, Category="Styling")
	FSlateBrush SelectedHoveredStyle;
	UPROPERTY(EditAnywhere, Category="Styling")
	FSlateBrush DefaultFileBrush;
	UPROPERTY(EditAnywhere, Category="Styling")
	FSlateBrush DirectoryBrush;

	bool bIsSelected = false;

	/**
	 * Used to map special icons to specific file types.
	 * E.g. png, jpg
	 */
	UPROPERTY(EditAnywhere, Category="Styling")
	TMap<FString,FSlateBrush> FileTypeMapping;	

	FVector2D IconSize;
	UFileDialogItem* Entry = nullptr;	

	virtual void NativePreConstruct() override;
	virtual void NativeOnInitialized() override;
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;	
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	virtual void NativeOnItemSelectionChanged(bool bInIsSelected) override;

	virtual void UpdateBorder();
	virtual void UpdateIcon();		
};

// ==================================================================================

UCLASS()
class FILEDIALOGPLUGIN_API UFileDialogItem : public UObject
{
	GENERATED_BODY()

protected:
	bool bIsFile;
	int32 RowIndex;
	FString ItemName;
	FString FileType = FString();
	FString FullPath;

	FString GetDirectoryName(const FString& Path);

public:

	virtual void Init(const FString& InFullPath, bool bInIsFile, int32 InRowIndex);

	virtual FString GetItemName(bool bIncludeType = true);

	virtual bool IsFile() const { return bIsFile; }
	virtual FString GetFileType() const { return FileType; }
	virtual FString GetFullPath() const { return FullPath; }
	virtual int32 GetRowIndex() const { return RowIndex; }
};

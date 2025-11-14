// Copyright Kevin Lichtenberg. All Rights Reserved. kevlicht@gmail.com

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "Blueprint/UserWidget.h"
#include "FileDialogEntryWidget.generated.h"

class UIconMappingStyle;
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
	TSubclassOf<UIconMappingStyle> IconMappingStyle;
	/**
	 * The type of the item as a string. In the case of a file, this is the file extension.
	 * In the case of a directory this is a #dir or a more specialized type that can display a special icon 
	 */
	UPROPERTY(EditAnywhere, Category="Styling")
	FString ItemType;

	bool bIsSelected = false;
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
	
	virtual const UIconMappingStyle* GetIconMapping();	

};

// ==================================================================================

UCLASS()
class FILEDIALOGPLUGIN_API UFileDialogItem : public UObject
{
	GENERATED_BODY()

protected:
	bool bIsFile;
	int32 RowIndex;
	FText ItemName;
	FString ItemNameStr;
	FString FileType = FString();
	FString FullPath;

public:

	virtual void Init(const FString& InFullPath, bool bInIsFile, int32 InRowIndex);
	virtual void InitShortcut(const FText& InName, const FString& InFullPath, const FString& InFileType);

	virtual FString GetItemName(bool bIncludeType = true);

	virtual bool IsFile() const { return bIsFile; }
	virtual FString GetFileType() const { return FileType; }
	virtual FString GetFullPath() const { return FullPath; }
	virtual int32 GetRowIndex() const { return RowIndex; }
};

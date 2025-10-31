// Copyright Kevin Lichtenberg. All Rights Reserved. kevlicht@gmail.com

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "CommonListView.h"
#include "FileDialogWidget.generated.h"

class UCommonBorder;
class UFileDialogButton;
class UCommonButtonBase;
class UEditableTextBox;
class UFileDialogItem;
class UCommonTextBlock;
class UBorder;

DECLARE_EVENT_TwoParams(UFileDialogWidget, FFileDialogDelegate, bool, FString);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FFileDialogClosed, bool, bCancelled, FString, Path);

enum class EFileDialogMode
{
	OpenFile, OpenDirectory, SaveFile
};

/**
 * 
 */
UCLASS()
class FILEDIALOGPLUGIN_API UFileDialogWidget : public UCommonActivatableWidget
{
	GENERATED_BODY()

protected:

	UPROPERTY(meta = (BindWidget, OptionalWidget))
	UCommonTextBlock* TitleText;
	UPROPERTY(meta = (BindWidget, OptionalWidget))
	UCommonButtonBase* DirBackBtn;
	UPROPERTY(meta = (BindWidget, OptionalWidget))
	UCommonButtonBase* DirFwdBtn;
	UPROPERTY(meta = (BindWidget, OptionalWidget))
	UCommonButtonBase* DirUpBtn;
	UPROPERTY(meta = (BindWidget, OptionalWidget))
	UCommonButtonBase* RefreshBtn;
	UPROPERTY(meta = (BindWidget))
	UCommonListView* ItemsListView;
	UPROPERTY(meta = (BindWidget, OptionalWidget))
	UEditableTextBox* DirBox;
	UPROPERTY(meta = (BindWidget, OptionalWidget))
	UEditableTextBox* FileNameBox;
	UPROPERTY(meta = (BindWidget, OptionalWidget))
	UFileDialogButton* ConfirmBtn;
	UPROPERTY(meta = (BindWidget, OptionalWidget))
	UFileDialogButton* CancelBtn;
	UPROPERTY(meta = (BindWidget, OptionalWidget))
	UCommonBorder* QuickAccessPanel;

	UPROPERTY()
	TArray<UFileDialogItem*> Items;
	TObjectPtr<UFileDialogItem> SelectedItem = nullptr;
	
	mutable FFileDialogDelegate OnDialogClosedEvent;

	EFileDialogMode FileDialogMode;
	int32 CurrentIndex;
	FString CurrentLocation;
	TArray<FString> LocationHistory;

	FString FileExtension;
	TArray<FString> FileTypeFilter;

	virtual void NativeOnInitialized() override;	

	virtual void ResetDialog();
	virtual bool NavigateTo(const FString& InLocation, bool bAdvanceIndex = true);
	virtual void ListDirContents();

	virtual void GoPrevDir();
	virtual void GoFwdDir();
	virtual void GoDirUp();
	virtual void Refresh();
	virtual void HandleConfirm();
	virtual void HandleCancel();
	void HandleItemClicked(UObject* Object);
	void HandleItemDoubleClicked(UObject* Object);

	virtual void ConfirmFilePath(const FString& FilePath);
	
	UFUNCTION()
	virtual void OnDirCommited(const FText& Text, ETextCommit::Type CommitMethod);
	UFUNCTION()
	virtual void OnFileCommited(const FText& Text, ETextCommit::Type CommitMethod);
	void InitText(const FText& InTitle, const FText& ConfirmText, const FText& CancelText);

	static FString RemoveTrailingSlashes(const FString& Path);

public:

	UPROPERTY(BlueprintAssignable, Category = "Events", meta = (AllowPrivateAccess = true))
	FFileDialogClosed OnFileDialogClosed;

	FFileDialogDelegate& OnDialogClosed() const { return OnDialogClosedEvent; }	

	/**
	 * 
	 * @param InTitle
	 * @param ConfirmText
	 * @param CancelText 
	 * @param StartLocation
	 * @param InFileExtensionFilter 
	 * @return 
	 */
	UFUNCTION(BlueprintCallable, Category = "FileDialog", meta=(AutoCreateRefTerm="InFileExtensionFilter"))
	virtual bool SetAsOpenFileDialog(const FText& InTitle, const FText& ConfirmText, const FText& CancelText,
	                                 const FString& StartLocation, const TArray<
		                                 FString>& InFileExtensionFilter);

	/**
	 * 
	 * @param InTitle
	 * @param ConfirmText
	 * @param CancelText 
	 * @param StartLocation
	 * @return 
	 */
	UFUNCTION(BlueprintCallable, Category = "FileDialog", meta=(AutoCreateRefTerm="InFileExtensionFilter"))
	virtual bool SetAsOpenDirectoryDialog(const FText& InTitle, const FText& ConfirmText, const FText& CancelText,
	                                      const FString& StartLocation);
	
	/**
	 * 
	 * @param InTitle
	 * @param ConfirmText
	 * @param CancelText 
	 * @param StartLocation
	 * @param FileNameSuggestion
	 * @param InFileExtension 
	 * @return 
	 */
	UFUNCTION(BlueprintCallable, Category = "FileDialog")
	virtual bool SetAsSaveFileDialog(const FText& InTitle, const FText& ConfirmText, const FText& CancelText, const FString& StartLocation, const FString&
	                                 FileNameSuggestion, const FString& InFileExtension);
	
};

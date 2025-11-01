// Copyright Kevin Lichtenberg. All Rights Reserved. kevlicht@gmail.com

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "Widgets/FileDialogWidget.h"
#include "OpenFileDialogAsyncAction.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FFileDialogAsyncActionDelegate,
	bool, bCancelled,
	FString, FilePath
);

/**
 * 
 */
UCLASS()
class FILEDIALOGPLUGIN_API UOpenFileDialogAsyncAction : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:

	/**
	 * Sets the file dialog widget to open a file and provides an async action for when the file was selected by the user.
	 * @param WorldContextObject 
	 * @param FileDialogWidget The FileDialogWidget that is used for the operation
	 * @param WindowTitle The title for the OpenFileDialog
	 * @param ConfirmText The text on the confirm button
	 * @param CancelText The text on the cancel button
	 * @param StartLocation The path the file dialog should open at
	 * @param FileExtensions Only the given file extensions are being displayed. If none are given all files will be displayed.
	 * @return 
	 */
	UFUNCTION(BlueprintCallable, meta=(WorldContext = "WorldContextObject", AutoCreateRefTerm="FileExtensions", BlueprintInternalUseOnly=true))
	static UOpenFileDialogAsyncAction* ShowOpenFileDialogAsync(
		UObject* WorldContextObject, UFileDialogWidget* FileDialogWidget, const FText& WindowTitle, const FText& ConfirmText, const
		FText& CancelText, const FString& StartLocation, TArray<FString> FileExtensions);

	/**
	 * Sets the file dialog widget to open a file and provides an async action for when the file was selected by the user.
	 * @param WorldContextObject 
	 * @param FileDialogWidget The FileDialogWidget that is used for the operation
	 * @param WindowTitle The title for the OpenFileDialog
	 * @param ConfirmText The text on the confirm button
	 * @param CancelText The text on the cancel button
	 * @param StartLocation The path the file dialog should open at
	 * @param FileExtensions Only the given file extensions are being displayed. If none are given all files will be displayed.
	 * @return 
	 */
	UFUNCTION(BlueprintCallable, meta=(WorldContext = "WorldContextObject", AutoCreateRefTerm="FileExtensions", BlueprintInternalUseOnly=true))
	static UOpenFileDialogAsyncAction* ShowOpenDirectoryDialogAsync(
		UObject* WorldContextObject, UFileDialogWidget* FileDialogWidget, const FText& WindowTitle, const FText& ConfirmText, const
		FText& CancelText, const FString& StartLocation);

	/**
	 * Sets the file dialog widget to save a file and provides an async action for when the file was selected by the user.
	 * @param WorldContextObject 
	* @param FileDialogWidget The FileDialogWidget that is used for the operation
	 * @param WindowTitle The title for the OpenFileDialog
	 * @param ConfirmText The text on the confirm button
	 * @param CancelText The text on the cancel button
	 * @param StartLocation The path the file dialog should open at 
	 * @param SuggestedFileName The file name that will be used be default
	 * @param InFileExtension The file extension that is enforced for the saved file
	 * @return 
	 */
	UFUNCTION(BlueprintCallable, meta=(WorldContext = "WorldContextObject", AutoCreateRefTerm="FileExtensions", BlueprintInternalUseOnly=true))
	static UOpenFileDialogAsyncAction* ShowSaveFileDialogAsync(
		UObject* WorldContextObject, UFileDialogWidget* FileDialogWidget, const FText& WindowTitle, const FText& ConfirmText, const
		FText& CancelText, const FString& StartLocation, const FString& SuggestedFileName, const FString& InFileExtension);
	
	UPROPERTY(BlueprintAssignable)
	FFileDialogAsyncActionDelegate OnFileDialogCompleted;

protected:
	UWorld* WorldContext = nullptr;
	UFileDialogWidget* DialogWidget = nullptr;

	virtual void Activate() override;	
	
	virtual void HandleFileDialogCompleted(bool bCancelled, FString FilePath);
	
};

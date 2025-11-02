# UnrealFileDialog

![thumbnail](/assets/fileDialog.jpg)

A simple plugin for a runtime file dialog made with UMG. It is entirely written in C++ while offering full blueprint compatibility.

## What it offers

A file dialog widget that supports three modes:
- Open file
- Open directory
- Save file

Thanks to the CommonUI plugin, it is easily stylable via Blueprints.
There is also an async blueprint node for easier callback handling.

## How to install

### Compile plugin from source
Clone or download this repository and navigate to the "Plugins" folder. There is a folder called "FileDialogPlugin".
Copy the entire folder into your project's "Plugins" folder, regenerate your project files, and build your solution.

### Use binaries
If you have a pure blueprint project and can't compile the source files, you can use the binaries.
Download a release that fits your Unreal Engine version and unpack the zip file. Copy the unzipped folder into your project's "Plugins" folder.
You might need to create the "Plugins" folder if it does not exist already.

## How to use

Before you can use the widget, you always need to **initialize** the widget with the corresponding function:
- SetAsOpenFileDialog
- SetAsOpenDirectoryDialog
- SetAsSaveFileDialog

### Blueprints
First, you need to have a live instance of the UFileDialogWidget class.
Then, you can just use one of the following blueprint nodes:

![thumbnail](/assets/fileDialogBlueprintNodes.jpg)

### General Dialog Inputs (Used by all three functions)
| Parameter      | Description                                                                                                                                          |
|----------------|------------------------------------------------------------------------------------------------------------------------------------------------------|
| Window Title   | The text displayed at the top of the dialog box (e.g., "Select a File to Open" or "Save Your Project"). It tells the user the purpose of the window. |
| Confirm Text   | The label for the confirmation button (e.g., "Open", "Save", or "OK").                                                                               |
| Cancel Text    | The label for the cancellation button (e.g., "Cancel").                                                                                              |
| Start Location | The initial directory that the dialog will open to, providing a starting point for the user's file navigation.                                       |


### File Open Dialog Specific Inputs
| Parameter             | Description                                                                                                                                                                      |
|-----------------------|----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| File Extension Array  | A list of file extensions (e.g., ["txt", "log"]). This filters the files displayed in the dialog, ensuring the user can only select files with one of the specified extensions.  |

### Save File Dialog Specific Inputs
| Parameter         | Description                                                                                                                            |
|-------------------|----------------------------------------------------------------------------------------------------------------------------------------|
| SuggestedFileName | The default file name that is pre-filled in the dialog's file name field (e.g., NewDocument).                                          |
| InFileExtension   | The specific file extension that should be used when saving the file (e.g., json or csv). This is typically applied to the file name.  |

> [!WARNING]
> Do not use '.' when listing file extensions. That is handled by the file dialog.
 
### C++

The function's signatures are identical to the blueprint nodes. You just have to handle the callback when the file dialog closes itself:

```cpp

// ============================ Functions you need to initialize the widget depending on your goal =====================

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
	                                 const FString& StartLocation, 
	                                 const TArray<FString>& InFileExtensionFilter);

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
	virtual bool SetAsSaveFileDialog(const FText& InTitle, const FText& ConfirmText, const FText& CancelText,
	                                const FString& StartLocation, const FString&
	                                FileNameSuggestion, const FString& InFileExtension);

// ======================================== Example usage ==============================================================

void ADialogTestActor::OpenFileDialog()
{

	UFileDialogWidget* DialogWidget = CreateWidget<UFileDialogWidget>(GetWorld(), DialogWidgetClass);
	DialogWidget->AddToViewport();

	TArray<FString> ExtensionFilter;
	ExtensionFilter.Add("png");
	ExtensionFilter.Add("jpg");	

	// This call is important!
	DialogWidget->SetAsOpenFileDialog(
		FText::FromString(TEXT("Open File")),
		FText::FromString(TEXT("Open")),
		FText::FromString(TEXT("Cancel")),
		FPlatformProcess::UserHomeDir(),
		ExtensionFilter);
	
	// Handle callback
	DialogWidget->OnDialogClosed().AddLambda([this, DialogWidget](bool bCancelled, const FString& FilePath)
	{
		DialogWidget->RemoveFromParent();	
		if (bCancelled)
		{
			UE_LOG(LogTemp, Display, TEXT("Cancelled"));
			return;
		}
		UE_LOG(LogTemp, Display, TEXT("Selected Path %s"), *FilePath);		
	});
}
```

> [!NOTE]
> This project also contains a C++ and a blueprint example.

## What's next?

- extension selection via combo box for file saving dialog
- ability to create a new directory via the file dialog widget

 

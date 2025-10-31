// Copyright Kevin Lichtenberg. All Rights Reserved. kevlicht@gmail.com

#pragma once

#include "CoreMinimal.h"
#include "CommonButtonBase.h"
#include "FileDialogButton.generated.h"

class UCommonTextBlock;
/**
 * 
 */
UCLASS()
class FILEDIALOGPLUGIN_API UFileDialogButton : public UCommonButtonBase
{
	GENERATED_BODY()

protected:
	
	UPROPERTY(meta=(BindWidgetOptional))
	UCommonTextBlock* ButtonTextBlock;

	UPROPERTY(EditAnywhere, Category="File Dialog Button")
	FText ButtonText;

	virtual void NativePreConstruct() override;

public:

	virtual void SetButtonText(const FText& Text);
};

// Copyright Kevin Lichtenberg. All Rights Reserved. kevlicht@gmail.com


#include "Widgets/FileDialogButton.h"

#include "CommonTextBlock.h"

void UFileDialogButton::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (ButtonTextBlock)
	{
		SetButtonText(ButtonText);
	}
}

void UFileDialogButton::SetButtonText(const FText& Text)
{
	ButtonText = Text;
	ButtonTextBlock->SetText(ButtonText);
}

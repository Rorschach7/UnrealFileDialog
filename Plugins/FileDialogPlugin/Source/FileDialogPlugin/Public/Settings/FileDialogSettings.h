// Copyright Kevin Lichtenberg. All Rights Reserved. kevlicht@gmail.com

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "FileDialogSettings.generated.h"

/**
 * 
 */
UCLASS(config=Plugin, defaultconfig, meta = (DisplayName="File Dialog"))
class FILEDIALOGPLUGIN_API UFileDialogSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Tile Map Editor")
	TSoftClassPtr<UUserWidget> FileDialogWidgetClass;
};

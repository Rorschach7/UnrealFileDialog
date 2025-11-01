// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogTestActor.h"

#include "Blueprint/UserWidget.h"
#include "Widgets/FileDialogWidget.h"


// Sets default values
ADialogTestActor::ADialogTestActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ADialogTestActor::BeginPlay()
{
	Super::BeginPlay();

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

// Called every frame
void ADialogTestActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


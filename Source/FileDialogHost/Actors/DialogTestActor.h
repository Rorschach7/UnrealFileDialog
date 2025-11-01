// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DialogTestActor.generated.h"

class UFileDialogWidget;

UCLASS()
class FILEDIALOGHOST_API ADialogTestActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADialogTestActor();

protected:

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UFileDialogWidget> DialogWidgetClass;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};

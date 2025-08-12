// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MultiplayerHandlerMenuWidget.generated.h"

class UButton;
/**
 * 
 */
UCLASS()
class MULTIPLAYERHANDLER_API UMultiplayerHandlerMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void Setup();


};

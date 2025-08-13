// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MultiplayerHandlerMenuWidget.generated.h"

class UMultiplayerHandlerSubsystem;
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
	void SetupMenu(int32 NumberOfPublicConnections = 4, FString TypeOfMatch = "FreeForAll");

protected:
	virtual bool Initialize() override;
	virtual void NativeDestruct() override;

private:
	UFUNCTION()
	void RemoveMenu();
	
	UPROPERTY(meta=(BindWidget))
	UButton* Button_Host;

	UPROPERTY(meta=(BindWidget))
	UButton* Button_Join;

	UFUNCTION()
	void HostButtonClicked();

	UFUNCTION()
	void JoinButtonClicked();

	UPROPERTY()
	UMultiplayerHandlerSubsystem* MultiplayerHandlerSubsystem;

	int32 NumPublicConnections;
	FString MatchType;
};

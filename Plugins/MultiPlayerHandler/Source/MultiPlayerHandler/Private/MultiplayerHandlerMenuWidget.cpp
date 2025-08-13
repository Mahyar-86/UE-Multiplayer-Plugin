// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiplayerHandlerMenuWidget.h"

#include "MultiplayerHandlerSubsystem.h"
#include "Components/Button.h"

void UMultiplayerHandlerMenuWidget::SetupMenu(int32 NumberOfPublicConnections, FString TypeOfMatch)
{
	NumPublicConnections = NumberOfPublicConnections;
	MatchType = TypeOfMatch;
	
	AddToViewport();
	SetVisibility(ESlateVisibility::Visible);
	SetIsFocusable(true);

	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		FInputModeUIOnly InputModeData;
		InputModeData.SetWidgetToFocus(TakeWidget());
		InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		PlayerController->SetInputMode(InputModeData);
		PlayerController->bShowMouseCursor = true;
	}

	if (const UGameInstance* GameInstance = GetGameInstance())
	{
		MultiplayerHandlerSubsystem = GameInstance->GetSubsystem<UMultiplayerHandlerSubsystem>();
	}
}

bool UMultiplayerHandlerMenuWidget::Initialize()
{
	if (!Super::Initialize())
	{
		return false;	
	}
	
	if (Button_Host)
	{
		Button_Host->OnClicked.AddDynamic(this, &UMultiplayerHandlerMenuWidget::HostButtonClicked);
	}
	if (Button_Join)
	{
		Button_Join->OnClicked.AddDynamic(this, &UMultiplayerHandlerMenuWidget::JoinButtonClicked);
	}
	
	return true;
}

void UMultiplayerHandlerMenuWidget::NativeDestruct()
{
	Super::NativeDestruct();

	RemoveMenu();
}

void UMultiplayerHandlerMenuWidget::RemoveMenu()
{
	RemoveFromParent();

	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		const FInputModeGameOnly InputModeData;
		PlayerController->SetInputMode(InputModeData);
		PlayerController->bShowMouseCursor = false;
	}
}

void UMultiplayerHandlerMenuWidget::HostButtonClicked()
{
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Black, TEXT("Host Button Clicked!"));

	if (MultiplayerHandlerSubsystem)
	{
		MultiplayerHandlerSubsystem->CreateSession(NumPublicConnections, MatchType);

		GetWorld()->ServerTravel("/Game/ThirdPerson/Maps/Lobby?listen");
	}
}

void UMultiplayerHandlerMenuWidget::JoinButtonClicked()
{
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Black, TEXT("Join Button Clicked!"));
}

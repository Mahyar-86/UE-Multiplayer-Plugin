// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiplayerHandlerMenuWidget.h"

#include "MultiplayerHandlerSubsystem.h"
#include "Components/Button.h"
#include "OnlineSubsystemUtils.h"

void UMultiplayerHandlerMenuWidget::SetupMenu(const int32 NumberOfPublicConnections, FString TypeOfMatch, FString LobbyPath)
{
	PathToLobby = LobbyPath + "?listen";
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

	if (MultiplayerHandlerSubsystem)
	{
		MultiplayerHandlerSubsystem->MultiplayerHandlerOnCreateSessionCompleteDelegate.AddDynamic(this, &UMultiplayerHandlerMenuWidget::OnCreateSession);
		MultiplayerHandlerSubsystem->MultiplayerHandlerOnFindSessionCompleteDelegate.AddUObject(this, &UMultiplayerHandlerMenuWidget::OnFindSession);
		MultiplayerHandlerSubsystem->MultiplayerHandlerOnJoinSessionCompleteDelegate.AddUObject(this, &UMultiplayerHandlerMenuWidget::OnJoinSession);
		MultiplayerHandlerSubsystem->MultiplayerHandlerOnDestroySessionCompleteDelegate.AddDynamic(this, &UMultiplayerHandlerMenuWidget::OnDestroySession);
		MultiplayerHandlerSubsystem->MultiplayerHandlerOnStartSessionCompleteDelegate.AddDynamic(this, &UMultiplayerHandlerMenuWidget::OnStartSession);
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
	if (!MultiplayerHandlerSubsystem)
	{
		return;
	}

	MultiplayerHandlerSubsystem->CreateSession(NumPublicConnections, MatchType);
}

void UMultiplayerHandlerMenuWidget::JoinButtonClicked()
{
	if (!MultiplayerHandlerSubsystem)
	{
		return;
	}

	MultiplayerHandlerSubsystem->FindSession(10000); // Cause many players using 480 dev steam id
}


void UMultiplayerHandlerMenuWidget::OnCreateSession(const bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Black, TEXT("Session Created Successfully!"));
		GetWorld()->ServerTravel(PathToLobby);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Black, TEXT("Failed to Create the Session!"));
	}
}

void UMultiplayerHandlerMenuWidget::OnFindSession(const TArray<FOnlineSessionSearchResult>& SessionResults, bool bWasSuccessful) const
{
	if (!MultiplayerHandlerSubsystem)
	{
		return;
	}

	for (FOnlineSessionSearchResult SearchResult: SessionResults)
	{
		FString SessionMatchType;
		SearchResult.Session.SessionSettings.Get(FName("MatchType"), SessionMatchType);

		if (SessionMatchType == MatchType)
		{
			MultiplayerHandlerSubsystem->JoinSession(SearchResult);
			return;
		}
	}
}

void UMultiplayerHandlerMenuWidget::OnJoinSession(EOnJoinSessionCompleteResult::Type Result) const
{
	if (const IOnlineSubsystem* OnlineSubsystem = Online::GetSubsystem(UObject::GetWorld()))
	{
		if (const TWeakPtr<IOnlineSession> OnlineSessionInterface = OnlineSubsystem->GetSessionInterface(); OnlineSessionInterface.IsValid())
		{
			FString Address;
			OnlineSessionInterface.Pin()->GetResolvedConnectString(NAME_GameSession, Address);
			
			if (APlayerController* LocalPlayerController = GetGameInstance()->GetFirstLocalPlayerController())
			{
				LocalPlayerController->ClientTravel(Address, TRAVEL_Absolute);
			}
		}
	}
}

void UMultiplayerHandlerMenuWidget::OnDestroySession(bool bWasSuccessful)
{
	
}

void UMultiplayerHandlerMenuWidget::OnStartSession(bool bWasSuccessful)
{
	
}

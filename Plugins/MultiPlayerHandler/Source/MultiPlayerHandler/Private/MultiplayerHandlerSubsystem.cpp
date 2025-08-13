// Fill out your copyright notice in the Description page of Project Settings.

#include "MultiplayerHandlerSubsystem.h"

#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"

UMultiplayerHandlerSubsystem::UMultiplayerHandlerSubsystem()
{
	if (const IOnlineSubsystem* OnlineSubsystem = Online::GetSubsystem(UObject::GetWorld()))
	{
		OnlineSessionInterface = OnlineSubsystem->GetSessionInterface();
	}

	OnCreateSessionCompleteDelegate.BindUObject(this, &UMultiplayerHandlerSubsystem::OnCreateSessionComplete);
	OnFindSessionsCompleteDelegate.BindUObject(this, &UMultiplayerHandlerSubsystem::OnFindSessionComplete);
	OnJoinSessionCompleteDelegate.BindUObject(this, &UMultiplayerHandlerSubsystem::OnJoinSessionComplete);
	OnDestroySessionCompleteDelegate.BindUObject(this, &UMultiplayerHandlerSubsystem::OnDestroySessionComplete);
	OnStartSessionCompleteDelegate.BindUObject(this, &UMultiplayerHandlerSubsystem::OnStartSessionComplete);
}

void UMultiplayerHandlerSubsystem::CreateSession(int32 NumPublicConnections, FString MatchType)
{
	if (!OnlineSessionInterface.IsValid())
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("OnlineSessionInterface is not valid in CreateSession"));
		return;
	}

	if (OnlineSessionInterface.Pin()->GetNamedSession(NAME_GameSession))
	{
		OnlineSessionInterface.Pin()->DestroySession(NAME_GameSession);
	}

	OnCreateSessionCompleteDelegateHandle = OnlineSessionInterface.Pin()->AddOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegate);

	LastSessionSettings = MakeShareable(new FOnlineSessionSettings);
	LastSessionSettings->bIsLANMatch = Online::GetSubsystem(GetWorld())->GetSubsystemName() == "NULL";
	LastSessionSettings->NumPublicConnections = NumPublicConnections;
	LastSessionSettings->bAllowJoinInProgress = true;
	LastSessionSettings->bAllowJoinViaPresence = true;
	LastSessionSettings->bShouldAdvertise = true;
	LastSessionSettings->bUsesPresence = true;
	LastSessionSettings->bUseLobbiesIfAvailable = true;
	LastSessionSettings->Set(FName("MatchType"), MatchType, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

	if (!OnlineSessionInterface.Pin()->CreateSession(*GetLocalPlayer()->GetPreferredUniqueNetId(), NAME_GameSession, *LastSessionSettings))
	{
		OnlineSessionInterface.Pin()->ClearOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegateHandle);
	}
}

void UMultiplayerHandlerSubsystem::FindSession(int32 MaxSearchResults)
{
	if (!OnlineSessionInterface.IsValid())
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("OnlineSessionInterface is not valid in FindSession"));
		return;
	}
}

void UMultiplayerHandlerSubsystem::JoinSession(const FOnlineSessionSearchResult& SearchResult)
{
	if (!OnlineSessionInterface.IsValid())
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("OnlineSessionInterface is not valid in JoinSession"));
		return;
	}
}

void UMultiplayerHandlerSubsystem::DestroySession()
{
	if (!OnlineSessionInterface.IsValid())
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("OnlineSessionInterface is not valid in DestroySession"));
		return;
	}
}

void UMultiplayerHandlerSubsystem::StartSession()
{
	if (!OnlineSessionInterface.IsValid())
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("OnlineSessionInterface is not valid in StartSession"));
		return;
	}
}

void UMultiplayerHandlerSubsystem::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	if (!OnlineSessionInterface.IsValid())
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("OnlineSessionInterface is not valid in OnCreateSessionComplete"));
		return;
	}
}

void UMultiplayerHandlerSubsystem::OnFindSessionComplete(bool bWasSuccessful)
{
	if (!OnlineSessionInterface.IsValid())
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("OnlineSessionInterface is not valid in OnFindSessionComplete"));
		return;
	}
}

void UMultiplayerHandlerSubsystem::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (!OnlineSessionInterface.IsValid())
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("OnlineSessionInterface is not valid in OnJoinSessionComplete"));
		return;
	}
}

void UMultiplayerHandlerSubsystem::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
	if (!OnlineSessionInterface.IsValid())
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("OnlineSessionInterface is not valid in OnDestroySessionComplete"));
		return;
	}
}

void UMultiplayerHandlerSubsystem::OnStartSessionComplete(FName SessionName, bool bWasSuccessful)
{
	if (!OnlineSessionInterface.IsValid())
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("OnlineSessionInterface is not valid in OnStartSessionComplete"));
		return;
	}
}

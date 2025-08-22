// Fill out your copyright notice in the Description page of Project Settings.

#include "MultiplayerHandlerSubsystem.h"

#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"
#include "Online/OnlineSessionNames.h"

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

void UMultiplayerHandlerSubsystem::CreateSession(const int32 NumPublicConnections, const FString& MatchType)
{
	if (!OnlineSessionInterface.IsValid())
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("OnlineSessionInterface is not valid in CreateSession"));
		return;
	}

	if (OnlineSessionInterface.Pin()->GetNamedSession(NAME_GameSession))
	{
		bCreateSessionOnDestroy = true;
		LastNumPublicConnections = NumPublicConnections;
		LastMatchType = MatchType;
		
		DestroySession();
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
	LastSessionSettings->BuildUniqueId = 1;

	if (!OnlineSessionInterface.Pin()->CreateSession(*GetLocalPlayer()->GetPreferredUniqueNetId(), NAME_GameSession, *LastSessionSettings))
	{
		OnlineSessionInterface.Pin()->ClearOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegateHandle);

		MultiplayerHandlerOnCreateSessionCompleteDelegate.Broadcast(false);
	}
}

void UMultiplayerHandlerSubsystem::FindSession(int32 MaxSearchResults)
{
	if (!OnlineSessionInterface.IsValid())
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("OnlineSessionInterface is not valid in FindSession"));
		return;
	}

	OnFindSessionsCompleteDelegateHandle = OnlineSessionInterface.Pin()->AddOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegate);

	LastSessionSearch = MakeShareable(new FOnlineSessionSearch());
	LastSessionSearch->MaxSearchResults = MaxSearchResults;
	LastSessionSearch->bIsLanQuery = Online::GetSubsystem(GetWorld())->GetSubsystemName() == "NULL";
	LastSessionSearch->QuerySettings.Set(SEARCH_LOBBIES, true, EOnlineComparisonOp::Equals);
	if (!OnlineSessionInterface.Pin()->FindSessions(*GetLocalPlayer()->GetPreferredUniqueNetId(), LastSessionSearch.ToSharedRef()))
	{
		OnlineSessionInterface.Pin()->ClearOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegateHandle);
		MultiplayerHandlerOnFindSessionCompleteDelegate.Broadcast(TArray<FOnlineSessionSearchResult>(), false);
	}
}

void UMultiplayerHandlerSubsystem::JoinSession(const FOnlineSessionSearchResult& SearchResult)
{
	if (!OnlineSessionInterface.IsValid())
	{
		MultiplayerHandlerOnJoinSessionCompleteDelegate.Broadcast(EOnJoinSessionCompleteResult::UnknownError);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("OnlineSessionInterface is not valid in JoinSession"));
		return;
	}

	OnJoinSessionCompleteDelegateHandle = OnlineSessionInterface.Pin()->AddOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegate);

	if (!OnlineSessionInterface.Pin()->JoinSession(*GetLocalPlayer()->GetPreferredUniqueNetId(), NAME_GameSession, SearchResult))
	{
		OnlineSessionInterface.Pin()->ClearOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegateHandle);
		MultiplayerHandlerOnJoinSessionCompleteDelegate.Broadcast(EOnJoinSessionCompleteResult::UnknownError);
	}
}

void UMultiplayerHandlerSubsystem::DestroySession()
{
	if (!OnlineSessionInterface.IsValid())
	{
		MultiplayerHandlerOnDestroySessionCompleteDelegate.Broadcast(false);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("OnlineSessionInterface is not valid in DestroySession"));
		return;
	}

	OnDestroySessionCompleteDelegateHandle = OnlineSessionInterface.Pin()->AddOnDestroySessionCompleteDelegate_Handle(OnDestroySessionCompleteDelegate);

	if (!OnlineSessionInterface.Pin()->DestroySession(NAME_GameSession))
	{
		OnlineSessionInterface.Pin()->ClearOnDestroySessionCompleteDelegate_Handle(OnDestroySessionCompleteDelegateHandle);
		MultiplayerHandlerOnDestroySessionCompleteDelegate.Broadcast(false);
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

void UMultiplayerHandlerSubsystem::OnCreateSessionComplete(FName SessionName, const bool bWasSuccessful)
{
	if (!OnlineSessionInterface.IsValid())
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("OnlineSessionInterface is not valid in OnCreateSessionComplete"));
		return;
	}
	
	OnlineSessionInterface.Pin()->ClearOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegateHandle);
	MultiplayerHandlerOnCreateSessionCompleteDelegate.Broadcast(bWasSuccessful);
}

void UMultiplayerHandlerSubsystem::OnFindSessionComplete(const bool bWasSuccessful)
{
	if (!OnlineSessionInterface.IsValid())
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("OnlineSessionInterface is not valid in OnFindSessionComplete"));
		return;
	}

	OnlineSessionInterface.Pin()->ClearOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegateHandle);

	if (LastSessionSearch->SearchResults.IsEmpty())
	{
		MultiplayerHandlerOnFindSessionCompleteDelegate.Broadcast(TArray<FOnlineSessionSearchResult>(), false);
		return;
	}
	
	MultiplayerHandlerOnFindSessionCompleteDelegate.Broadcast(LastSessionSearch->SearchResults, bWasSuccessful);
}

void UMultiplayerHandlerSubsystem::OnJoinSessionComplete(FName SessionName, const EOnJoinSessionCompleteResult::Type Result)
{
	if (!OnlineSessionInterface.IsValid())
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("OnlineSessionInterface is not valid in OnJoinSessionComplete"));
		return;
	}

	OnlineSessionInterface.Pin()->ClearOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegateHandle);

	MultiplayerHandlerOnJoinSessionCompleteDelegate.Broadcast(Result);
}

void UMultiplayerHandlerSubsystem::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
	if (!OnlineSessionInterface.IsValid())
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("OnlineSessionInterface is not valid in OnDestroySessionComplete"));
		return;
	}

	OnlineSessionInterface.Pin()->ClearOnDestroySessionCompleteDelegate_Handle(OnDestroySessionCompleteDelegateHandle);

	if (bWasSuccessful && bCreateSessionOnDestroy)
	{
		bCreateSessionOnDestroy = false;
		
		CreateSession(LastNumPublicConnections, LastMatchType);
	}
	
	MultiplayerHandlerOnDestroySessionCompleteDelegate.Broadcast(bWasSuccessful);
}

void UMultiplayerHandlerSubsystem::OnStartSessionComplete(FName SessionName, bool bWasSuccessful)
{
	if (!OnlineSessionInterface.IsValid())
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("OnlineSessionInterface is not valid in OnStartSessionComplete"));
		return;
	}
}

// Fill out your copyright notice in the Description page of Project Settings.

#include "MultiplayerHandlerSubsystem.h"

#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"

UMultiplayerHandlerSubsystem::UMultiplayerHandlerSubsystem()
{
	if (const IOnlineSubsystem* OnlineSubsystem = Online::GetSubsystem(UObject::GetWorld()))
	{
		OnlineSessionInterface = OnlineSubsystem->GetSessionInterface();
		
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Black, FString::Printf(TEXT("Found subsystem %s"), *OnlineSubsystem->GetSubsystemName().ToString()));
	}

	OnCreateSessionCompleteDelegate.BindUObject(this, &UMultiplayerHandlerSubsystem::OnCreateSessionComplete);
	OnFindSessionsCompleteDelegate.BindUObject(this, &UMultiplayerHandlerSubsystem::OnFindSessionComplete);
	OnJoinSessionCompleteDelegate.BindUObject(this, &UMultiplayerHandlerSubsystem::OnJoinSessionComplete);
	OnDestroySessionCompleteDelegate.BindUObject(this, &UMultiplayerHandlerSubsystem::OnDestroySessionComplete);
	OnStartSessionCompleteDelegate.BindUObject(this, &UMultiplayerHandlerSubsystem::OnStartSessionComplete);
}

void UMultiplayerHandlerSubsystem::CreateSession(int32 NumPublicConnections, FString MatchType)
{
}

void UMultiplayerHandlerSubsystem::FindSession(int32 MaxSearchResults)
{
}

void UMultiplayerHandlerSubsystem::JoinSession(const FOnlineSessionSearchResult& SearchResult)
{
}

void UMultiplayerHandlerSubsystem::DestroySession()
{
}

void UMultiplayerHandlerSubsystem::StartSession()
{
}

void UMultiplayerHandlerSubsystem::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
}

void UMultiplayerHandlerSubsystem::OnFindSessionComplete(bool bWasSuccessful)
{
}

void UMultiplayerHandlerSubsystem::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
}

void UMultiplayerHandlerSubsystem::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
}

void UMultiplayerHandlerSubsystem::OnStartSessionComplete(FName SessionName, bool bWasSuccessful)
{
}

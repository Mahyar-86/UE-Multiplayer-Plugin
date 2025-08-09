// Copyright Epic Games, Inc. All Rights Reserved.

#include "MenuSystem_EOS_SteamGameMode.h"
#include "MenuSystem_EOS_SteamCharacter.h"
#include "UObject/ConstructorHelpers.h"

AMenuSystem_EOS_SteamGameMode::AMenuSystem_EOS_SteamGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

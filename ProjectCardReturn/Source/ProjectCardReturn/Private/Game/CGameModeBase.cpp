// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/CGameModeBase.h"

#include "Entities/Players/Erica/CEricaCharacter.h"
#include "Entities/Players/Erica/CEricaPlayerController.h"

ACGameModeBase::ACGameModeBase()
{
	DefaultPawnClass = ACEricaCharacter::StaticClass();
	PlayerControllerClass = ACEricaPlayerController::StaticClass();
}

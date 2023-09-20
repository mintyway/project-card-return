// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/CGameModeBase.h"

#include "Entities/Players/EricaRoss/CEricaRossCharacter.h"
#include "Entities/Players/EricaRoss/CEricaRossPlayerController.h"

ACGameModeBase::ACGameModeBase()
{
	DefaultPawnClass = ACEricaRossCharacter::StaticClass();
	PlayerControllerClass = ACEricaRossPlayerController::StaticClass();
}

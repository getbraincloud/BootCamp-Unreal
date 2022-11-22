// Copyright 2022 bitHeads, Inc. All Rights Reserved.


#include "SpaceShooterPlayerController.h"
#include "DialogWidget.h"


void ASpaceShooterPlayerController::BeginPlay()
{
    SetShowMouseCursor(true);
    
    FInputModeGameAndUI inputMode;
    SetInputMode(inputMode);
}

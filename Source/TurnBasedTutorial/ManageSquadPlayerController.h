// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ManageSquadTrooper.h"
#include "ManageSquadPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class TURNBASEDTUTORIAL_API AManageSquadPlayerController
    : public APlayerController {
    GENERATED_BODY()

public:
    AManageSquadPlayerController();
    
    virtual void SetupInputComponent() override;
    
private:
    UPROPERTY()
    AManageSquadTrooper *SelectedTrooper;

    UFUNCTION()
    void OnLeftMouseClick();
};

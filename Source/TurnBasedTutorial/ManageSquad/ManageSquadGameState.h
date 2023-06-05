// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "ManageSquadGameState.generated.h"

/**
 * 
 */
UCLASS()
class TURNBASEDTUTORIAL_API AManageSquadGameState : public AGameState {
    GENERATED_BODY()

public:
    AManageSquadGameState();

    UFUNCTION()
    void ChangeSquad(int TrooperIndex, int TrooperKind);

private:
    UPROPERTY()
    TArray<int> TroopersKinds;
};

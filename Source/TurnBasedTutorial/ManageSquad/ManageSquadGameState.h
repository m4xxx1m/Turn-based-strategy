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
    virtual void BeginPlay() override;

    UFUNCTION()
    void ChangeSquad(int TrooperIndex, int TrooperKind);

    UFUNCTION()
    TArray<uint8> GetSquad() const;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<UClass *> TroopersBpAssets;

    UFUNCTION()
    void InitializeTroopers() const;
    
    UPROPERTY()
    TArray<uint8> TroopersKinds;
};

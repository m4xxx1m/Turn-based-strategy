// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyPlayerState.h"
#include "GameFramework/GameState.h"
#include "MyGameState.generated.h"

/**
 * 
 */
UCLASS()
class TURNBASEDTUTORIAL_API AMyGameState : public AGameState {
    GENERATED_BODY()

public:
    virtual void BeginPlay() override;
    
    UFUNCTION()
    void AddTrooper(ATrooper *Trooper);

    UFUNCTION(Server, Reliable)
    void StartGame() const;
    
    UFUNCTION(BlueprintCallable, Server, Reliable)
    void CycleTurns();

    UFUNCTION(BlueprintPure)
    AMyPlayerState *PlayerInTurn() const;

    UFUNCTION(BlueprintPure)
    AMyPlayerState *PlayerNotInTurn() const;

    auto GetMyPlayerState(uint8 PlayerIndex) const;

private:
    UPROPERTY(Replicated)
    TArray<ATrooper *> Troopers;

    UPROPERTY(Replicated)
    uint8 CurrentPlayerTurn{0};
    
};

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
    
    UFUNCTION(Server, Reliable)
    void AddTrooper(ATrooper *Trooper);

    UFUNCTION(Server, Reliable)
    void StartGame();
    
    UFUNCTION(BlueprintCallable, Server, Reliable)
    void CycleTurns();

    UFUNCTION(BlueprintPure)
    AMyPlayerState *PlayerInTurn() const;

    UFUNCTION(BlueprintPure)
    AMyPlayerState *PlayerNotInTurn() const;
    
    UFUNCTION()
    TArray<ATrooper *> GetTroopers() const;

    UFUNCTION()
    bool IsInTurn(uint8 PlayerIndex) const;

    UFUNCTION()
    bool IsGameStarted() const;

    UFUNCTION()
    void DecreaseLivingTroopers(int PlayerIndex);

private:
    UPROPERTY(Replicated)
    bool bGameIsOver = false;
    
    UPROPERTY(Replicated)
    TArray<ATrooper *> Troopers;

    UPROPERTY(Replicated)
    TArray<int> LivingTroopers;

    UPROPERTY(Replicated)
    bool bGameStarted = false;

    UPROPERTY(Replicated)
    uint8 CurrentPlayerTurn{0};
    
    auto GetMyPlayerState(uint8 PlayerIndex) const;
};

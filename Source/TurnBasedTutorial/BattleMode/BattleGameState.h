// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BattlePlayerState.h"
#include "GameFramework/GameState.h"
#include "BattleGameState.generated.h"

/**
 * 
 */
UCLASS()
class TURNBASEDTUTORIAL_API ABattleGameState : public AGameState {
    GENERATED_BODY()

public:
    ABattleGameState();

    virtual void BeginPlay() override;

    UFUNCTION(Server, Reliable)
    virtual void AddTrooper(ATrooper *Trooper);

    UFUNCTION(Server, Reliable)
    void StartGame();

    UFUNCTION(BlueprintCallable, Server, Reliable)
    virtual void CycleTurns();

    UFUNCTION(BlueprintPure)
    ABattlePlayerState *PlayerInTurn() const;

    UFUNCTION(BlueprintPure)
    ABattlePlayerState *PlayerNotInTurn() const;

    UFUNCTION()
    TArray<ATrooper *> GetTroopers() const;

    UFUNCTION()
    bool IsInTurn(uint8 PlayerIndex) const;

    UFUNCTION()
    bool IsGameStarted() const;

    UFUNCTION()
    void DecreaseLivingTroopers(int PlayerIndex);

    UFUNCTION()
    void GameOver(int PlayerIndexLose) const;

protected:
    // UPROPERTY(EditAnywhere, BlueprintReadWrite)
    // TSubclassOf<UUserWidget> GameOverWidgetClass;

    UPROPERTY()
    bool bIsMultiplayer = true;

    UPROPERTY(Replicated)
    bool bGameIsOver = false;

    UPROPERTY(Replicated)
    TArray<ATrooper *> Troopers;

    UPROPERTY(Replicated)
    TArray<int> LivingTroopersCount;

    UPROPERTY(Replicated)
    bool bGameStarted = false;

    UPROPERTY(Replicated)
    uint8 CurrentPlayerTurn{0};

    auto GetMyPlayerState(uint8 PlayerIndex) const;
};

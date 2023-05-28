// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EngineUtils.h"
// #include "MyPlayerController.h"
#include "MyPlayerStart.h"
#include "GameFramework/GameMode.h"
#include "MyGameMode.generated.h"

UCLASS()
class TURNBASEDTUTORIAL_API AMyGameMode : public AGameMode {
    GENERATED_BODY()

public:
    AMyGameMode();

    virtual AActor *
    ChoosePlayerStart_Implementation(AController *Player) override;

    virtual void PostLogin(APlayerController *NewPlayer) override;

    virtual void BeginPlay() override;

    //
    // UFUNCTION(BlueprintCallable)
    // void CycleTurns();

protected:
    UPROPERTY()
    TArray<UClass *> LoadedBpAssets;
    
    UPROPERTY()
    bool bIsMultiplayer = true;
    
    void InitializeSpawnPointsIfNeeded(AController *Player);

    UFUNCTION(Server, Reliable)
    void InitializeBattleField();

    UPROPERTY()
    TMap<uint8, AMyPlayerStart *> SpawnPoints{};
    
    // UPROPERTY()
    // mutable TArray<ATrooper *> Troopers;

    UFUNCTION(Server, Reliable)
    void StartGame();


    // UFUNCTION(BlueprintPure)
    // AMyPlayerController *PlayerInTurn() const;
    //
    // UFUNCTION(BlueprintPure)
    // AMyPlayerController *PlayerNotInTurn() const;

    // UPROPERTY()
    // uint8 CurrentPlayerTurn{0};

    // UFUNCTION()
    // AMyPlayerController *GetMyPlayerController(uint8 const PlayerIndex) const;

    auto GetMyGameState() const;
};

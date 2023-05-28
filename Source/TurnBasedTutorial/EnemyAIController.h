// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class TURNBASEDTUTORIAL_API AEnemyAIController : public AActor {
    GENERATED_BODY()

public:
    AEnemyAIController();

    UFUNCTION()
    void StartTurn();

    UFUNCTION()
    void EndTurn();

    UFUNCTION()
    void ActionDone();

    UFUNCTION()
    void InitializeTroopers(const TArray<ATrooper *> &Troopers);

    virtual void BeginPlay() override;
    
    virtual void Tick(float DeltaSeconds) override;

    UPROPERTY()
    bool bIsEnded = false;

    UFUNCTION()
    bool IsAITurn() const;

    UFUNCTION()
    void SpawnIfNeeded();

    // UFUNCTION()
    // void SetTrooperAssetsAndSpawn(TArray<UClass *> TrooperAssets, int TrooperCount);

private:
    static constexpr int AI_INDEX = 1;

    static constexpr int PLAYER_INDEX = 0;

    static constexpr int MAX_TROOPERS_COUNT = 3;

    UPROPERTY()
    int TroopersCount = 5;

    // UPROPERTY()
    // TArray<UClass *> LoadedTrooperAssets;

    UFUNCTION()
    void RemoveDeadTroopers();
    
    FVector GetFreeLocation() const;
    
    UFUNCTION()
    void MakeMove();
    
    UPROPERTY()
    int TroopersCursor = 0;

    int GetClosestTrooper() const;

    bool IsCloseEnough(int TrooperIndex) const;

    bool TryAttack(int TrooperIndex);

    bool MoveTo(int TrooperIndex);

    void InitializeSpawnPoints();
    
    UPROPERTY()
    bool bIsAITurn = false;

    UPROPERTY()
    bool bIsActing = false;

    UPROPERTY()
    TArray<ATrooper *> PossessedTroopers;

    UPROPERTY()
    TArray<ATrooper *> PlayerTroopers;

    TArray<FVector> SpawnPoints;

    const FVector TROOPERS_DISTANCE = {0.0f, 1000.0f, 0.0f};
};

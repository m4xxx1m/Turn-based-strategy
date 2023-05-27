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
    static constexpr int AI_INDEX = 1;
    
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

private:
    UFUNCTION()
    void MakeMove();
    
    UPROPERTY()
    int TroopersCursor = 0;

    int GetClosestTrooper() const;

    bool IsCloseEnough(int TrooperIndex) const;

    bool TryAttack(int TrooperIndex);

    bool MoveTo(int TrooperIndex);
    
    UPROPERTY()
    bool bIsAITurn = false;

    UPROPERTY()
    bool bIsActing = false;

    UPROPERTY()
    TArray<ATrooper *> PossessedTroopers;

    UPROPERTY()
    TArray<ATrooper *> PlayerTroopers;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyAIController.h"
#include "MyGameState.h"
#include "SinglePlayerGS.generated.h"

/**
 * 
 */
UCLASS()
class TURNBASEDTUTORIAL_API ASinglePlayerGS : public AMyGameState {
    GENERATED_BODY()
    
public:
    ASinglePlayerGS();

    virtual void BeginPlay() override;

    virtual void CycleTurns() override;

    virtual void Tick(float DeltaSeconds) override;

    AEnemyAIController *GetEnemyAIController() const;

    const TArray<TSubclassOf<ATrooper>> &GetTroopersAssets() const;

protected:
    UPROPERTY(Replicated)
    AEnemyAIController *EnemyAiManager = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<TSubclassOf<ATrooper>> TrooperBpAssets;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Trooper/Trooper.h"
#include "CoreMinimal.h"
#include "BattlePlayerController.h"
#include "GameFramework/PlayerState.h"
#include "BattlePlayerState.generated.h"

/**
 * 
 */
UCLASS()
class TURNBASEDTUTORIAL_API ABattlePlayerState : public APlayerState {
    GENERATED_BODY()

public:
    ABattlePlayerState();

    virtual void BeginPlay() override;

    // virtual void Tick(float DeltaSeconds) override;

    UFUNCTION(Client, Reliable)
    void StartTurn();

    UFUNCTION(Client, Reliable, BlueprintCallable)
    void EndTurn();

    UFUNCTION(Server, Reliable)
    void MoveTrooper(ATrooper *Trooper, FVector Location);

    UFUNCTION(Server, Reliable)
    void Attack(ATrooper *Attacker,
                FVector Location,
                int ActionIndex);

    // UFUNCTION(Client, Reliable)
    // void CycleTurns() const;

    UFUNCTION(BlueprintCallable)
    bool IsMyTurn() const;

    UFUNCTION()
    void OnPlayerAction(const FHitResult &HitResult);

    UFUNCTION(BlueprintCallable, Client, Reliable)
    void SetCurrentAction(int Action);

    UFUNCTION(BlueprintCallable)
    uint8 GetPlayerIndex() const;

    UFUNCTION()
    void SetPlayerIndex(uint8 NewPlayerIndex);

    UFUNCTION(Client, Reliable)
    void SetEnemySelection(/*const TArray<AActor *> &Troopers*/) const;

    UFUNCTION(Client, Reliable)
    void GameOver(int PlayerLoseIndex);
    
    // UFUNCTION(Client, Reliable)
    // void SetBattleWidget(UBattleUI *BattleWidget);

protected:
    // UPROPERTY()
    // UBattleUI *BattleUI;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<UUserWidget> GameOverWidgetClass;

    UPROPERTY(Replicated)
    bool bIsSelectionInitialized = false;

    UPROPERTY(Replicated)
    uint8 PlayerIndex;

    UFUNCTION()
    void SetMyTurn(bool bMyTurn);

    UPROPERTY(Replicated)
    bool bIsMyTurn;

    UPROPERTY(Replicated)
    int CurrentAction = 0;

    UPROPERTY(Replicated)
    ATrooper *SelectedTrooper;

    auto GetMyGameState() const;
};

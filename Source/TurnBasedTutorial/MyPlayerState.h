// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "MyPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class TURNBASEDTUTORIAL_API AMyPlayerState : public APlayerState {
    GENERATED_BODY()
public:
    AMyPlayerState();

    virtual void BeginPlay() override;

    UFUNCTION(Client, Reliable)
    void StartTurn();
    
    UFUNCTION(Client, Reliable, BlueprintCallable)
    void EndTurn();

    UFUNCTION(Server, Reliable)
    void MoveTrooper(ATrooper *Trooper, FVector Location);

    UFUNCTION(Server, Reliable)
    void Attack(ATrooper *Attacker, FVector Location, int ActionIndex);

    UFUNCTION()
    void CycleTurns() const;

    UFUNCTION(BlueprintCallable)
    bool IsMyTurn() const;

    UFUNCTION()
    void OnPlayerAction(const FHitResult &HitResult);

    UFUNCTION(BlueprintCallable, Client, Reliable)
    void SetCurrentAction(int Action);
    
    UFUNCTION(BlueprintCallable)
    uint8 GetPlayerIndex();

    UFUNCTION()
    void SetPlayerIndex(uint8 NewPlayerIndex);

private:
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
};

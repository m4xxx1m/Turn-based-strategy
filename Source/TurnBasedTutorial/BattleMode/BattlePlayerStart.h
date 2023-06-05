// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "BattlePlayerStart.generated.h"

/**
 * 
 */
UCLASS()
class TURNBASEDTUTORIAL_API ABattlePlayerStart : public APlayerStart {
    GENERATED_BODY()

public:
    uint8 GetPlayerIndex() const;

protected:
private:
    UPROPERTY(EditAnywhere, Category="Spawn Info")
    uint8 PlayerIndex = 0;
};

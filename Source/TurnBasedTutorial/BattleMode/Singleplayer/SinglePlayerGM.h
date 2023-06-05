// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../BattleGameMode.h"
#include "SinglePlayerGM.generated.h"

/**
 * 
 */
UCLASS()
class TURNBASEDTUTORIAL_API ASinglePlayerGM : public ABattleGameMode {
    GENERATED_BODY()

public:
    ASinglePlayerGM();

    virtual void BeginPlay() override;

    virtual void PostLogin(APlayerController *NewPlayer) override;
};

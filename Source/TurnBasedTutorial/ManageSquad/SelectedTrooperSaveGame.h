// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SelectedTrooperSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class TURNBASEDTUTORIAL_API USelectedTrooperSaveGame : public USaveGame {
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, Category=Basic)
    TArray<uint8> SelectedTroopers;

    USelectedTrooperSaveGame();
};

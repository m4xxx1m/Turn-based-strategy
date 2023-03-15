// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EngineUtils.h"
#include "MyPlayerController.h"
#include "MyGameState.h"
#include "MyPawn.h"
#include "MyPlayerStart.h"
#include "GameFramework/GameMode.h"
#include "MyGameMode.generated.h"



UCLASS()
class TURNBASEDTUTORIAL_API AMyGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	AMyGameMode();

	void BeginPlay() override;

    AActor *ChoosePlayerStart_Implementation(AController * Player);
private:
	void StartGame();

    void InitializeSpawnPointsIfNeeded();

	AMyPlayerController *GetPlayerController();

    TMap<uint8, AMyPlayerStart*> SpawnPoints;
};

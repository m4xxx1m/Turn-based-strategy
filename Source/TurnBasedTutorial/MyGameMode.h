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

	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;

private:
	void InitializeSpawnPointsIfNeeded(AController *Player);

	TMap<uint8, AMyPlayerStart*> SpawnPoints;
};

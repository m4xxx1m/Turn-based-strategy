// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EngineUtils.h"
#include "MyPlayerController.h"
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

	virtual void PostLogin(APlayerController* NewPlayer) override;

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void CycleTurns();

private:
	void InitializeSpawnPointsIfNeeded(AController* Player);

	void InitializeBattleField() const;

	UPROPERTY()
	TMap<uint8, AMyPlayerStart*> SpawnPoints{};

	AMyPlayerController* GetMyPlayerController(uint8 const PlayerIndex) const;

	UFUNCTION(BlueprintCallable)
	void StartGame();

	UFUNCTION(BlueprintPure)
	AMyPlayerController* PlayerInTurn() const;

	UFUNCTION(BlueprintPure)
	AMyPlayerController* PlayerNotInTurn() const;

	UPROPERTY()
	uint8 CurrentPlayerTurn{0};
};

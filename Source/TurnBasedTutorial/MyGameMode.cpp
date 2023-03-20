// Fill out your copyright notice in the Description page of Project Settings.

#include "MyGameMode.h"
#include "Kismet/GameplayStatics.h"


AMyGameMode::AMyGameMode() : Super()
{
	UE_LOG(LogTemp, Warning, TEXT("GameMode Constructor"));
	GameStateClass = AMyGameState::StaticClass();
	PlayerControllerClass = AMyPlayerController::StaticClass();
	DefaultPawnClass = AMyPawn::StaticClass();
}

AActor* AMyGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	UE_LOG(LogTemp, Warning, TEXT("GameMode ChoosePlayerStart %d"), GetNumPlayers());
	InitializeSpawnPointsIfNeeded(Player);
	auto ptr = *SpawnPoints.Find(GetNumPlayers());
	UE_LOG(LogTemp, Warning, TEXT("GameMode ChoosePlayerStart end %d"), ptr->GetPlayerIndex());
	return ptr;
}

void AMyGameMode::InitializeSpawnPointsIfNeeded(AController* Player)
{
	if (SpawnPoints.Num() != 0)
	{
		return;
	}
	auto World = GetWorld();
	for (TActorIterator<AMyPlayerStart> PlayerStartIterator(GetWorld()); PlayerStartIterator; ++PlayerStartIterator)
	{
		auto PlayerStart = *PlayerStartIterator;
		UClass* PawnClass = GetDefaultPawnClassForController(Player);
		APawn* PawnToFit = PawnClass ? PawnClass->GetDefaultObject<APawn>() : nullptr;
		FVector ActorLocation = PlayerStart->GetActorLocation();
		const FRotator ActorRotation = PlayerStart->GetActorRotation();
		if (!World->EncroachingBlockingGeometry(PawnToFit, ActorLocation, ActorRotation))
		{
			SpawnPoints.Add(PlayerStartIterator->GetPlayerIndex(), *PlayerStartIterator);
			UE_LOG(LogTemp, Warning, TEXT("PlayerStart unoccupied iterator %d"), PlayerStartIterator->GetPlayerIndex());
		}
	}
}

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
	InitializeSpawnPointsIfNeeded();
	auto ptr = *SpawnPoints.Find(GetNumPlayers());
	UE_LOG(LogTemp, Warning, TEXT("GameMode ChoosePlayerStart end %d"), ptr->GetPlayerIndex());
	return ptr;
}

void AMyGameMode::InitializeSpawnPointsIfNeeded()
{
	if (SpawnPoints.Num() != 0)
	{
		return;
	}
	for (TActorIterator <AMyPlayerStart> PlayerStartIterator(GetWorld()); PlayerStartIterator; ++PlayerStartIterator) {
	    UE_LOG(LogTemp, Warning, TEXT("PlayerStart iterator %d"), PlayerStartIterator->GetPlayerIndex());
	    SpawnPoints.Add(PlayerStartIterator->GetPlayerIndex(), *PlayerStartIterator);
	}
	// TArray<AActor*> MyPlayerStartObjects;
	// UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMyPlayerStart::StaticClass(), MyPlayerStartObjects);
	// for (const auto& PlayerStart : MyPlayerStartObjects)
	// {
	// 	SpawnPoints.Add(dynamic_cast<AMyPlayerStart*>(PlayerStart)->GetPlayerIndex(),
	// 	                dynamic_cast<AMyPlayerStart*>(PlayerStart));
	// }
}

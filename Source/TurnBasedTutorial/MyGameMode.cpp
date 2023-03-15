// Fill out your copyright notice in the Description page of Project Settings.

#include "MyGameMode.h"
#include "Kismet/GameplayStatics.h"


AMyGameMode::AMyGameMode() : Super() {
    UE_LOG(LogTemp, Warning, TEXT("GameMode Constructor"));
    GameStateClass = AMyGameState::StaticClass();
    PlayerControllerClass = AMyPlayerController::StaticClass();
    DefaultPawnClass = AMyPawn::StaticClass();
}

AActor *AMyGameMode::ChoosePlayerStart(AController *Player) {
    InitializeSpawnPointsIfNeeded();
    return *SpawnPoints.Find(GetNumPlayers());
}

void AMyGameMode::InitializeSpawnPointsIfNeeded() {
    if (SpawnPoints.Num() != 0) {
        return;
    }
    for (TActorIterator <AMyPlayerStart> PlayerStartIterator(GetWorld()); PlayerStartIterator; ++PlayerStartIterator) {
        SpawnPoints[PlayerStartIterator->GetPlayerIndex()] = *PlayerStartIterator;
    }
}

void AMyGameMode::BeginPlay() {
    Super::BeginPlay();
}

void AMyGameMode::StartGame() {
    GetPlayerController()->StartTurn();
}

AMyPlayerController *AMyGameMode::GetPlayerController() {
    return dynamic_cast<AMyPlayerController *>(
            UGameplayStatics::GetPlayerController(GetWorld(), 0)
    );
}

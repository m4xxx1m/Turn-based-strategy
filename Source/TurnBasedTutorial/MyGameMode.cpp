// Fill out your copyright notice in the Description page of Project Settings.

#include "MyGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Trooper.h"

AMyGameMode::AMyGameMode() : Super() {
	UE_LOG(LogTemp, Warning, TEXT("GameMode Constructor"));
	PlayerControllerClass = AMyPlayerController::StaticClass();
}

void AMyGameMode::BeginPlay() {
	Super::BeginPlay();
	ATrooper::InitNumberOfTroopersForId();
	UE_LOG(LogTemp, Warning, TEXT("GameMode BeginPlay"));
	if (GetWorld()->GetMapName().Contains("BattleFieldMap")) {
		UE_LOG(LogTemp, Warning, TEXT("Player Logined"));
		StartGame();
	}
}

void AMyGameMode::StartGame() {
	FVector Location(2000.0f, -1000.0f, 0.0f);
	FRotator Rotation(0.0f, 180.0f, 0.0f);
	FActorSpawnParameters SpawnInfo;
	for (int i = 0; i < 5; ++i) {
		AActor *spawned = GetWorld()->SpawnActor<ATrooper>(Location, Rotation, SpawnInfo);
		dynamic_cast<ATrooper*>(spawned)->InitTrooper(Location, true);
		Location += { 0.f, 500.f, 0.0f };
	}
	Location = { -2000.0f, -1000.0f, 0.0f };
	Rotation = { 0.0f, 0.0f, 0.0f };
	for (int i = 0; i < 5; ++i) {
		AActor* spawned = GetWorld()->SpawnActor<ATrooper>(Location, Rotation, SpawnInfo);
		dynamic_cast<ATrooper*>(spawned)->InitTrooper(Location, false);
		Location += { 0.f, 500.f, 0.0f };
	}
	GetPlayerController()->StartTurn();
}

AMyPlayerController *AMyGameMode::GetPlayerController() {
	return dynamic_cast<AMyPlayerController *>(
		UGameplayStatics::GetPlayerController(GetWorld(), 0)
	);
}

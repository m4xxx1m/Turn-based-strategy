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

void AMyGameMode::BeginPlay()
{
	Super::BeginPlay();
	ATrooper::InitNumberOfTroopersForId();
	UE_LOG(LogTemp, Warning, TEXT("GameMode BeginPlay"));
	if (GetWorld()->GetMapName().Contains("BattleFieldMap"))
	{
		UE_LOG(LogTemp, Warning, TEXT("Player Logined"));
		InitializeBattleField();
		GetPlayerController()->StartTurn();
	}
}

void AMyGameMode::InitializeBattleField() const
{
	FVector Location(2000.0f, -1000.0f, 0.0f);
	FRotator Rotation(0.0f, 180.0f, 0.0f);
	FActorSpawnParameters const SpawnInfo;
	for (int i = 0; i < 5; ++i)
	{
		AActor* Spawned = GetWorld()->SpawnActor<ATrooper>(Location, Rotation, SpawnInfo);
		dynamic_cast<ATrooper*>(Spawned)->InitTrooper(Location, true);
		Location += {0.f, 500.f, 0.0f};
	}
	Location = {-2000.0f, -1000.0f, 0.0f};
	Rotation = {0.0f, 0.0f, 0.0f};
	for (int i = 0; i < 5; ++i)
	{
		AActor* Spawned = GetWorld()->SpawnActor<ATrooper>(Location, Rotation, SpawnInfo);
		dynamic_cast<ATrooper*>(Spawned)->InitTrooper(Location, false);
		Location += {0.f, 500.f, 0.0f};
	}
}


AActor* AMyGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	UE_LOG(LogTemp, Warning, TEXT("GameMode ChoosePlayerStart %d"), GetNumPlayers());
	InitializeSpawnPointsIfNeeded(Player);
	const auto CurrentPlayerStart = *SpawnPoints.Find(GetNumPlayers());
	UE_LOG(LogTemp, Warning, TEXT("GameMode ChoosePlayerStart end %d"), CurrentPlayerStart->GetPlayerIndex());
	return CurrentPlayerStart;
}

void AMyGameMode::InitializeSpawnPointsIfNeeded(AController* Player)
{
	if (SpawnPoints.Num() != 0)
	{
		return;
	}
	const auto World = GetWorld();
	for (TActorIterator<AMyPlayerStart> PlayerStartIterator(GetWorld()); PlayerStartIterator; ++PlayerStartIterator)
	{
		const auto PlayerStart = *PlayerStartIterator;
		const UClass* PawnClass = GetDefaultPawnClassForController(Player);
		const APawn* PawnToFit = PawnClass ? PawnClass->GetDefaultObject<APawn>() : nullptr;
		const FVector ActorLocation = PlayerStart->GetActorLocation();
		const FRotator ActorRotation = PlayerStart->GetActorRotation();
		if (!World->EncroachingBlockingGeometry(PawnToFit, ActorLocation, ActorRotation))
		{
			SpawnPoints.Add(PlayerStartIterator->GetPlayerIndex(), *PlayerStartIterator);
			UE_LOG(LogTemp, Warning, TEXT("PlayerStart unoccupied iterator %d"), PlayerStartIterator->GetPlayerIndex());
		}
	}
}


AMyPlayerController* AMyGameMode::GetPlayerController() const
{
	return dynamic_cast<AMyPlayerController*>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
}

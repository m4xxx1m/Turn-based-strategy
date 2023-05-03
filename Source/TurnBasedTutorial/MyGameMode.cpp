// Fill out your copyright notice in the Description page of Project Settings.

#include "MyGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "MyPawn.h"
#include "MyGameState.h"

AMyGameMode::AMyGameMode()
    : Super() {
    UE_LOG(LogTemp, Warning, TEXT("GameMode Constructor"));
    GameStateClass = AMyGameState::StaticClass();
    PlayerControllerClass = AMyPlayerController::StaticClass();
    DefaultPawnClass = AMyPawn::StaticClass();
}

void AMyGameMode::BeginPlay() {
    Super::BeginPlay();
}


void AMyGameMode::InitializeBattleField() const {
    UE_LOG(LogTemp, Warning, TEXT("InitializeBattleField"));
    FVector Location(2000.0f, -1000.0f, 0.0f);
    FRotator Rotation(0.0f, 180.0f, 0.0f);

    uint8 TrooperCount = 0;

    TArray<const TCHAR *> bpPaths{
        TEXT(
            "Blueprint'/Game/Troopers/TrooperSkeletonMelee.TrooperSkeletonMelee_C'"
        ),
        TEXT("Blueprint'/Game/Troopers/TrooperWizard.TrooperWizard_C'")
    };
    TArray<UClass *> LoadedBpAssets;
    for (int i = 0; i < bpPaths.Num(); ++i) {
        TSoftClassPtr<ATrooper> ActorBpClass = TSoftClassPtr<ATrooper>(
            FSoftObjectPath(bpPaths[i])
        );
        LoadedBpAssets.Push(ActorBpClass.LoadSynchronous());
    }

    for (int i = 0; i < 5; ++i) {
        FTransform SpawnLocationAndRotation(Rotation);
        SpawnLocationAndRotation.SetLocation(Location);
        AActor *Spawned = GetWorld()->SpawnActorDeferred<ATrooper>(
            LoadedBpAssets[i % 2], SpawnLocationAndRotation);
        // AActor *Spawned = GetWorld()->SpawnActorDeferred<ATrooper>(
        //     ATrooper::StaticClass(), SpawnLocationAndRotation);
        dynamic_cast<ATrooper *>(Spawned)->Initialize(
            0, Location, TrooperCount++);
        Spawned->FinishSpawning(SpawnLocationAndRotation);
        Spawned->SetActorLocation(Location);
        Troopers.Add(dynamic_cast<ATrooper *>(Spawned));
        Location += {0.f, 500.f, 0.0f};
    }
    Location = {-2000.0f, -1000.0f, 0.0f};
    Rotation = {0.0f, 0.0f, 0.0f};
    for (int i = 0; i < 5; ++i) {
        FTransform SpawnLocationAndRotation(Rotation);
        SpawnLocationAndRotation.SetLocation(Location);
        AActor *Spawned = GetWorld()->SpawnActorDeferred<ATrooper>(
            LoadedBpAssets[i % 2], SpawnLocationAndRotation);
        // AActor *Spawned = GetWorld()->SpawnActorDeferred<ATrooper>(
        //     ATrooper::StaticClass(), SpawnLocationAndRotation);
        dynamic_cast<ATrooper *>(Spawned)->Initialize(
            1, Location, TrooperCount++);
        Spawned->FinishSpawning(SpawnLocationAndRotation);
        Spawned->SetActorLocation(Location);
        Troopers.Add(dynamic_cast<ATrooper *>(Spawned));
        Location += {0.f, 500.f, 0.0f};
    }
}


AActor *AMyGameMode::ChoosePlayerStart_Implementation(AController *Player) {
    UE_LOG(LogTemp, Warning, TEXT("GameMode ChoosePlayerStart %d"),
           GetNumPlayers());
    InitializeSpawnPointsIfNeeded(Player);
    const auto CurrentPlayerStart = *SpawnPoints.Find(GetNumPlayers());
    UE_LOG(LogTemp, Warning, TEXT("GameMode ChoosePlayerStart end %d"),
           CurrentPlayerStart->GetPlayerIndex());
    return CurrentPlayerStart;
}

void AMyGameMode::InitializeSpawnPointsIfNeeded(AController *Player) {
    UE_LOG(LogTemp, Warning, TEXT("InitializeSpawnPointsIfNeeded"));
    if (SpawnPoints.Num() != 0) {
        UE_LOG(LogTemp, Warning, TEXT("InitializeSpawnPointsIfNeeded Exit %d"),
               SpawnPoints.Num());
        return;
    }
    UE_LOG(LogTemp, Warning, TEXT("Rebuilding spawnpoints"));
    const auto World = GetWorld();
    for (TActorIterator<AMyPlayerStart> PlayerStartIterator(GetWorld());
         PlayerStartIterator; ++PlayerStartIterator) {
        const auto PlayerStart = *PlayerStartIterator;
        const UClass *PawnClass = GetDefaultPawnClassForController(Player);
        const APawn *PawnToFit = PawnClass
                                     ? PawnClass->GetDefaultObject<APawn>()
                                     : nullptr;
        const FVector ActorLocation = PlayerStart->GetActorLocation();
        const FRotator ActorRotation = PlayerStart->GetActorRotation();
        UE_LOG(LogTemp, Warning, TEXT("PlayerStart iterator %d"),
               PlayerStartIterator->GetPlayerIndex());
        if (!World->EncroachingBlockingGeometry(PawnToFit, ActorLocation,
                                                ActorRotation)) {
            SpawnPoints.Add(PlayerStartIterator->GetPlayerIndex(),
                            *PlayerStartIterator);
            UE_LOG(LogTemp, Warning, TEXT("PlayerStart unoccupied iterator %d"),
                   PlayerStartIterator->GetPlayerIndex());
        }
    }
}

void AMyGameMode::PostLogin(APlayerController *NewPlayer) {
    Super::PostLogin(NewPlayer);
    UE_LOG(LogTemp, Warning, TEXT("PostLogin"));
    // PlayerControllers.Add(dynamic_cast<AMyPlayerController *>(NewPlayer));
    // const auto World = GetWorld();
    const auto CurrentNumberOfPlayers = GetNumPlayers();

    // 0-indexation
    dynamic_cast<AMyPlayerController *>(NewPlayer)->SetPlayerIndex(
        CurrentNumberOfPlayers - 1);
    UE_LOG(LogTemp, Warning, TEXT("%d"), CurrentNumberOfPlayers);
    if (CurrentNumberOfPlayers == 2) {
        UE_LOG(LogTemp, Warning, TEXT("Game Start"));
        // start the game
        // dynamic_cast<AMyGameState *>(
        //             GetWorld()->GetGameState())->StartGame();
        // InitializeBattleField();
        StartGame();
    } else {
        // delay the game
        UE_LOG(LogTemp, Warning, TEXT("Game Delay"));
    }
}


void AMyGameMode::StartGame() {
    InitializeBattleField();
    // PlayerNotInTurn()->SetEnemySelection(Troopers);
    // PlayerInTurn()->SetEnemySelection(Troopers);
    // PlayerControllers[0]->SetEnemySelection(Troopers);
    // PlayerControllers[1]->SetEnemySelection(Troopers);
    PlayerInTurn()->StartTurn();
}


AMyPlayerController *AMyGameMode::PlayerInTurn() const {
    return GetMyPlayerController(CurrentPlayerTurn);
}

AMyPlayerController *AMyGameMode::PlayerNotInTurn() const {
    // uint8 PlayerControllerIndexNotInTurn;
    // if (CurrentPlayerTurn == 0) {
    //     PlayerControllerIndexNotInTurn = 1;
    // } else {
    //     PlayerControllerIndexNotInTurn = 0;
    // }
    // return GetMyPlayerController(PlayerControllerIndexNotInTurn);
    return GetMyPlayerController(!CurrentPlayerTurn);
}

void AMyGameMode::CycleTurns() {
    if (!this)
        return;
    // PlayerInTurn()->EndTurn();
    for (const auto Trooper : Troopers) {
        if (Trooper != nullptr) {
            Trooper->ResetActionPoints();
        }
    }
    CurrentPlayerTurn = !CurrentPlayerTurn;
    // if (CurrentPlayerTurn == 0) {
    //     CurrentPlayerTurn = 1;
    // } else {
    //     CurrentPlayerTurn = 0;
    // }
    PlayerInTurn()->StartTurn();
}


AMyPlayerController *AMyGameMode::GetMyPlayerController(
    uint8 const PlayerIndex) const {
    return dynamic_cast<AMyPlayerController *>(
        UGameplayStatics::GetPlayerController(GetWorld(), PlayerIndex));
}

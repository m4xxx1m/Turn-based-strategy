// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyAIController.h"
#include "SinglePlayerGS.h"
#include "Kismet/GameplayStatics.h"
#include "GenericPlatform/GenericPlatformMath.h"

AEnemyAIController::AEnemyAIController()
    : Super() {
    PrimaryActorTick.bCanEverTick = true;
    SetActorTickInterval(2.0f);
}

void AEnemyAIController::Tick(float DeltaSeconds) {
    Super::Tick(DeltaSeconds);
    if (bIsAITurn && !bIsActing) {
        if (TroopersCursor >= PossessedTroopers.Num()) {
            EndTurn();
        } else {
            MakeMove();
        }
    }
}


void AEnemyAIController::StartTurn() {
    UE_LOG(LogTemp, Warning, TEXT("Enemy AI StartTurn"));
    bIsAITurn = true;
    bIsActing = false;
    // MakeMove();
}

void AEnemyAIController::EndTurn() {
    bIsAITurn = false;
    TroopersCursor = 0;
    bIsEnded = true;
}

void AEnemyAIController::ActionDone() {
    bIsActing = false;
}

bool AEnemyAIController::IsAITurn() const {
    return bIsAITurn;
}

void AEnemyAIController::SpawnIfNeeded() {
    RemoveDeadTroopers();
    while (PossessedTroopers.Num() < MAX_TROOPERS_COUNT) {
        const FVector Location = GetFreeLocation();
        const FVector Rotation = {0.0f, 0.0f, 0.0f};
        FTransform SpawnLocationAndRotation(Rotation);
        SpawnLocationAndRotation.SetLocation(Location);
        const TArray<TSubclassOf<ATrooper>> &LoadedTroopersAssets = GetWorld()->
            GetGameState<ASinglePlayerGS>()->GetTroopersAssets();
        AActor *Spawned = GetWorld()->SpawnActorDeferred<ATrooper>(
            LoadedTroopersAssets[FMath::RandRange(
                0, LoadedTroopersAssets.Num() - 1)],
            SpawnLocationAndRotation);
        Cast<ATrooper>(Spawned)->Initialize(
            1, Location, TroopersCount++);
        Spawned->FinishSpawning(SpawnLocationAndRotation);
        Spawned->SetActorLocation(Location);
        ATrooper *Trooper = Cast<ATrooper>(Spawned);
        GetWorld()->GetGameState<ASinglePlayerGS>()->AddTrooper(Trooper);
        PossessedTroopers.Add(Trooper);
        Trooper->SetAIPossession(this);
        Trooper->HighlightAsEnemy(PLAYER_INDEX);
    }
}

// void AEnemyAIController::SetTrooperAssetsAndSpawn(
// TArray<UClass *> TrooperAssets,
// int TrooperCount) {
// LoadedTrooperAssets = MoveTemp(TrooperAssets);
// TroopersCount = TrooperCount;
// SpawnIfNeeded();
// }

void AEnemyAIController::RemoveDeadTroopers() {
    for (int index = 0; index < PossessedTroopers.Num(); ++index) {
        if (!PossessedTroopers[index] || !PossessedTroopers[index]->
            IsValidLowLevel() ||
            PossessedTroopers[index]->IsDead()) {
            PossessedTroopers.RemoveAtSwap(index);
            index--;
        }
    }
}

FVector AEnemyAIController::GetFreeLocation() const {
    for (const auto Location : SpawnPoints) {
        bool bIsClose = false;
        for (const auto Trooper : PossessedTroopers) {
            if ((Location - Trooper->GetLocation()).Size() < 100.0f) {
                bIsClose = true;
                break;
            }
        }
        if (bIsClose) {
            continue;
        }
        return Location;
    }
    return {-2000.0f, FMath::RandRange(-2000.0f, 2000.0f), 0.0f};
}

void AEnemyAIController::MakeMove() {
    while (TroopersCursor < PossessedTroopers.Num()) {
        while (TroopersCursor < PossessedTroopers.Num() && (
                   !PossessedTroopers[TroopersCursor] ||
                   !PossessedTroopers[TroopersCursor]->IsValidLowLevel()
                   || PossessedTroopers[TroopersCursor]->IsDead())) {
            ++TroopersCursor;
        }
        if (TroopersCursor >= PossessedTroopers.Num()) {
            return;
        }
        const int Index = GetClosestTrooper();
        if (Index == -1) {
            return;
        }
        bool failed;
        if (!IsCloseEnough(Index)) {
            failed = MoveTo(Index);
        } else {
            failed = TryAttack(Index);
        }
        if (failed) {
            TroopersCursor++;
        } else {
            return;
        }
    }
}

bool AEnemyAIController::IsCloseEnough(int TrooperIndex) const {
    const ATrooper *CurrentTrooper = PossessedTroopers[TroopersCursor];
    const ATrooper *OtherTrooper = PlayerTroopers[TrooperIndex];
    return (CurrentTrooper->GetLocation() - OtherTrooper->GetLocation()).Size()
           <= CurrentTrooper->GetRealActionRadius(1);
}

bool AEnemyAIController::TryAttack(int TrooperIndex) {
    ATrooper *Attacker = PossessedTroopers[TroopersCursor];
    const auto Location = PlayerTroopers[TrooperIndex]->GetLocation();
    constexpr int ActionIndex = 1;
    if (Attacker->CheckAttackCorrectness(Location, ActionIndex)) {
        bIsActing = true;
        Attacker->Attack(ActionIndex, Location);
        return false;
    }
    return true;
}

bool AEnemyAIController::MoveTo(int TrooperIndex) {
    ATrooper *Trooper = PossessedTroopers[TroopersCursor];
    const FVector CurrentLocation = Trooper->GetLocation();
    const FVector Destination = PlayerTroopers[TrooperIndex]->GetLocation();
    constexpr int ActionIndex = 1;
    constexpr int MoveActionIndex = 0;
    const float AttackRadius = Trooper->GetRealActionRadius(ActionIndex);
    const FVector Vector = Destination - CurrentLocation;
    const float MaxLength = Trooper->GetRealActionRadius(MoveActionIndex);
    float PathLength = Vector.Size() - AttackRadius + 10.f;
    PathLength = FMath::Min(PathLength, MaxLength);
    FVector Location = CurrentLocation + Vector.GetSafeNormal2D() *
                       PathLength;
    Location.Z = 0.0f;
    if (PathLength > 1.0f && Trooper->CheckMoveCorrectness(Location)) {
        bIsActing = true;
        Trooper->MoveTrooper(Location);
        return false;
    }
    return true;
}

void AEnemyAIController::InitializeSpawnPoints() {
    SpawnPoints.Add(FVector{
        -2000.0f,
        -(MAX_TROOPERS_COUNT - 1) * TROOPERS_DISTANCE.Y / 2,
        0.0f
    });
    for (int index = 1; index < MAX_TROOPERS_COUNT; ++index) {
        SpawnPoints.Add(SpawnPoints[SpawnPoints.Num() - 1] + TROOPERS_DISTANCE);
    }
}

int AEnemyAIController::GetClosestTrooper() const {
    float minDistance = 1000000.0f;
    int minIndex = -1;
    const ATrooper *CurrentTrooper = PossessedTroopers[TroopersCursor];
    for (int index = 0; index < PlayerTroopers.Num(); ++index) {
        const ATrooper *OtherTrooper = PlayerTroopers[index];
        if (OtherTrooper->IsValidLowLevel() && !OtherTrooper->IsDead()) {
            const float distance = (
                CurrentTrooper->GetLocation() - OtherTrooper->
                GetLocation()).Size();
            if (distance < minDistance) {
                minDistance = distance;
                minIndex = index;
            }
        }
    }
    return minIndex;
}

void AEnemyAIController::InitializeTroopers(
    const TArray<ATrooper *> &Troopers) {
    for (const auto Trooper : Troopers) {
        if (Trooper != nullptr) {
            if (Trooper->GetPlayerIndex() == AI_INDEX) {
                Trooper->SetAIPossession(this);
                PossessedTroopers.Add(Trooper);
            } else {
                PlayerTroopers.Add(Trooper);
            }
        }
    }
    InitializeSpawnPoints();
    SpawnIfNeeded();
}

void AEnemyAIController::BeginPlay() {
    Super::BeginPlay();
}

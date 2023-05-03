// Fill out your copyright notice in the Description page of Project Settings.

#include "MyGameState.h"

// #include "MyPlayerController.h"
// #include "Trooper.h"
// #include "Kismet/GameplayStatics.h"

// void AMyGameState::InitializeBattleField() const {
//     UE_LOG(LogTemp, Warning, TEXT("InitializeBattleField"));
//     FVector Location(2000.0f, -1000.0f, 0.0f);
//     FRotator Rotation(0.0f, 180.0f, 0.0f);
//
//     uint8 TrooperCount = 0;
//
//     TArray<const TCHAR *> bpPaths{
//         TEXT(
//             "Blueprint'/Game/Troopers/TrooperSkeletonMelee.TrooperSkeletonMelee_C'"
//         ),
//         TEXT("Blueprint'/Game/Troopers/TrooperWizard.TrooperWizard_C'")
//     };
//     TArray<UClass *> LoadedBpAssets;
//     for (int i = 0; i < bpPaths.Num(); ++i) {
//         TSoftClassPtr<ATrooper> ActorBpClass = TSoftClassPtr<ATrooper>(
//             FSoftObjectPath(bpPaths[i])
//         );
//         LoadedBpAssets.Push(ActorBpClass.LoadSynchronous());
//     }
//
//     for (int i = 0; i < 5; ++i) {
//         FTransform SpawnLocationAndRotation(Rotation);
//         SpawnLocationAndRotation.SetLocation(Location);
//         AActor *Spawned = GetWorld()->SpawnActorDeferred<ATrooper>(
//             LoadedBpAssets[i % 2], SpawnLocationAndRotation);
//         // AActor *Spawned = GetWorld()->SpawnActorDeferred<ATrooper>(
//         //     ATrooper::StaticClass(), SpawnLocationAndRotation);
//         dynamic_cast<ATrooper *>(Spawned)->Initialize(
//             0, Location, TrooperCount++);
//         Spawned->FinishSpawning(SpawnLocationAndRotation);
//         Spawned->SetActorLocation(Location);
//         Location += {0.f, 500.f, 0.0f};
//     }
//     Location = {-2000.0f, -1000.0f, 0.0f};
//     Rotation = {0.0f, 0.0f, 0.0f};
//     for (int i = 0; i < 5; ++i) {
//         FTransform SpawnLocationAndRotation(Rotation);
//         SpawnLocationAndRotation.SetLocation(Location);
//         AActor *Spawned = GetWorld()->SpawnActorDeferred<ATrooper>(
//             LoadedBpAssets[i % 2], SpawnLocationAndRotation);
//         // AActor *Spawned = GetWorld()->SpawnActorDeferred<ATrooper>(
//         //     ATrooper::StaticClass(), SpawnLocationAndRotation);
//         dynamic_cast<ATrooper *>(Spawned)->Initialize(
//             1, Location, TrooperCount++);
//         Spawned->FinishSpawning(SpawnLocationAndRotation);
//         Spawned->SetActorLocation(Location);
//         Location += {0.f, 500.f, 0.0f};
//     }
// }

// AMyPlayerController *AMyGameState::GetMyPlayerController(
//     uint8 const PlayerIndex) const {
//     return dynamic_cast<AMyPlayerController *>(
//         UGameplayStatics::GetPlayerController(GetWorld(), PlayerIndex));
// }
//
// void AMyGameState::StartGame() {
//     // InitializeBattleField();
//     PlayerInTurn()->StartTurn();
// }
//
// void AMyGameState::CycleTurns() {
//     PlayerInTurn()->EndTurn();
//     if (CurrentPlayerTurn == 0) {
//         CurrentPlayerTurn = 1;
//     } else {
//         CurrentPlayerTurn = 0;
//     }
//     PlayerInTurn()->StartTurn();
// }
//
// AMyPlayerController *AMyGameState::PlayerInTurn() const {
//     return GetMyPlayerController(CurrentPlayerTurn);
// }
//
// AMyPlayerController *AMyGameState::PlayerNotInTurn() const {
//     return GetMyPlayerController(CurrentPlayerTurn == 0 ? 1 : 0);
// }

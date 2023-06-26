// Fill out your copyright notice in the Description page of Project Settings.

#include "ManageSquadGameState.h"

#include "ManageSquadTrooper.h"
#include "SelectedTrooperSaveGame.h"
#include "Kismet/GameplayStatics.h"

void AManageSquadGameState::BeginPlay() {
    // USelectedTrooperSaveGame *SaveGameInstance = Cast<USelectedTrooperSaveGame>(
    //     UGameplayStatics::CreateSaveGameObject(
    //         USelectedTrooperSaveGame::StaticClass()));
    if (UGameplayStatics::DoesSaveGameExist("Selected troopers", 0)) {
        const USelectedTrooperSaveGame *SaveGameInstance = Cast<
            USelectedTrooperSaveGame>(
            UGameplayStatics::LoadGameFromSlot(TEXT("Selected troopers"), 0));
        TroopersKinds = SaveGameInstance->SelectedTroopers;
    } else {
        TroopersKinds = {0, 0, 0, 0, 0};
    }
    InitializeTroopers();
}

void AManageSquadGameState::ChangeSquad(int TrooperIndex, int TrooperKind) {
    TroopersKinds[TrooperIndex] = TrooperKind;
}

TArray<uint8> AManageSquadGameState::GetSquad() const {
    return TroopersKinds;
}

void AManageSquadGameState::InitializeTroopers() const {
    FVector Location(-1150.0f, -2850.0f, 3000.0f);
    const FRotator Rotation(0.0f, 90.0f, 0.0f);
    for (int index = 0; index < 5; ++index) {
        constexpr float DeltaX = 500.0f;
        FTransform SpawnLocationAndRotation(Rotation);
        SpawnLocationAndRotation.SetLocation(Location);
        if (TroopersBpAssets.Num() > TroopersKinds[index]) {
            AActor *Spawned = GetWorld()->SpawnActorDeferred<AManageSquadTrooper>(
                TroopersBpAssets[TroopersKinds[index]], SpawnLocationAndRotation);
            Cast<AManageSquadTrooper>(Spawned)->Initialize(index);
            Spawned->SetActorRelativeScale3D({1.5f, 1.5f, 1.5f});
            Spawned->FinishSpawning(SpawnLocationAndRotation);
            Spawned->SetActorLocation(Location);
        }
        Location += {DeltaX, 0.f, 0.0f};
    }
}

// Fill out your copyright notice in the Description page of Project Settings.

#include "MyGameState.h"

#include "MyPlayerState.h"
#include "Trooper.h"
#include "Net/UnrealNetwork.h"

auto AMyGameState::GetMyPlayerState(uint8 PlayerIndex) const {
    return Cast<AMyPlayerState>(PlayerArray[PlayerIndex]);
}

void AMyGameState::BeginPlay() {
    Super::BeginPlay();
}

void AMyGameState::AddTrooper_Implementation(ATrooper *Trooper) {
    Troopers.Add(Trooper);
}

void AMyGameState::StartGame_Implementation() const {
    PlayerNotInTurn()->SetEnemySelection(Troopers);
    PlayerInTurn()->SetEnemySelection(Troopers);
    PlayerInTurn()->StartTurn();
}

// void AMyGameState::StartGame() const {
//     PlayerNotInTurn()->SetEnemySelection(Troopers);
//     PlayerInTurn()->SetEnemySelection(Troopers);
//     PlayerInTurn()->StartTurn();
// }

void AMyGameState::CycleTurns_Implementation() {
    PlayerInTurn()->EndTurn();
    for (const auto Trooper : Troopers) {
        if (Trooper != nullptr) {
            Trooper->ResetActionPoints();
        }
    }
    CurrentPlayerTurn = !CurrentPlayerTurn;
    PlayerInTurn()->StartTurn();
}

// void AMyGameState::CycleTurns(uint8 CurrentPlayerIndex) {
//     if (CurrentPlayerTurn == CurrentPlayerIndex) {
//         PlayerInTurn()->EndTurn();
//         for (const auto Trooper : Troopers) {
//             if (Trooper != nullptr) {
//                 Trooper->ResetActionPoints();
//             }
//         }
//         CurrentPlayerTurn = !CurrentPlayerTurn;
//         PlayerInTurn()->StartTurn();
//     }
// }

AMyPlayerState *AMyGameState::PlayerInTurn() const {
    return GetMyPlayerState(CurrentPlayerTurn);
}

AMyPlayerState *AMyGameState::PlayerNotInTurn() const {
    return GetMyPlayerState(!CurrentPlayerTurn);
}

TArray<ATrooper *> AMyGameState::GetTroopers() const {
    return Troopers;
}

bool AMyGameState::IsInTurn(uint8 PlayerIndex) const {
    return PlayerIndex == CurrentPlayerTurn;
}

void AMyGameState::GetLifetimeReplicatedProps(
    TArray<FLifetimeProperty> &OutLifetimeProps) const {
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(AMyGameState, Troopers);
    DOREPLIFETIME(AMyGameState, CurrentPlayerTurn);
}

// Fill out your copyright notice in the Description page of Project Settings.

#include "MyGameState.h"

#include "MyPlayerState.h"
#include "Trooper.h"
#include "Net/UnrealNetwork.h"

auto AMyGameState::GetMyPlayerState(uint8 PlayerIndex) const {
    return Cast<AMyPlayerState>(PlayerArray[PlayerIndex]);
}

AMyGameState::AMyGameState() : Super() {
}

void AMyGameState::BeginPlay() {
    Super::BeginPlay();
    LivingTroopersCount.SetNum(2);
}

void AMyGameState::AddTrooper_Implementation(ATrooper *Trooper) {
    if (Trooper->GetPlayerIndex() >= 0 && Trooper->GetPlayerIndex() <=
        LivingTroopersCount.Num()) {
        if (LivingTroopersCount.Num() < 2) {
            LivingTroopersCount.SetNum(2);
        }
        LivingTroopersCount[Trooper->GetPlayerIndex()]++;
    }
    Troopers.Add(Trooper);
}

void AMyGameState::StartGame_Implementation() {
    // PlayerNotInTurn()->SetEnemySelection();
    PlayerInTurn()->SetEnemySelection();
    bGameStarted = true;
    PlayerInTurn()->StartTurn();
}

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

bool AMyGameState::IsGameStarted() const {
    return bGameStarted;
}

void AMyGameState::DecreaseLivingTroopers(int PlayerIndex) {
    if (bGameIsOver)
        return;
    LivingTroopersCount[PlayerIndex]--;
    if (LivingTroopersCount[PlayerIndex] <= 0) {
        UE_LOG(LogTemp, Warning, TEXT("Player %d lose!"), PlayerIndex);
        bGameIsOver = true;
    }
}


void AMyGameState::GetLifetimeReplicatedProps(
    TArray<FLifetimeProperty> &OutLifetimeProps) const {
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(AMyGameState, Troopers);
    DOREPLIFETIME(AMyGameState, CurrentPlayerTurn);
    DOREPLIFETIME(AMyGameState, bGameStarted);
}

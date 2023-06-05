// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleGameState.h"

// #include "GameOverWidget.h"
#include "BattlePlayerState.h"
#include "Trooper/Trooper.h"
#include "Blueprint/UserWidget.h"
#include "Net/UnrealNetwork.h"

auto ABattleGameState::GetMyPlayerState(uint8 PlayerIndex) const {
    return Cast<ABattlePlayerState>(PlayerArray[PlayerIndex]);
}

ABattleGameState::ABattleGameState()
    : Super() {
}

void ABattleGameState::BeginPlay() {
    Super::BeginPlay();
    LivingTroopersCount.SetNum(2);
}

void ABattleGameState::AddTrooper_Implementation(ATrooper *Trooper) {
    if (Trooper->GetPlayerIndex() >= 0 && Trooper->GetPlayerIndex() <=
        LivingTroopersCount.Num()) {
        if (LivingTroopersCount.Num() < 2) {
            LivingTroopersCount.SetNum(2);
        }
        LivingTroopersCount[Trooper->GetPlayerIndex()]++;
    }
    Troopers.Add(Trooper);
}

void ABattleGameState::StartGame_Implementation() {
    // PlayerNotInTurn()->SetEnemySelection();
    PlayerInTurn()->SetEnemySelection();
    bGameStarted = true;
    PlayerInTurn()->StartTurn();
}

void ABattleGameState::CycleTurns_Implementation() {
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

ABattlePlayerState *ABattleGameState::PlayerInTurn() const {
    return GetMyPlayerState(CurrentPlayerTurn);
}

ABattlePlayerState *ABattleGameState::PlayerNotInTurn() const {
    return GetMyPlayerState(!CurrentPlayerTurn);
}

TArray<ATrooper *> ABattleGameState::GetTroopers() const {
    return Troopers;
}

bool ABattleGameState::IsInTurn(uint8 PlayerIndex) const {
    return PlayerIndex == CurrentPlayerTurn;
}

bool ABattleGameState::IsGameStarted() const {
    return bGameStarted;
}

void ABattleGameState::DecreaseLivingTroopers(int PlayerIndex) {
    if (bGameIsOver)
        return;
    LivingTroopersCount[PlayerIndex]--;
    if (!bIsMultiplayer && PlayerIndex == 1) {
        return;
    }
    if (LivingTroopersCount[PlayerIndex] <= 0) {
        UE_LOG(LogTemp, Warning, TEXT("Player %d lose!"), PlayerIndex);
        bGameIsOver = true;
        GameOver(PlayerIndex);
    }
}

void ABattleGameState::GameOver(int PlayerIndexLose) const {
    Cast<ABattlePlayerState>(PlayerArray[0])->GameOver(PlayerIndexLose);
    if (bIsMultiplayer) {
        Cast<ABattlePlayerState>(PlayerArray[1])->GameOver(PlayerIndexLose);
    }
}

// void AMyGameState::GameOver_Implementation(int PlayerIndexLose) {
//     UGameOverWidget *CreatedWidget = CreateWidget<UGameOverWidget>(
//         GetWorld(), GameOverWidgetClass);
//     CreatedWidget->AddToViewport();
//     CreatedWidget->SetWidgetText(PlayerIndexLose != );
// }


void ABattleGameState::GetLifetimeReplicatedProps(
    TArray<FLifetimeProperty> &OutLifetimeProps) const {
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(ABattleGameState, Troopers);
    DOREPLIFETIME(ABattleGameState, CurrentPlayerTurn);
    DOREPLIFETIME(ABattleGameState, bGameStarted);
}

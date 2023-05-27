// Fill out your copyright notice in the Description page of Project Settings.

#include "SinglePlayerGS.h"
#include "Net/UnrealNetwork.h"

ASinglePlayerGS::ASinglePlayerGS()
    : Super() {
    IsMultiplayer = false;
    PrimaryActorTick.bCanEverTick = true;
    SetActorTickInterval(0.5f);
}

void ASinglePlayerGS::BeginPlay() {
    Super::BeginPlay();
    UE_LOG(LogTemp, Warning, TEXT("SinglePlayer GameState BeginPlay"));
    if (LivingTroopersCount.Num() < 2) {
        LivingTroopersCount.SetNum(2);
    }
    EnemyAiManager = GetWorld()->SpawnActor<AEnemyAIController>(
        AEnemyAIController::StaticClass(), FVector(0.0f, 0.0f, 1000.0f),
        FRotator(0.0f, 0.0f, 0.0f), FActorSpawnParameters());
    EnemyAiManager->InitializeTroopers(Troopers);
}

void ASinglePlayerGS::CycleTurns() {
    if (CurrentPlayerTurn == 0) {
        PlayerInTurn()->EndTurn();
    }
    for (const auto Trooper : Troopers) {
        if (Trooper != nullptr) {
            Trooper->ResetActionPoints();
        }
    }
    CurrentPlayerTurn = !CurrentPlayerTurn;
    if (CurrentPlayerTurn == 0) {
        PlayerInTurn()->StartTurn();
    } else {
        EnemyAiManager->StartTurn();
    }
}

void ASinglePlayerGS::Tick(float DeltaSeconds) {
    Super::Tick(DeltaSeconds);
    if (EnemyAiManager->bIsEnded) {
        EnemyAiManager->bIsEnded = false;
        CycleTurns();
    } else if (CurrentPlayerTurn == 1 && !EnemyAiManager->IsAITurn()) {
        CycleTurns();
    }
}

void ASinglePlayerGS::GetLifetimeReplicatedProps(
    TArray<FLifetimeProperty> &OutLifetimeProps) const {
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(ASinglePlayerGS, EnemyAiManager);
}

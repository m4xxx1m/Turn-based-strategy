// Fill out your copyright notice in the Description page of Project Settings.

#include "SinglePlayerGM.h"
#include "SinglePlayerGS.h"
#include "TurnBasedTutorial/BattleMode/BattlePlayerController.h"

ASinglePlayerGM::ASinglePlayerGM()
    : Super() {
    GameStateClass = ASinglePlayerGS::StaticClass();
    bIsMultiplayer = false;
}

void ASinglePlayerGM::BeginPlay() {
    Super::BeginPlay();
    UE_LOG(LogTemp, Warning, TEXT("SinglePlayer GameMode BeginPlay"));
    // GameStateClass = ASinglePlayerGS::StaticClass();
    // StartGame();
}

// void ASinglePlayerGM::PostLogin(APlayerController *NewPlayer) {
//     AGameMode::PostLogin(NewPlayer);
//     Cast<ABattlePlayerController>(NewPlayer)->
//         StartPlayingMusic(BackgroundSound);
// }

// Fill out your copyright notice in the Description page of Project Settings.

#include "ManageSquadGameState.h"

AManageSquadGameState::AManageSquadGameState() {
    TroopersKinds = {0, 0, 0, 0, 0};
}

void AManageSquadGameState::ChangeSquad(int TrooperIndex, int TrooperKind) {
    TroopersKinds[TrooperIndex] = TrooperKind;
}

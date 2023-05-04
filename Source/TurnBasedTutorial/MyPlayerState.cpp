// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerState.h"

#include "MyGameState.h"
#include "Trooper.h"
#include "Net/UnrealNetwork.h"

AMyPlayerState::AMyPlayerState()
    : Super(), bIsMyTurn(false), SelectedTrooper(nullptr) {
}

void AMyPlayerState::BeginPlay() {
    Super::BeginPlay();
}

void AMyPlayerState::SetPlayerIndex(uint8 NewPlayerIndex) {
    PlayerIndex = NewPlayerIndex;
}

bool AMyPlayerState::IsMyTurn() const {
    return bIsMyTurn;
}

void AMyPlayerState::SetMyTurn(bool bMyTurn) {
    bIsMyTurn = bMyTurn;
    if (bIsMyTurn) {
        GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green,
                                         FString::Printf(
                                             TEXT("CURRENT TURN: %d"),
                                             PlayerIndex));
    }
}

void AMyPlayerState::StartTurn_Implementation() {
    SetMyTurn(true);
    UE_LOG(LogTemp, Warning, TEXT("Your turn, %d"), PlayerIndex);
}

void AMyPlayerState::EndTurn_Implementation() {
    if (bIsMyTurn) {
        UE_LOG(LogTemp, Warning, TEXT("End Turn from player %d"), PlayerIndex);
        SetMyTurn(false);
        if (SelectedTrooper) {
            SelectedTrooper->SetSelection(false);
            SelectedTrooper = nullptr;
        }
        UE_LOG(LogTemp, Warning, TEXT("Not your turn, %d"), PlayerIndex);
        // AMyGameMode *gameMode = GetMyGameMode();
        // gameMode->CycleTurns();
        // Cast<AMyGameState>(GetWorld()->GetGameState())->CycleTurns();
    }
}

void AMyPlayerState::MoveTrooper_Implementation(
    ATrooper *Trooper,
    FVector Location) {
    if (Trooper->CheckMoveCorrectness(Location)) {
        Trooper->MoveTrooper(Location);
        // GetMyGameMode()->CycleTurns();
    } else {
        GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red,
                                         FString::Printf(
                                             TEXT("Out of move radius!")));
    }
}

void AMyPlayerState::Attack_Implementation(ATrooper *Attacker,
                                           FVector Location,
                                           int ActionIndex) {
    if (Attacker && CurrentAction >= 1 && CurrentAction <= 2 &&
        Attacker->CheckAttackCorrectness(Location, CurrentAction)) {
        Attacker->Attack(CurrentAction);
    } else {
        UE_LOG(LogTemp, Warning,
               TEXT("Out of radius or not enough Action Points"));
    }
}

void AMyPlayerState::OnPlayerAction(const FHitResult &HitResult) {
    auto const NewlySelectedLocation = HitResult.Location;
    ATrooper *NewlySelectedTrooper = Cast<ATrooper>(
        HitResult.GetActor());

    // skip re-selection
    if (SelectedTrooper == NewlySelectedTrooper) {
        UE_LOG(LogTemp, Warning, TEXT("Skip reselection"));
        return;
    }

    if (NewlySelectedTrooper == nullptr || !NewlySelectedTrooper->
        IsValidLowLevel() || NewlySelectedTrooper->GetPlayerIndex() !=
        PlayerIndex) {
        if (SelectedTrooper != nullptr && SelectedTrooper->IsValidLowLevel()) {
            switch (CurrentAction) {
                case 0:
                    UE_LOG(LogTemp, Warning, TEXT("Do move"));
                // move this mf
                    MoveTrooper(SelectedTrooper, NewlySelectedLocation);
                // and reset the selection....
                    SelectedTrooper->SetSelection(false);
                    SelectedTrooper = nullptr;
                    break;
                default:
                    // ATTACK! ATTACK!
                    UE_LOG(LogTemp, Warning, TEXT("Do attack"));
                    Attack(SelectedTrooper, NewlySelectedLocation,
                           CurrentAction);
                    SelectedTrooper->SetSelection(false);
                    SelectedTrooper = nullptr;
                    break;
            }
        }
    } else if (NewlySelectedTrooper != nullptr && NewlySelectedTrooper->
               IsValidLowLevel() && NewlySelectedTrooper->GetPlayerIndex() ==
               PlayerIndex) {
        UE_LOG(LogTemp, Warning, TEXT("Do reselect"));
        // our move, selection
        if (SelectedTrooper) {
            SelectedTrooper->SetSelection(false);
        }
        SelectedTrooper = NewlySelectedTrooper;
        SelectedTrooper->SetSelection(true);
    }
}

float AMyPlayerState::SetCurrentActionAndReturnRadius(int Action) {
    CurrentAction = Action;
    UE_LOG(LogTemp, Warning, TEXT("SetCurrentAction: %d on Player Controller "
               "with index %d"), CurrentAction, PlayerIndex);
    if (SelectedTrooper) {
        return SelectedTrooper->GetActionRadius(CurrentAction);
    }
    return 0.0f;
}

uint8 AMyPlayerState::GetPlayerIndex() {
    return PlayerIndex;
}

void AMyPlayerState::GetLifetimeReplicatedProps(
    TArray<FLifetimeProperty> &OutLifetimeProps) const {
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(AMyPlayerState, PlayerIndex);
    DOREPLIFETIME(AMyPlayerState, CurrentAction);
    DOREPLIFETIME(AMyPlayerState, bIsMyTurn);
    DOREPLIFETIME(AMyPlayerState, SelectedTrooper);
}

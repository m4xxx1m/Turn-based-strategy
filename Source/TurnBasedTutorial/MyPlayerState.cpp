// Fill out your copyright notice in the Description page of Project Settings.

#include "MyPlayerState.h"

#include "GameOverWidget.h"
#include "MyGameState.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

AMyPlayerState::AMyPlayerState()
    : Super(), bIsMyTurn(false), SelectedTrooper(nullptr) {
    // PrimaryActorTick.bCanEverTick = true;
}

void AMyPlayerState::BeginPlay() {
    Super::BeginPlay();
}

auto AMyPlayerState::GetMyGameState() const {
    return Cast<AMyGameState>(GetWorld()->GetGameState());
}

// void AMyPlayerState::Tick(float DeltaSeconds) {
//     Super::Tick(DeltaSeconds);
//     if (GetMyGameState() && GetMyGameState()->IsGameStarted()) {
//         // for (const auto Actor : Troopers) {
//         //     const auto Trooper = Cast<ATrooper>(Actor);
//         //     if (Trooper != nullptr && Trooper->GetPlayerIndex() !=
//         //         PlayerIndex) {
//         //         Trooper->HighlightAsEnemy(PlayerIndex);
//         //     }
//         // }
//         bIsSelectionInitialized = true;
//         SetActorTickEnabled(false);
//     }
// }

void AMyPlayerState::SetPlayerIndex(uint8 NewPlayerIndex) {
    PlayerIndex = NewPlayerIndex;
}

void AMyPlayerState::GameOver_Implementation(int PlayerLoseIndex) {
    UGameOverWidget *CreatedWidget = CreateWidget<UGameOverWidget>(
        GetWorld(), GameOverWidgetClass);
    CreatedWidget->AddToViewport();
    CreatedWidget->SetWidgetText(PlayerLoseIndex != PlayerIndex);
}

void AMyPlayerState::SetEnemySelection_Implementation(
    /*const TArray<AActor *> &Troopers*/) const {
    TArray<AActor *> Troopers;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(),
                                          ATrooper::StaticClass(),
                                          Troopers);
    for (const auto Actor : Troopers) {
        const auto Trooper = Cast<ATrooper>(Actor);
        if (Trooper != nullptr) {
            Trooper->HighlightAsEnemy(PlayerIndex);
        }
    }
}

void AMyPlayerState::MoveTrooper_Implementation(ATrooper *Trooper,
                                                FVector Location) {
    Location.Z = 0.0f;
    if (Trooper->CheckMoveCorrectness(Location)) {
        Trooper->MoveTrooper(Location);
        // GetMyGameMode()->CycleTurns();
    } else {
        GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red,
                                         FString::Printf(
                                             TEXT("Out of move radius!")));
    }
}

// void AMyPlayerState::Attack_Implementation(ATrooper *Attacker,
//                                            FVector Location,
//                                            int ActionIndex) {
//     if (Attacker->CheckAttackCorrectness(Location, ActionIndex)) {
//         Attacker->Attack(ActionIndex);
//     } else {
//         GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red,
//                                          FString::Printf(
//                                              TEXT(
//                                                  "Out of radius or not enough Action Points!")));
//     }
// }

void AMyPlayerState::Attack_Implementation(ATrooper *Attacker,
                                           FVector Location,
                                           int ActionIndex) {
    if (Attacker->CheckAttackCorrectness(Location, ActionIndex)) {
        Attacker->Attack(ActionIndex, Location);
        // for (const auto Trooper : Troopers) {
        //     if (Attacker->GetPlayerIndex() != Trooper->GetPlayerIndex()) {
        //         Trooper->TakeDamage(Attacker->GetAbility(ActionIndex)->Damage);
        //     }
        // }
    } else {
        GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red,
                                         FString::Printf(
                                             TEXT(
                                                 "Out of radius or not enough Action Points!")));
    }
}

// void AMyPlayerState::CycleTurns_Implementation() const {
//     if (bIsMyTurn) {
//         GetMyGameState()->CycleTurns();
//     }
// }

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
        UE_LOG(LogTemp, Warning, TEXT("End Turn from player %d"),
               PlayerIndex);
        SetMyTurn(false);
        if (SelectedTrooper) {
            SelectedTrooper->SetSelection(false, CurrentAction);
            SelectedTrooper = nullptr;
        }
        UE_LOG(LogTemp, Warning, TEXT("Not your turn, %d"), PlayerIndex);
        // AMyGameMode *gameMode = GetMyGameMode();
        // gameMode->CycleTurns();
        // Cast<AMyGameState>(GetWorld()->GetGameState())->CycleTurns();
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
        if (SelectedTrooper != nullptr && SelectedTrooper->
            IsValidLowLevel()) {
            switch (CurrentAction) {
                case 0:
                    UE_LOG(LogTemp, Warning, TEXT("Do move"));
                // move this mf
                    MoveTrooper(SelectedTrooper, NewlySelectedLocation);
                // and reset the selection....
                    SelectedTrooper->SetSelection(false, CurrentAction);
                    SelectedTrooper = nullptr;
                    break;
                default:
                    // ATTACK! ATTACK!
                    UE_LOG(LogTemp, Warning, TEXT("Do attack"));
                    Attack(SelectedTrooper, NewlySelectedLocation,
                           CurrentAction);
                    SelectedTrooper->SetSelection(false, CurrentAction);
                    SelectedTrooper = nullptr;
                    break;
            }
        }
    } else if (NewlySelectedTrooper != nullptr && NewlySelectedTrooper->
               IsValidLowLevel() && NewlySelectedTrooper->GetPlayerIndex()
               ==
               PlayerIndex) {
        UE_LOG(LogTemp, Warning, TEXT("Do reselect"));
        // our move, selection
        if (SelectedTrooper) {
            SelectedTrooper->SetSelection(false, CurrentAction);
        }
        SelectedTrooper = NewlySelectedTrooper;
        SelectedTrooper->SetSelection(true, CurrentAction);
    }
}

void AMyPlayerState::SetCurrentAction_Implementation(int Action) {
    CurrentAction = Action;
    if (SelectedTrooper) {
        SelectedTrooper->UpdateSelectionRadius(Action);
    }
    UE_LOG(LogTemp, Warning,
           TEXT("SetCurrentAction: %d on Player Controller "
               "with index %d"), CurrentAction, PlayerIndex);
}

uint8 AMyPlayerState::GetPlayerIndex() const {
    return PlayerIndex;
}

void AMyPlayerState::GetLifetimeReplicatedProps(
    TArray<FLifetimeProperty> &OutLifetimeProps) const {
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(AMyPlayerState, PlayerIndex);
    DOREPLIFETIME(AMyPlayerState, CurrentAction);
    DOREPLIFETIME(AMyPlayerState, bIsMyTurn);
    DOREPLIFETIME(AMyPlayerState, SelectedTrooper);
    DOREPLIFETIME(AMyPlayerState, bIsSelectionInitialized);
}

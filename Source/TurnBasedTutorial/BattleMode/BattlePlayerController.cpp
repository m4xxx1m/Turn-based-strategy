// Fill out your copyright notice in the Description page of Project Settings.

#include "BattlePlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "BattleGameMode.h"
#include "BattleGameState.h"
#include "BattlePlayerState.h"
#include "Blueprint/UserWidget.h"
#include "Net/UnrealNetwork.h"
#include "TurnBasedTutorial/ManageSquad/SelectedTrooperSaveGame.h"

ABattlePlayerController::ABattlePlayerController()
    : Super()/*, bIsMyTurn(false), SelectedTrooper(nullptr)*/ {
    UE_LOG(LogTemp, Warning, TEXT("Player controller created"));
    SetShowMouseCursor(true);
    PlayerIndex = 0;

    if (UGameplayStatics::DoesSaveGameExist("Selected troopers", 0)) {
        const USelectedTrooperSaveGame *SaveGameInstance = Cast<
            USelectedTrooperSaveGame>(
            UGameplayStatics::LoadGameFromSlot(TEXT("Selected troopers"), 0));
        TrooperKinds = SaveGameInstance->SelectedTroopers;
    } else {
        TrooperKinds = {0, 0, 0, 0, 0};
    }
}

void ABattlePlayerController::BeginPlay() {
    Super::BeginPlay();
    CreateBattleWidget();
}

void ABattlePlayerController::SetupInputComponent() {
    Super::SetupInputComponent();
    InputComponent->BindAction("MyAction", IE_Pressed, this,
                               &ABattlePlayerController::OnLeftMouseClick);
}

// void AMyPlayerController::SetMyTurn(bool bMyTurn) {
//     bIsMyTurn = bMyTurn;
//     if (bIsMyTurn) {
//         GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green,
//                                          FString::Printf(
//                                              TEXT("CURRENT TURN: %d"),
//                                              PlayerIndex));
//     }
// OnMyTurnChanged.ExecuteIfBound(bIsMyTurn);
// }

// void AMyPlayerController::StartTurn_Implementation() {
//     SetMyTurn(true);
//     UE_LOG(LogTemp, Warning, TEXT("Your turn, %d"), PlayerIndex);
// }

auto ABattlePlayerController::GetMyGameState() const {
    return Cast<ABattleGameState>(UGameplayStatics::GetGameState(GetWorld()));
}

auto ABattlePlayerController::GetMyPlayerState() const {
    return GetPlayerState<ABattlePlayerState>();
}


auto ABattlePlayerController::GetMyGameMode() const {
    return Cast<ABattleGameMode>(
        UGameplayStatics::GetGameMode(GetWorld()));
}


void ABattlePlayerController::EndTurn_Implementation() {
    // if (GetMyPlayerState()->IsMyTurn()) {
    //     GetMyGameState()->CycleTurns();
    // }
    // GetMyPlayerState()->CycleTurns();
    if (GetMyGameState()->IsInTurn(PlayerIndex)) {
        GetMyGameState()->CycleTurns();
    }
}

// void AMyPlayerController::EndTurn_Implementation() {
//     GetMyPlayerState()->EndTurn();
// }


//     if (bIsMyTurn) {
//         UE_LOG(LogTemp, Warning, TEXT("End Turn from player %d"), PlayerIndex);
//         SetMyTurn(false);
//         if (SelectedTrooper) {
//             SelectedTrooper->SetSelection(false);
//             SelectedTrooper = nullptr;
//         }
//         UE_LOG(LogTemp, Warning, TEXT("Not your turn, %d"), PlayerIndex);
//         AMyGameMode *gameMode = GetMyGameMode();
//         gameMode->CycleTurns();
//     }
// }

// AMyGameState *AMyPlayerController::GetMyGameState() const {
//     return dynamic_cast<AMyGameState *>(
//         GetWorld()->GetGameState());
// }

// void AMyPlayerController::MoveTrooper_Implementation(
//     ATrooper *Trooper,
//     FVector Location) {
//     if (Trooper->CheckMoveCorrectness(Location)) {
//         Trooper->MoveTrooper(Location);
//         // GetMyGameMode()->CycleTurns();
//     } else {
//         GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red,
//                                          FString::Printf(
//                                              TEXT("Out of move radius!")));
//     }
// }

// void AMyPlayerController::AttackTrooper_Implementation(
//     ATrooper *Attacker,
//     ATrooper *Victim) {
//     if (Attacker->CheckAttackCorrectness(Victim->GetLocation())) {
//         Attacker->Attack();
//         // GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red,
//         //                                  FString::Printf(
//         //                                      TEXT("ATTACK!! %d attacked %d"),
//         //                                      Attacker->GetId(),
//         //                                      Victim->GetId()));
//         // GetMyGameMode()->CycleTurns();
//     } else {
//         GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red,
//                                          FString::Printf(
//                                              TEXT(
//                                                  "Attack failed! Out of attack radius!")));
//     }
// }


// void AMyPlayerController::Attack_Implementation(ATrooper *Attacker,
//                                                 FVector Location,
//                                                 int ActionIndex) {
//     if (Attacker && CurrentAction >= 1 && CurrentAction <= 2 &&
//         Attacker->CheckAttackCorrectness(Location, CurrentAction)) {
//         Attacker->Attack(CurrentAction);
//     } else {
//         UE_LOG(LogTemp, Warning,
//                TEXT("Out of radius or not enough Action Points"));
//     }
// }

void ABattlePlayerController::SetPlayerIndex(uint8 NewPlayerIndex) {
    PlayerIndex = NewPlayerIndex;

    GetMyPlayerState()->SetPlayerIndex(NewPlayerIndex);
    // GetMyPlayerState()->PlayerIndex = NewPlayerIndex;
}

uint8 ABattlePlayerController::GetPlayerIndex() const {
    return PlayerIndex;
}

void ABattlePlayerController::CreateBattleWidget_Implementation() {
    BattleWidget = CreateWidget<UBattleUI>(
        GetWorld(), WidgetClass);
    BattleWidget->AddToViewport();
    // GetPlayerState<ABattlePlayerState>()->SetBattleWidget(BattleWidget);
    
}

void ABattlePlayerController::SetWidgetTurn_Implementation(bool bIsMyTurn) {
    if (BattleWidget) {
        BattleWidget->SetWhoseTurnText(bIsMyTurn);
    }
}

void ABattlePlayerController::StartPlayingMusic_Implementation(
    USoundBase *BackgroundSound) const {
    UGameplayStatics::PlaySound2D(GetWorld(), BackgroundSound, 0.25f);
}

// float AMyPlayerController::SetCurrentActionAndReturnRadius(int action) {
//     return GetMyPlayerState()->SetCurrentActionAndReturnRadius(action);
//
//     // CurrentAction = action;
//     // UE_LOG(LogTemp, Warning, TEXT("SetCurrentAction: %d on Player Controller "
//     //            "with index %d"), CurrentAction, PlayerIndex);
//     // if (SelectedTrooper) {
//     //     return SelectedTrooper->GetActionRadius(CurrentAction);
//     // }
//     // return 0.0f;
// }


// void AMyPlayerController::SetEnemySelection_Implementation(
//     const TArray<ATrooper *> &Troopers) const {
//     for (const auto Trooper : Troopers) {
//         if (Trooper != nullptr && Trooper->GetPlayerIndex() != PlayerIndex) {
//             Trooper->HighlightAsEnemy();
//         }
//     }
// }


// void AMyPlayerController::SetEnemySelection_Implementation() {
//     if (!GetMyGameMode()) {
//         UE_LOG(LogTemp, Warning, TEXT("Failed to get Game mode, Index: %d"), PlayerIndex);
//         return;
//     }
//     const auto &Troopers  = GetMyGameMode()->GetTroopers();
//     for (const auto Trooper : Troopers) {
//         if (Trooper != nullptr && Trooper->GetPlayerIndex() != PlayerIndex) {
//             Trooper->HighlightAsEnemy();
//         }
//     }
// }

// void AMyPlayerController::SetEnemySelection(TArray<ATrooper *> &Troopers) const {
//     for (const auto Trooper : Troopers) {
//         if (Trooper->GetPlayerIndex() != PlayerIndex) {
//             Trooper->HighlightAsEnemy();
//         }
//     }
// }

void ABattlePlayerController::GetLifetimeReplicatedProps(
    TArray<FLifetimeProperty> &OutLifetimeProps) const {
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(ABattlePlayerController, PlayerIndex);
    // DOREPLIFETIME(AMyPlayerController, CurrentAction);
    // DOREPLIFETIME(AMyPlayerController, bIsMyTurn);
    // DOREPLIFETIME(AMyPlayerController, SelectedTrooper);
}

void ABattlePlayerController::OnLeftMouseClick() {
    if (!GetMyPlayerState()->IsMyTurn()) {
        return;
    }
    UE_LOG(LogTemp, Warning, TEXT("Mouse clicked"));
    // UE_LOG(LogTemp, Warning, TEXT("Current action: %d"), CurrentAction);
    FHitResult HitResult;
    bool const IsHitResult = GetHitResultUnderCursorForObjects(
        TArray<TEnumAsByte<EObjectTypeQuery>>{ObjectTypeQuery1}, false,
        HitResult);
    // GetHitResultUnderCursorForObjects();
    // bool const IsHitResult = GetHitResultUnderCursorByChannel(
    // TraceTypeQuery1, false, HitResult);
    if (!IsHitResult)
        return;
    UE_LOG(LogTemp, Warning, TEXT("Got hit result"));
    GetMyPlayerState()->OnPlayerAction(HitResult);
    // auto const NewlySelectedLocation = HitResult.Location;
    // ATrooper *NewlySelectedTrooper = Cast<ATrooper>(
    //     HitResult.GetActor());
    //
    // // skip re-selection
    // if (SelectedTrooper == NewlySelectedTrooper) {
    //     UE_LOG(LogTemp, Warning, TEXT("Skip reselection"));
    //     return;
    // }
    //
    // if (NewlySelectedTrooper == nullptr || !NewlySelectedTrooper->
    //     IsValidLowLevel() || NewlySelectedTrooper->GetPlayerIndex() !=
    //     PlayerIndex) {
    //     // we selected something that is not a trooper (or trooper in shitty state...)
    //     // probably we should move to it if we can...
    //
    //     // UE_LOG(LogTemp, Warning, TEXT("Not a trooper"));
    //
    //     // if initial trooper is valid...
    //     if (SelectedTrooper != nullptr && SelectedTrooper->IsValidLowLevel()) {
    //         switch (CurrentAction) {
    //             case 0:
    //                 UE_LOG(LogTemp, Warning, TEXT("Do move"));
    //             // move this mf
    //                 MoveTrooper(SelectedTrooper, NewlySelectedLocation);
    //             // and reset the selection....
    //                 SelectedTrooper->SetSelection(false);
    //                 SelectedTrooper = nullptr;
    //                 break;
    //             default:
    //                 // ATTACK! ATTACK!
    //                 UE_LOG(LogTemp, Warning, TEXT("Do attack"));
    //                 Attack(SelectedTrooper, NewlySelectedLocation,
    //                        CurrentAction);
    //                 SelectedTrooper->SetSelection(false);
    //                 SelectedTrooper = nullptr;
    //                 break;
    //         }
    //     }
    // } else if (NewlySelectedTrooper != nullptr && NewlySelectedTrooper->
    //            IsValidLowLevel() && NewlySelectedTrooper->GetPlayerIndex() ==
    //            PlayerIndex) {
    //     UE_LOG(LogTemp, Warning, TEXT("Do reselect"));
    //     // our move, selection
    //     if (SelectedTrooper) {
    //         SelectedTrooper->SetSelection(false);
    //     }
    //     SelectedTrooper = NewlySelectedTrooper;
    //     SelectedTrooper->SetSelection(true);
    // }

    //
    // if (NewlySelectedTrooper == nullptr || !NewlySelectedTrooper->
    //     IsValidLowLevel()) {
    //     // we selected something that is not a trooper (or trooper in shitty state...)
    //     // probably we should move to it if we can...
    //
    //     UE_LOG(LogTemp, Warning, TEXT("Not a trooper"));
    //
    //     // if initial trooper is valid...
    //     if (SelectedTrooper != nullptr && SelectedTrooper->IsValidLowLevel()) {
    //         UE_LOG(LogTemp, Warning, TEXT("Do move"));
    //         // move this mf
    //         MoveTrooper(SelectedTrooper, NewlySelectedLocation);
    //         // and reset the selection....
    //         SelectedTrooper->SetSelection(false);
    //         SelectedTrooper = nullptr;
    //     }
    //     return;
    // }
    // UE_LOG(LogTemp, Warning, TEXT("New Selected Player Index %d"),
    //        NewlySelectedTrooper->GetPlayerIndex());
    // // skip re-selection
    // if (SelectedTrooper == NewlySelectedTrooper) {
    //     UE_LOG(LogTemp, Warning, TEXT("Skip reselection"));
    //     return;
    // }
    // we selected valid trooper...
    // if (NewlySelectedTrooper->GetPlayerIndex() == PlayerIndex) {
    //     UE_LOG(LogTemp, Warning, TEXT("Do reselect"));
    //     // our move, selection
    //     if (SelectedTrooper) {
    //         SelectedTrooper->SetSelection(false);
    //     }
    //     SelectedTrooper = NewlySelectedTrooper;
    //     SelectedTrooper->SetSelection(true);
    // } else {
    // UE_LOG(LogTemp, Warning, TEXT("Attack or skip..."));
    // // maybe selected trooper had gone crazy...
    // if (SelectedTrooper == nullptr || !SelectedTrooper->IsValidLowLevel())
    //     return;
    // UE_LOG(LogTemp, Warning, TEXT("Do attack"));
    // // ATTACK!!! ATTACK!!!!!!
    // AttackTrooper(SelectedTrooper, NewlySelectedTrooper);
    // SelectedTrooper->SetSelection(false);
    // SelectedTrooper = nullptr;
    // }
}

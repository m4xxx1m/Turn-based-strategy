// Fill out your copyright notice in the Description page of Project Settings.

#include "MyPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "MyGameMode.h"
#include "Net/UnrealNetwork.h"

AMyPlayerController::AMyPlayerController()
    : Super(), bIsMyTurn(false), SelectedTrooper(nullptr) {
    UE_LOG(LogTemp, Warning, TEXT("Player controller created"));
}

void AMyPlayerController::SetupInputComponent() {
    Super::SetupInputComponent();
    InputComponent->BindAction("MyAction", IE_Pressed, this,
                               &AMyPlayerController::OnLeftMouseClick);
}

void AMyPlayerController::SetMyTurn(bool bMyTurn) {
    bIsMyTurn = bMyTurn;
    if (bIsMyTurn) {
        GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green,
                                         FString::Printf(
                                             TEXT("CURRENT TURN: %d"),
                                             PlayerIndex));
    }
    // OnMyTurnChanged.ExecuteIfBound(bIsMyTurn);
}

void AMyPlayerController::StartTurn_Implementation() {
    SetMyTurn(true);
    UE_LOG(LogTemp, Warning, TEXT("Your turn, %d"), PlayerIndex);
}

void AMyPlayerController::EndTurn_Implementation() {
    SetMyTurn(false);
    UE_LOG(LogTemp, Warning, TEXT("Not your turn, %d"), PlayerIndex);
}

auto AMyPlayerController::GetMyGameMode() const {
    return dynamic_cast<AMyGameMode *>(
        UGameplayStatics::GetGameMode(GetWorld()));
}

void AMyPlayerController::MoveTrooper_Implementation(
    ATrooper *Trooper,
    FVector Location) {
    Trooper->MoveTrooper(Location);
    GetMyGameMode()->CycleTurns();
}


void AMyPlayerController::AttackTrooper_Implementation(
    ATrooper *Attacker,
    ATrooper *Victim) {
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red,
                                     FString::Printf(
                                         TEXT("ATTACK!! %d attacked %d"),
                                         Attacker->GetId(), Victim->GetId()));
    GetMyGameMode()->CycleTurns();
}


void AMyPlayerController::SetPlayerIndex(uint8 NewPlayerIndex) {
    PlayerIndex = NewPlayerIndex;
}

void AMyPlayerController::GetLifetimeReplicatedProps(
    TArray<FLifetimeProperty> &OutLifetimeProps) const {
    DOREPLIFETIME(AMyPlayerController, PlayerIndex);
}

void AMyPlayerController::OnLeftMouseClick() {
    if (!bIsMyTurn) {
        return;
    }
    UE_LOG(LogTemp, Warning, TEXT("Mouse clicked"));
    FHitResult HitResult;
    bool const IsHitResult = GetHitResultUnderCursorByChannel(
        TraceTypeQuery1, false, HitResult);
    if (!IsHitResult)
        return;
    UE_LOG(LogTemp, Warning, TEXT("Got hit result"));
    auto const NewlySelectedLocation = HitResult.Location;
    ATrooper *NewlySelectedTrooper = dynamic_cast<ATrooper *>(
        HitResult.GetActor());

    if (NewlySelectedTrooper == nullptr || !NewlySelectedTrooper->
        IsValidLowLevel()) {
        // we selected something that is not a trooper (or trooper in shitty state...)
        // probably we should move to it if we can...

        UE_LOG(LogTemp, Warning, TEXT("Not a trooper"));

        // if initial trooper is valid...
        if (SelectedTrooper != nullptr && SelectedTrooper->IsValidLowLevel()) {
            UE_LOG(LogTemp, Warning, TEXT("Do move"));
            // move this mf
            MoveTrooper(SelectedTrooper, NewlySelectedLocation);
            // and reset the selection....
            SelectedTrooper = nullptr;
        }
        return;
    }
    UE_LOG(LogTemp, Warning, TEXT("New Selected Player Index %d"),
           NewlySelectedTrooper->GetPlayerIndex());
    // skip re-selection
    if (SelectedTrooper == NewlySelectedTrooper) {
        UE_LOG(LogTemp, Warning, TEXT("Skip reselection"));
        return;
    }
    // we selected valid trooper...
    if (NewlySelectedTrooper->GetPlayerIndex() == PlayerIndex) {
        UE_LOG(LogTemp, Warning, TEXT("Do reselect"));
        // our move, selection
        SelectedTrooper = NewlySelectedTrooper;
    } else {
        UE_LOG(LogTemp, Warning, TEXT("Attack or skip..."));
        // maybe selected trooper had gone crazy...
        if (SelectedTrooper == nullptr || !SelectedTrooper->IsValidLowLevel())
            return;
        UE_LOG(LogTemp, Warning, TEXT("Do attack"));
        // ATTACK!!! ATTACK!!!!!!
        AttackTrooper(SelectedTrooper, NewlySelectedTrooper);
        SelectedTrooper = nullptr;
    }
}

// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleUI.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "BattlePlayerController.h"
#include "BattlePlayerState.h"


void UBattleUI::NativeConstruct() {
    Super::NativeConstruct();
    EndTurnButton->OnClicked.AddDynamic(this, &ThisClass::OnEndTurnClicked);
    ButtonAction_0->OnClicked.AddDynamic(this, &ThisClass::OnActionSwitched_0);
    ButtonAction_1->OnClicked.AddDynamic(this, &ThisClass::OnActionSwitched_1);
    ButtonAction_2->OnClicked.AddDynamic(this, &ThisClass::OnActionSwitched_2);
}

void UBattleUI::SetWidgetText_Implementation(const FString &Text) {
    InformationText->SetText(FText::FromString(Text));
}

void UBattleUI::SetWhoseTurnText_Implementation(bool IsThisPlayerTurn) {
    if (IsThisPlayerTurn) {
        SetWidgetText(TEXT("Your turn!"));
    } else {
        SetWidgetText(TEXT("Opponent's turn"));
    }
}

void UBattleUI::OnEndTurnClicked() {
    Cast<ABattlePlayerController>(GetWorld()->GetFirstPlayerController())->
        EndTurn();
}

void UBattleUI::OnActionSwitched_0() {
    ButtonAction_0->SetIsEnabled(false);
    ButtonAction_1->SetIsEnabled(true);
    ButtonAction_2->SetIsEnabled(true);
    ActionType = 0;
    OnActionSwitched();
}

void UBattleUI::OnActionSwitched_1() {
    ButtonAction_0->SetIsEnabled(true);
    ButtonAction_1->SetIsEnabled(false);
    ButtonAction_2->SetIsEnabled(true);
    ActionType = 1;
    OnActionSwitched();
}

void UBattleUI::OnActionSwitched_2() {
    ButtonAction_0->SetIsEnabled(true);
    ButtonAction_1->SetIsEnabled(true);
    ButtonAction_2->SetIsEnabled(false);
    ActionType = 2;
    OnActionSwitched();
}

void UBattleUI::OnActionSwitched() const {
    Cast<ABattlePlayerController>(GetWorld()->GetFirstPlayerController())->
        GetPlayerState<ABattlePlayerState>()->SetCurrentAction(ActionType);
}


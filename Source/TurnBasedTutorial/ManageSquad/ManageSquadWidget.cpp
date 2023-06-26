// Fill out your copyright notice in the Description page of Project Settings.

#include "ManageSquadWidget.h"

#include "ManageSquadGameState.h"
#include "SelectedTrooperSaveGame.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"


void UManageSquadWidget::NativeConstruct() {
    Super::NativeConstruct();
    BackButton->OnClicked.AddDynamic(
        this, &ThisClass::UManageSquadWidget::OnBackButtonClicked);
}

void UManageSquadWidget::OnBackButtonClicked() {
    USelectedTrooperSaveGame *SaveGameInstance = Cast<USelectedTrooperSaveGame>(
        UGameplayStatics::CreateSaveGameObject(
            USelectedTrooperSaveGame::StaticClass()));
    SaveGameInstance->SelectedTroopers = GetWorld()->GetGameState<
        AManageSquadGameState>()->GetSquad();
    UGameplayStatics::SaveGameToSlot(
        SaveGameInstance,TEXT("Selected troopers"), 0);
    UGameplayStatics::OpenLevel(GetWorld(), "MainMenuLevel");
    RemoveFromParent();
}

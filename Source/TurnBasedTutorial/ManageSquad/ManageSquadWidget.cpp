// Fill out your copyright notice in the Description page of Project Settings.

#include "ManageSquadWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"


void UManageSquadWidget::NativeConstruct() {
    Super::NativeConstruct();
    BackButton->OnClicked.AddDynamic(
        this, &ThisClass::UManageSquadWidget::OnBackButtonClicked);
}

void UManageSquadWidget::OnBackButtonClicked() {
    UGameplayStatics::OpenLevel(GetWorld(), "MainMenuLevel");
    RemoveFromParent();
}

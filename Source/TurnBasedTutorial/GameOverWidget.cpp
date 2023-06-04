// Fill out your copyright notice in the Description page of Project Settings.

#include "GameOverWidget.h"

#include "MyGameInstanceSubsystem.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"


void UGameOverWidget::NativeConstruct()
{
	ButtonToMenu->OnClicked.AddDynamic(this, &ThisClass::QuitCurrentSession);
}

void UGameOverWidget::QuitCurrentSession()
{
	const UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GetWorld());
	UMyGameInstanceSubsystem* GameInstanceSubsystem = GameInstance->GetSubsystem<UMyGameInstanceSubsystem>();
	GameInstanceSubsystem->QuitCurrentSession();
}


void UGameOverWidget::SetWidgetText_Implementation(bool HasWon)
{
	if (HasWon)
	{
		GameOverText->SetText(FText::FromString("You won!"));
	}
	else
	{
		GameOverText->SetText(FText::FromString("You lose!"));
	}
}

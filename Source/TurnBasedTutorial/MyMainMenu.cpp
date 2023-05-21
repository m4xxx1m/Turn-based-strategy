// Fill out your copyright notice in the Description page of Project Settings.


#include "MyMainMenu.h"

#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UMyMainMenu::NativeConstruct()
{
	Super::NativeConstruct();

	HostOnlineGameButton->OnClicked.AddDynamic(this, &ThisClass::UMyMainMenu::OnHostOnlineGameButtonClicked);
}	

void UMyMainMenu::OnHostOnlineGameButtonClicked()
{
	GetMyGameSubsystem()->CreateSession(2, true);
	UGameplayStatics::OpenLevel(GetWorld(), FName(TEXT("BattleFieldMap")));
	this->RemoveFromParent();
}

UMyGameInstanceSubsystem* UMyMainMenu::GetMyGameSubsystem() const
{
	const UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GetWorld());
	UMyGameInstanceSubsystem* GameInstanceSubsystem = GameInstance->GetSubsystem<UMyGameInstanceSubsystem>();
	return GameInstanceSubsystem;
}


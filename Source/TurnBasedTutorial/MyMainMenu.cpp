// Fill out your copyright notice in the Description page of Project Settings.


#include "MyMainMenu.h"

#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UMyMainMenu::NativeConstruct()
{
	Super::NativeConstruct();

	HostOnlineGameButton->OnClicked.AddDynamic(this, &ThisClass::UMyMainMenu::OnHostOnlineGameButtonClicked);

	GetMyGameSubsystem()->OnCreateSessionCompleteEvent.AddDynamic(this, &ThisClass::StartSessionWhenCreatingSessonComplete);
}	

void UMyMainMenu::OnHostOnlineGameButtonClicked()
{
	GetMyGameSubsystem()->CreateSession("Lobby " + FString::FromInt(FMath::RandRange(1, 1e6)),2, true);
}

void UMyMainMenu::StartSessionWhenCreatingSessonComplete(bool bSuccess)
{
	GetMyGameSubsystem()->StartSession();
}


UMyGameInstanceSubsystem* UMyMainMenu::GetMyGameSubsystem() const
{
	const UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GetWorld());
	UMyGameInstanceSubsystem* GameInstanceSubsystem = GameInstance->GetSubsystem<UMyGameInstanceSubsystem>();
	return GameInstanceSubsystem;
}


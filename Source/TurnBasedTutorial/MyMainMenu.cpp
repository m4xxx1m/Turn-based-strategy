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
	GetMyGameSubsystem()->CreateSession(2, true);
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


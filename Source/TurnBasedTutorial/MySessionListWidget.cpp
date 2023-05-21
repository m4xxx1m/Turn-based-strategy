// Fill out your copyright notice in the Description page of Project Settings.


#include "MySessionListWidget.h"

#include "MyGameInstanceSubsystem.h"
#include "Kismet/GameplayStatics.h"

void UMySessionListWidget::NativeConstruct()
{
	Super::NativeConstruct();

	const auto MyGameInstanceSubsystem = GetMyGameSubsystem();
	MyGameInstanceSubsystem->OnFindSessionsCompleteEvent.AddUObject(this, &ThisClass::FillSessionListFromDelegate);


	// Initiate search
	MyGameInstanceSubsystem->FindSessions(10, true);
}

void UMySessionListWidget::FillSessionListFromDelegate(const TArray<FOnlineSessionSearchResult>& SessionResults,
                                                       bool bSuccessful)
{
	UE_LOG(LogTemp, Warning, TEXT("FIND SESSION DELEGATED %d %d"), bSuccessful, SessionResults.Num());
}

UMyGameInstanceSubsystem* UMySessionListWidget::GetMyGameSubsystem() const
{
	const UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GetWorld());
	UMyGameInstanceSubsystem* GameInstanceSubsystem = GameInstance->GetSubsystem<UMyGameInstanceSubsystem>();
	return GameInstanceSubsystem;
}

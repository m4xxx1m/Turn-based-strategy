// Fill out your copyright notice in the Description page of Project Settings.


#include "MySessionListWidget.h"

#include "MyGameInstanceSubsystem.h"
#include "Components/VerticalBox.h"
#include "MySessionListEntryWidget.h"
#include "Kismet/GameplayStatics.h"

void UMySessionListWidget::NativeConstruct()
{
	Super::NativeConstruct();

	const auto MyGameInstanceSubsystem = GetMyGameSubsystem();
	MyGameInstanceSubsystem->OnFindSessionsCompleteEvent.AddUObject(this, &ThisClass::RefreshList);


	// Initiate search
	MyGameInstanceSubsystem->FindSessions(10, true);
}

void UMySessionListWidget::RefreshList(const TArray<FOnlineSessionSearchResult>& SessionResults,
                                                       bool bSuccessful)
{
	if (!bSuccessful)
	{
		UE_LOG(LogTemp, Error, TEXT("Find sessions FAILED!!!!"));
		// TODO: Mark find sessions error
		return;
	}
	SessionListBox->ClearChildren();
	for (const auto &Session: SessionResults)
	{
		auto *ItemWidget = CreateWidget<UMySessionListEntryWidget>(this, EntryClass);
		ItemWidget->Update(SessionListBox->GetChildrenCount(), Session);
		SessionListBox->AddChild(ItemWidget);
	}
}


void UMySessionListWidget::OnRefreshListButtonClicked()
{
	// TODO: Show that we started searching...
	// Initiate search
	GetMyGameSubsystem()->FindSessions(10, true);
}


UMyGameInstanceSubsystem* UMySessionListWidget::GetMyGameSubsystem() const
{
	const UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GetWorld());
	UMyGameInstanceSubsystem* GameInstanceSubsystem = GameInstance->GetSubsystem<UMyGameInstanceSubsystem>();
	return GameInstanceSubsystem;
}

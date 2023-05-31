// Fill out your copyright notice in the Description page of Project Settings.


#include "MySessionListWidget.h"

#include "MyGameInstanceSubsystem.h"
#include "Components/VerticalBox.h"
#include "MySessionListEntryWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UMySessionListWidget::NativeConstruct()
{
	Super::NativeConstruct();

	RefreshListButton->OnClicked.AddDynamic(this, &ThisClass::OnRefreshListButtonClicked);
	ConnectToSelectedSessionButton->OnClicked.AddDynamic(this, &ThisClass::ConnectToFirstSession);
	
	const auto MyGameInstanceSubsystem = GetMyGameSubsystem();
	MyGameInstanceSubsystem->OnFindSessionsCompleteEvent.AddUObject(this, &ThisClass::RefreshList);
	MyGameInstanceSubsystem->OnJoinSessionCompleteEvent.AddUObject(this, &ThisClass::OnJoinSessionSuccess);

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
	SessionListBox->ClearChildren();
	GetMyGameSubsystem()->FindSessions(10, true);
}

void UMySessionListWidget::ConnectToFirstSession()
{
	GetMyGameSubsystem()->JoinSession(0);
}


void UMySessionListWidget::OnJoinSessionSuccess(EOnJoinSessionCompleteResult::Type Result)
{
	if (Result != EOnJoinSessionCompleteResult::Success)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to connect to session!!"));
		return;
	}
	if (!GetMyGameSubsystem()->TryConnectToCurrentSession())
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to travel client to session!!"));
		return;
	}
	UE_LOG(LogTemp, Display, TEXT("Connected and travelled to session!!!"));
}


UMyGameInstanceSubsystem* UMySessionListWidget::GetMyGameSubsystem() const
{
	const UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GetWorld());
	UMyGameInstanceSubsystem* GameInstanceSubsystem = GameInstance->GetSubsystem<UMyGameInstanceSubsystem>();
	return GameInstanceSubsystem;
}

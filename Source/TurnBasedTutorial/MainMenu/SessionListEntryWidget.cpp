// Fill out your copyright notice in the Description page of Project Settings.


#include "SessionListEntryWidget.h"
#include "OnlineSessionSettings.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void USessionListEntryWidget::NativeConstruct() {
    Super::NativeConstruct();
    JoinSessionButton->OnClicked.AddDynamic(
        this, &ThisClass::OnJoinButton);
}

void USessionListEntryWidget::Update(int SessionIndex,
                                     const FOnlineSessionSearchResult &
                                     Session) {
    SessionId = SessionIndex;
    IndexText->SetText(FText::AsNumber(SessionIndex + 1));

    Session.Session.SessionSettings.Get(SETTING_MAPNAME, SessionName);
    SessionNameText->SetText(FText::FromString(SessionName));

    const int MaxPlayerCount = Session.Session.SessionSettings.
                                       NumPublicConnections;
    const int CurPlayerCount = MaxPlayerCount - Session.Session.
                               NumOpenPublicConnections;

    PlayersCountText->SetText(FText::AsNumber(CurPlayerCount));
    PingText->SetText(FText::AsNumber(Session.PingInMs));
}

void USessionListEntryWidget::OnJoinButton() {
    GetMyGameSubsystem()->JoinSession(SessionId);
}

USessionsGameInstanceSubsystem *
USessionListEntryWidget::GetMyGameSubsystem() const {
    const UGameInstance *GameInstance = UGameplayStatics::GetGameInstance(
        GetWorld());
    USessionsGameInstanceSubsystem *GameInstanceSubsystem = GameInstance->
        GetSubsystem<USessionsGameInstanceSubsystem>();
    return GameInstanceSubsystem;
}

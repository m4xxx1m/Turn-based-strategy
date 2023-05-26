// Fill out your copyright notice in the Description page of Project Settings.


#include "MySessionListEntryWidget.h"

#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Components/TextBlock.h"

void UMySessionListEntryWidget::Update(int SessionIndex, const FOnlineSessionSearchResult& Session)
{
	SessionId = SessionIndex;
	IndexText->SetText(FText::AsNumber(SessionIndex + 1));
	// TODO: SessionNameText->SetText(FText::FromString(Session.Session.SessionSettings.Get(...)))
	SessionNameText->SetText(FText::FromString("Test session name"));

	int MaxPlayerCount = Session.Session.SessionSettings.NumPublicConnections;
	int CurPlayerCount = MaxPlayerCount - Session.Session.NumOpenPublicConnections;

	PlayersCountText->SetText(FText::AsNumber(CurPlayerCount));
	PingText->SetText(FText::AsNumber(Session.PingInMs));
}

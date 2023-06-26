// Fill out your copyright notice in the Description page of Project Settings.

#include "SessionsGameInstanceSubsystem.h"

#include "BattleMode/BattlePlayerController.h"
#include "OnlineSubsystemUtils.h"
#include "GameFramework/GameModeBase.h"
#include "Kismet/GameplayStatics.h"

USessionsGameInstanceSubsystem::USessionsGameInstanceSubsystem()
    : bIsHost(false),
      CreateSessionCompleteDelegate(
          FOnCreateSessionCompleteDelegate::CreateUObject(
              this, &ThisClass::OnCreateSessionCompleted)),
      UpdateSessionCompleteDelegate(
          FOnUpdateSessionCompleteDelegate::CreateUObject(
              this, &ThisClass::OnUpdateSessionCompleted)),
      StartSessionCompleteDelegate(
          FOnStartSessionCompleteDelegate::CreateUObject(
              this, &ThisClass::OnStartSessionCompleted)),
      EndSessionCompleteDelegate(
          FOnEndSessionCompleteDelegate::CreateUObject(
              this, &ThisClass::OnEndSessionCompleted)),
      DestroySessionCompleteDelegate(
          FOnDestroySessionCompleteDelegate::CreateUObject(
              this, &ThisClass::OnDestroySessionCompleted)),
      FindSessionsCompleteDelegate(
          FOnFindSessionsCompleteDelegate::CreateUObject(
              this, &ThisClass::OnFindSessionsCompleted)),
      JoinSessionCompleteDelegate(
          FOnJoinSessionCompleteDelegate::CreateUObject(
              this, &ThisClass::OnJoinSessionCompleted)) {
}


void USessionsGameInstanceSubsystem::CreateSession(FString SessionName,
                                             int32 NumPublicConnections,
                                             bool bIsLANMatch) {
    const IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(
        GetWorld());
    if (!SessionInterface.IsValid()) {
        OnCreateSessionCompleteEvent.Broadcast(false);
        return;
    }

    LastSessionSettings = MakeShareable(new FOnlineSessionSettings());
    LastSessionSettings->NumPrivateConnections = 0;
    LastSessionSettings->NumPublicConnections = NumPublicConnections;
    LastSessionSettings->bAllowInvites = true;
    LastSessionSettings->bAllowJoinInProgress = true;
    LastSessionSettings->bAllowJoinViaPresence = true;
    LastSessionSettings->bAllowJoinViaPresenceFriendsOnly = true;
    LastSessionSettings->bUsesPresence = true;
    LastSessionSettings->bIsLANMatch = bIsLANMatch;
    LastSessionSettings->bShouldAdvertise = true;

    LastSessionSettings->Set(SETTING_MAPNAME, SessionName,
                             EOnlineDataAdvertisementType::ViaOnlineService);

    CreateSessionCompleteDelegateHandle = SessionInterface->
        AddOnCreateSessionCompleteDelegate_Handle(
            CreateSessionCompleteDelegate);

    const ULocalPlayer *localPlayer = GetWorld()->
        GetFirstLocalPlayerFromController();
    if (!SessionInterface->CreateSession(
        *localPlayer->GetPreferredUniqueNetId(), NAME_GameSession,
        *LastSessionSettings)) {
        SessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(
            CreateSessionCompleteDelegateHandle);

        OnCreateSessionCompleteEvent.Broadcast(false);
    }
}

void USessionsGameInstanceSubsystem::OnCreateSessionCompleted(
    FName SessionName,
    bool bSuccessful) {
    const IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(
        GetWorld());
    if (SessionInterface.IsValid()) {
        SessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(
            CreateSessionCompleteDelegateHandle);
    }

    bIsHost = true;

    OnCreateSessionCompleteEvent.Broadcast(bSuccessful);
}

void USessionsGameInstanceSubsystem::UpdateSession() {
    const IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(
        GetWorld());
    if (!SessionInterface.IsValid()) {
        OnCreateSessionCompleteEvent.Broadcast(false);
        return;
    }

    const TSharedPtr<FOnlineSessionSettings> UpdatedSessionSettings =
        MakeShareable(
            new FOnlineSessionSettings(*LastSessionSettings));

    // Here we can insert any changes we want
    UpdatedSessionSettings->Set(SETTING_MAPNAME, FString("Updated Level Name"),
                                EOnlineDataAdvertisementType::ViaOnlineService);

    UpdateSessionCompleteDelegateHandle =
        SessionInterface->AddOnUpdateSessionCompleteDelegate_Handle(
            UpdateSessionCompleteDelegate);

    if (!SessionInterface->UpdateSession(NAME_GameSession,
                                         *UpdatedSessionSettings)) {
        SessionInterface->ClearOnUpdateSessionCompleteDelegate_Handle(
            UpdateSessionCompleteDelegateHandle);

        OnUpdateSessionCompleteEvent.Broadcast(false);
    } else {
        LastSessionSettings = UpdatedSessionSettings;
    }
}

void USessionsGameInstanceSubsystem::OnUpdateSessionCompleted(
    FName SessionName,
    bool bSuccessful) {
    const IOnlineSessionPtr sessionInterface = Online::GetSessionInterface(
        GetWorld());
    if (sessionInterface.IsValid()) {
        sessionInterface->ClearOnUpdateSessionCompleteDelegate_Handle(
            UpdateSessionCompleteDelegateHandle);
    }

    OnUpdateSessionCompleteEvent.Broadcast(bSuccessful);
}


void USessionsGameInstanceSubsystem::StartSession() {
    const IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(
        GetWorld());
    if (!SessionInterface.IsValid()) {
        OnCreateSessionCompleteEvent.Broadcast(false);
        return;
    }
    StartSessionCompleteDelegateHandle = SessionInterface->
        AddOnStartSessionCompleteDelegate_Handle(
            StartSessionCompleteDelegate);

    if (!SessionInterface->StartSession(NAME_GameSession)) {
        SessionInterface->ClearOnStartSessionCompleteDelegate_Handle(
            StartSessionCompleteDelegateHandle);

        OnStartSessionCompleteEvent.Broadcast(false);
    }
}


void USessionsGameInstanceSubsystem::OnStartSessionCompleted(
    FName SessionName,
    bool bSuccessful) {
    const IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(
        GetWorld());
    if (SessionInterface.IsValid()) {
        SessionInterface->ClearOnStartSessionCompleteDelegate_Handle(
            StartSessionCompleteDelegateHandle);
    }
    OnStartSessionCompleteEvent.Broadcast(bSuccessful);

    // TODO: Move this from gameinstance subsystem. This should not be here.
    UGameplayStatics::OpenLevel(GetWorld(), "BattleFieldMap", true, "listen");
}


void USessionsGameInstanceSubsystem::EndSession() {
    const IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(
        GetWorld());
    if (!SessionInterface.IsValid()) {
        OnCreateSessionCompleteEvent.Broadcast(false);
        return;
    }

    EndSessionCompleteDelegateHandle = SessionInterface->
        AddOnEndSessionCompleteDelegate_Handle(
            EndSessionCompleteDelegate);

    if (!SessionInterface->EndSession(NAME_GameSession)) {
        SessionInterface->ClearOnEndSessionCompleteDelegate_Handle(
            EndSessionCompleteDelegateHandle);

        OnEndSessionCompleteEvent.Broadcast(false);
    }
}


void USessionsGameInstanceSubsystem::OnEndSessionCompleted(
    FName SessionName,
    bool bSuccessful) {
    const IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(
        GetWorld());
    if (SessionInterface.IsValid()) {
        SessionInterface->ClearOnEndSessionCompleteDelegate_Handle(
            EndSessionCompleteDelegateHandle);
    }

    OnEndSessionCompleteEvent.Broadcast(bSuccessful);
}


void USessionsGameInstanceSubsystem::DestroySession() {
    const IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(
        GetWorld());
    if (!SessionInterface.IsValid()) {
        OnDestroySessionCompleteEvent.Broadcast(false);
        return;
    }

    DestroySessionCompleteDelegateHandle = SessionInterface->
        AddOnDestroySessionCompleteDelegate_Handle(
            DestroySessionCompleteDelegate);

    if (!SessionInterface->DestroySession(NAME_GameSession)) {
        SessionInterface->ClearOnDestroySessionCompleteDelegate_Handle(
            EndSessionCompleteDelegateHandle);

        OnDestroySessionCompleteEvent.Broadcast(false);
    }
}


void USessionsGameInstanceSubsystem::OnDestroySessionCompleted(
    FName SessionName,
    bool bSuccessful) {
    const IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(
        GetWorld());
    if (SessionInterface.IsValid()) {
        SessionInterface->ClearOnDestroySessionCompleteDelegate_Handle(
            DestroySessionCompleteDelegateHandle);
    }

    OnDestroySessionCompleteEvent.Broadcast(bSuccessful);
}


void USessionsGameInstanceSubsystem::FindSessions(int32 MaxSearchResults,
                                            bool bIsLANQuery) {
    const IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(
        GetWorld());
    if (!SessionInterface.IsValid()) {
        OnFindSessionsCompleteEvent.Broadcast(
            TArray<FOnlineSessionSearchResult>(), false);
        return;
    }

    FindSessionsCompleteDelegateHandle = SessionInterface->
        AddOnFindSessionsCompleteDelegate_Handle(
            FindSessionsCompleteDelegate);

    LastSessionSearch = MakeShareable(new FOnlineSessionSearch());
    LastSessionSearch->MaxSearchResults = MaxSearchResults;
    LastSessionSearch->bIsLanQuery = bIsLANQuery;

    // Disable dedicated server search (maybe enable later, when dedicated server is implemented)
    // LastSessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

    const ULocalPlayer *LocalPlayer = GetWorld()->
        GetFirstLocalPlayerFromController();
    if (!SessionInterface->FindSessions(*LocalPlayer->GetPreferredUniqueNetId(),
                                        LastSessionSearch.ToSharedRef())) {
        SessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(
            FindSessionsCompleteDelegateHandle);

        OnFindSessionsCompleteEvent.Broadcast(
            TArray<FOnlineSessionSearchResult>(), false);
    }
}


void USessionsGameInstanceSubsystem::OnFindSessionsCompleted(bool bSuccessful) {
    const IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(
        GetWorld());
    if (SessionInterface.IsValid()) {
        SessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(
            FindSessionsCompleteDelegateHandle);
    }

    if (LastSessionSearch->SearchResults.Num() <= 0) {
        OnFindSessionsCompleteEvent.Broadcast(
            TArray<FOnlineSessionSearchResult>(), bSuccessful);
        return;
    }

    OnFindSessionsCompleteEvent.Broadcast(LastSessionSearch->SearchResults,
                                          bSuccessful);
}


void USessionsGameInstanceSubsystem::JoinSession(
    const FOnlineSessionSearchResult &SessionSearchResult) {
    const IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(
        GetWorld());
    if (!SessionInterface.IsValid()) {
        OnJoinSessionCompleteEvent.Broadcast(
            EOnJoinSessionCompleteResult::UnknownError);
        return;
    }

    JoinSessionCompleteDelegateHandle = SessionInterface->
        AddOnJoinSessionCompleteDelegate_Handle(
            JoinSessionCompleteDelegate);

    const ULocalPlayer *LocalPlayer = GetWorld()->
        GetFirstLocalPlayerFromController();
    if (!SessionInterface->JoinSession(*LocalPlayer->GetPreferredUniqueNetId(),
                                       NAME_GameSession, SessionSearchResult)) {
        SessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(
            JoinSessionCompleteDelegateHandle);

        OnJoinSessionCompleteEvent.Broadcast(
            EOnJoinSessionCompleteResult::UnknownError);
    }
}

void USessionsGameInstanceSubsystem::JoinSession(const int32 Index) {
    const IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(
        GetWorld());
    if (!SessionInterface.IsValid()) {
        OnJoinSessionCompleteEvent.Broadcast(
            EOnJoinSessionCompleteResult::UnknownError);
        return;
    }
    if (!LastSessionSearch.IsValid() || Index >= LastSessionSearch->
                                                 SearchResults.Num()) {
        OnJoinSessionCompleteEvent.Broadcast(
            EOnJoinSessionCompleteResult::UnknownError);
        return;
    }

    JoinSessionCompleteDelegateHandle = SessionInterface->
        AddOnJoinSessionCompleteDelegate_Handle(
            JoinSessionCompleteDelegate);

    const ULocalPlayer *LocalPlayer = GetWorld()->
        GetFirstLocalPlayerFromController();
    if (!SessionInterface->JoinSession(*LocalPlayer->GetPreferredUniqueNetId(),
                                       NAME_GameSession,
                                       LastSessionSearch->SearchResults[
                                           Index])) {
        SessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(
            JoinSessionCompleteDelegateHandle);

        OnJoinSessionCompleteEvent.Broadcast(
            EOnJoinSessionCompleteResult::UnknownError);
    }
}


void USessionsGameInstanceSubsystem::OnJoinSessionCompleted(
    FName SessionName,
    EOnJoinSessionCompleteResult::Type Result) {
    const IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(
        GetWorld());
    if (SessionInterface.IsValid()) {
        SessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(
            JoinSessionCompleteDelegateHandle);
    }

    OnJoinSessionCompleteEvent.Broadcast(Result);
}


bool USessionsGameInstanceSubsystem::TryConnectToCurrentSession() const {
    const IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(
        GetWorld());
    if (!SessionInterface.IsValid()) {
        return false;
    }

    FString ConnectString;
    if (!SessionInterface->GetResolvedConnectString(
        NAME_GameSession, ConnectString)) {
        return false;
    }

    APlayerController *PlayerController = GetWorld()->
        GetFirstPlayerController();
    PlayerController->ClientTravel(ConnectString, TRAVEL_Absolute);
    return true;
}

void USessionsGameInstanceSubsystem::QuitCurrentSession() {
    if (bIsHost) {
        UGameplayStatics::GetGameMode(GetWorld())->ReturnToMainMenuHost();
    } else {
        APlayerController *LocalController = GEngine->
            GetFirstLocalPlayerController(GetWorld());
        LocalController->ClientReturnToMainMenuWithTextReason(
            FText::FromString("Session ended"));
    }
    bIsHost = false;
    DestroySession();
}

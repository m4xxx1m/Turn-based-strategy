// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OnlineSessionSettings.h"

#include "Interfaces/OnlineSessionInterface.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "MyGameInstanceSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMyOnCreateSessionComplete, bool, bSuccessful);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMyOnUpdateSessionComplete, bool, bSuccessful);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMyOnStartSessionCompete, bool, bSuccessful);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMyOnEndSessionComplete, bool, bSuccessful);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMyOnDestroySessionComplete, bool, bSuccessful);

DECLARE_MULTICAST_DELEGATE_TwoParams(FMyOnFindSessionsComplete,
                                     const TArray<FOnlineSessionSearchResult>& SessionResults, bool bSuccessful);

DECLARE_MULTICAST_DELEGATE_OneParam(FMyOnJoinSessionCompele, EOnJoinSessionCompleteResult::Type Result);
/**
 * 
 */
UCLASS()
class TURNBASEDTUTORIAL_API UMyGameInstanceSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UMyGameInstanceSubsystem();

	void CreateSession(FString SessionName, int32 NumPublicConnections, bool bIsLANMatch);

	void UpdateSession();

	void StartSession();

	void EndSession();

	void DestroySession();

	void FindSessions(int32 MaxSearchResults, bool bIsLANQuery);

	void JoinSession(const FOnlineSessionSearchResult& SessionSearchResult);

	void JoinSession(const int32 Index);

	bool TryConnectToCurrentSession() const;

	void QuitCurrentSession();

	void UpdateSessionName(FString NewSessionName);
	
	FMyOnCreateSessionComplete OnCreateSessionCompleteEvent;
	FMyOnUpdateSessionComplete OnUpdateSessionCompleteEvent;
	FMyOnStartSessionCompete OnStartSessionCompleteEvent;
	FMyOnEndSessionComplete OnEndSessionCompleteEvent;
	FMyOnDestroySessionComplete OnDestroySessionCompleteEvent;
	FMyOnFindSessionsComplete OnFindSessionsCompleteEvent;
	FMyOnJoinSessionCompele OnJoinSessionCompleteEvent;

protected:
	void OnCreateSessionCompleted(FName SessionName, bool bSuccessful);

	void OnUpdateSessionCompleted(FName SessionName, bool bSuccessful);

	void OnStartSessionCompleted(FName SessionName, bool bSuccessful);

	void OnEndSessionCompleted(FName SessionName, bool bSuccessful);

	void OnDestroySessionCompleted(FName SessionName, bool bSuccessful);

	void OnFindSessionsCompleted(bool bSuccessful);

	void OnJoinSessionCompleted(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

private:
	bool bIsHost;
	
	FOnCreateSessionCompleteDelegate CreateSessionCompleteDelegate;
	FDelegateHandle CreateSessionCompleteDelegateHandle;
	TSharedPtr<FOnlineSessionSettings> LastSessionSettings;

	FOnUpdateSessionCompleteDelegate UpdateSessionCompleteDelegate;
	FDelegateHandle UpdateSessionCompleteDelegateHandle;

	FOnStartSessionCompleteDelegate StartSessionCompleteDelegate;
	FDelegateHandle StartSessionCompleteDelegateHandle;

	FOnEndSessionCompleteDelegate EndSessionCompleteDelegate;
	FDelegateHandle EndSessionCompleteDelegateHandle;

	FOnDestroySessionCompleteDelegate DestroySessionCompleteDelegate;
	FDelegateHandle DestroySessionCompleteDelegateHandle;

	FOnFindSessionsCompleteDelegate FindSessionsCompleteDelegate;
	FDelegateHandle FindSessionsCompleteDelegateHandle;
	TSharedPtr<FOnlineSessionSearch> LastSessionSearch;

	FOnJoinSessionCompleteDelegate JoinSessionCompleteDelegate;
	FDelegateHandle JoinSessionCompleteDelegateHandle;
};

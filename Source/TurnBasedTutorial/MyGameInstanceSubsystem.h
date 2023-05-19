// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Interfaces/OnlineSessionInterface.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "MyGameInstanceSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMyOnCreateSessionComplete, bool, Successful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMyOnUpdateSessionComplete, bool, Successful);

/**
 * 
 */
UCLASS()
class TURNBASEDTUTORIAL_API UMyGameInstanceSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UMyGameInstanceSubsystem();

	void CreateSession(int32 NumPublicConnections, bool bIsLANMatch);

	void UpdateSession();
	
	FMyOnCreateSessionComplete OnCreateSessionCompleteEvent;
	FMyOnUpdateSessionComplete OnUpdateSessionCompleteEvent;
	
protected:
	void OnCreateSessionCompleted(FName SessionName, bool bSuccessful);

	void OnUpdateSessionCompleted(FName SessionName, bool bSuccessful);
	
private:
	FOnCreateSessionCompleteDelegate CreateSessionCompleteDelegate;
	FDelegateHandle CreateSessionCompleteDelegateHandle;
	TSharedPtr<FOnlineSessionSettings> LastSessionSettings;

	FOnUpdateSessionCompleteDelegate UpdateSessionCompleteDelegate;
	FDelegateHandle UpdateSessionCompleteDelegateHandle;
};

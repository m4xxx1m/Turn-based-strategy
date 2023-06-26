// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../SessionsGameInstanceSubsystem.h"
#include "Blueprint/UserWidget.h"
#include "SessionListEntryWidget.generated.h"

/**
 * 
 */
UCLASS()
class TURNBASEDTUTORIAL_API USessionListEntryWidget : public UUserWidget {
    GENERATED_BODY()

public:
    virtual void NativeConstruct() override;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
    class UTextBlock *IndexText;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
    class UTextBlock *SessionNameText;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
    class UTextBlock *PlayersCountText;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
    class UTextBlock *PingText;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
    class UButton *JoinSessionButton;

    void Update(int SessionIndex, const FOnlineSessionSearchResult &Session);


protected:
    int SessionId;
    FString SessionName;

    USessionsGameInstanceSubsystem *GetMyGameSubsystem() const;

    UFUNCTION()
    void OnJoinButton();
};

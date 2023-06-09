// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../SessionsGameInstanceSubsystem.h"

#include "MainMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class TURNBASEDTUTORIAL_API UMainMenuWidget : public UUserWidget {
    GENERATED_BODY()

public:
    UPROPERTY(meta = (BindWidget))
    class UButton *HostOnlineGameButton;

    virtual void NativeConstruct() override;

protected:
    UFUNCTION()
    void OnHostOnlineGameButtonClicked();

    UFUNCTION()
    void StartSessionWhenCreatingSessionComplete(bool bSuccess);

private:
    USessionsGameInstanceSubsystem *GetMyGameSubsystem() const;
};

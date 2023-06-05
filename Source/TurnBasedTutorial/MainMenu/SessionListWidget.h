// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../SessionsGameInstanceSubsystem.h"
#include "Blueprint/UserWidget.h"
#include "SessionListWidget.generated.h"

/**
 * 
 */
UCLASS()
class TURNBASEDTUTORIAL_API USessionListWidget : public UUserWidget {
    GENERATED_BODY()

protected:
    virtual void NativeConstruct() override;

    UPROPERTY(meta = (BindWidget))
    class UButton *ConnectToSelectedSessionButton;

    UPROPERTY(meta = (BindWidget))
    class UButton *GoBackToMainMenuButton;

    UPROPERTY(meta = (BindWidget))
    class UButton *RefreshListButton;

    UPROPERTY(meta = (BindWidget))
    class UVerticalBox *SessionListBox;

    UPROPERTY(EditDefaultsOnly, Category="Session Info Class")
    TSubclassOf<class USessionListEntryWidget> EntryClass;

    void RefreshList(const TArray<FOnlineSessionSearchResult> &SessionResults,
                     bool bSuccessful);

    void OnJoinSessionSuccess(EOnJoinSessionCompleteResult::Type Result);

private:
    USessionsGameInstanceSubsystem *GetMyGameSubsystem() const;

    UFUNCTION()
    void OnRefreshListButtonClicked();

    UFUNCTION()
    void ConnectToFirstSession();
};

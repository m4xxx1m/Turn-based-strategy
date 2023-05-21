// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyGameInstanceSubsystem.h"

#include "Blueprint/UserWidget.h"
#include "MySessionListWidget.generated.h"

/**
 * 
 */
UCLASS()
class TURNBASEDTUTORIAL_API UMySessionListWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	
	UPROPERTY(meta = (BindWidget))
	class UButton* ConnectToSelectedSessionButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* GoBackToMainMenuButton;

	UPROPERTY(meta = (BindWidget))
	class UListView* AvailableSessionsList;

	void FillSessionListFromDelegate(const TArray<FOnlineSessionSearchResult>& SessionResults, bool bSuccessful);

private:
	UMyGameInstanceSubsystem* GetMyGameSubsystem() const;	
};

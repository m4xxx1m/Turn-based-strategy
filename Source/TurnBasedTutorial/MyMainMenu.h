// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyGameInstanceSubsystem.h"

#include "MyMainMenu.generated.h"

/**
 * 
 */
UCLASS()
class TURNBASEDTUTORIAL_API UMyMainMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	class UButton* HostOnlineGameButton;

	virtual void NativeConstruct() override;

protected:
	UFUNCTION()
	void OnHostOnlineGameButtonClicked();

private:
	UMyGameInstanceSubsystem* GetMyGameSubsystem() const;	
};

// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "Trooper.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class TURNBASEDTUTORIAL_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AMyPlayerController();

	void StartTurn();

	void EndTurn();

	virtual void SetupInputComponent() override;

	void SetTrooperIsMoving(bool isMoving);

private:
	bool IsMyTurn;

	bool IsThereTrooperMoving = false;

	ATrooper* SelectedTrooper;

	void OnLeftMouseClick();
};

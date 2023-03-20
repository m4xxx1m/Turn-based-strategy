// Fill out your copyright notice in the Description page of Project Settings.

#include "MyPlayerController.h"

AMyPlayerController::AMyPlayerController() : Super(), IsMyTurn(false), SelectedTrooper(nullptr)
{
	UE_LOG(LogTemp, Warning, TEXT("Player controller created"));
}

void AMyPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAction("MyAction", IE_Pressed, this,
	                           &AMyPlayerController::OnLeftMouseClick);
}

void AMyPlayerController::StartTurn()
{
	IsMyTurn = true;
	UE_LOG(LogTemp, Warning, TEXT("Your turn"));
}

void AMyPlayerController::EndTurn()
{
	IsMyTurn = false;
	UE_LOG(LogTemp, Warning, TEXT("Not your turn"));
}

void AMyPlayerController::SetTrooperIsMoving(bool isMoving)
{
	IsThereTrooperMoving = isMoving;
}

void AMyPlayerController::OnLeftMouseClick()
{
	if (IsThereTrooperMoving)
	{
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("Mouse clicked"));
	FHitResult HitResult;
	bool const IsHitResult = GetHitResultUnderCursorByChannel(TraceTypeQuery1, false, HitResult);
	if (IsHitResult)
	{
		AActor* Actor = HitResult.Actor.Get();
		if (Actor->ActorHasTag(FName("Trooper")))
		{
			ATrooper* Trooper = dynamic_cast<ATrooper*>(Actor);
			if (Trooper != nullptr && Trooper != SelectedTrooper)
			{
				if (Trooper->IsOnPlayersSide())
				{
					UE_LOG(LogTemp, Warning, TEXT("Hitted trooper id: %d, on our side"),
					       Trooper->GetId());
					SelectedTrooper = Trooper;
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("Hitted trooper id: %d, enemy"),
					       Trooper->GetId());
					UE_LOG(LogTemp, Warning, TEXT("Trooper #%d hit enemy trooper #%d"),
					       SelectedTrooper->GetId(), Trooper->GetId());
				}
			}
		}
		else if (Actor->ActorHasTag(FName("Floor")))
		{
			UE_LOG(LogTemp, Warning, TEXT("Hitted floor: %f, %f, %f"), HitResult.Location.X,
			       HitResult.Location.Y, HitResult.Location.Z);
			if (SelectedTrooper != nullptr)
			{
				SelectedTrooper->MoveTrooper(HitResult.Location);
				IsThereTrooperMoving = true;
			}
		}
	}
}

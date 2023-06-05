// Fill out your copyright notice in the Description page of Project Settings.


#include "BattlePawn.h"


// Sets default values
ABattlePawn::ABattlePawn() {
    // Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABattlePawn::BeginPlay() {
    Super::BeginPlay();
}

void ABattlePawn::MoveForward(float Val) {
    if (Val != 0.f) {
        if (Controller) {
            FRotator ControlSpaceRot = Controller->GetControlRotation();
            ControlSpaceRot.Pitch = 0;

            // transform to world space and add it
            AddMovementInput(
                FRotationMatrix(ControlSpaceRot).GetScaledAxis(EAxis::X), Val);
        }
    }
}

// Called to bind functionality to input
void ABattlePawn::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent) {
    Super::SetupPlayerInputComponent(PlayerInputComponent);
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPawn.h"


// Sets default values
AMyPawn::AMyPawn() {
    // Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AMyPawn::BeginPlay() {
    Super::BeginPlay();
}

void AMyPawn::MoveForward(float Val) {
    if (Val != 0.f)
    {
        if (Controller)
        {
            FRotator ControlSpaceRot = Controller->GetControlRotation();
            ControlSpaceRot.Pitch = 0;

            // transform to world space and add it
            AddMovementInput( FRotationMatrix(ControlSpaceRot).GetScaledAxis( EAxis::X ), Val );
        }
    }
}

// Called every frame
void AMyPawn::Tick(float DeltaTime) {
    Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AMyPawn::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent) {
    Super::SetupPlayerInputComponent(PlayerInputComponent);
}

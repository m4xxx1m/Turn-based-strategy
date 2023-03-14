
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Trooper.generated.h"

UCLASS()
class TURNBASEDTUTORIAL_API ATrooper : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	ATrooper();

protected:
	static int NumberOfTroopersForId;

	virtual void BeginPlay() override;

	virtual void Tick(float deltaTime) override;

	UPROPERTY()
	FVector Position;

	UPROPERTY()
	bool OnPlayersSide;

	UPROPERTY()
	int Id;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* Mesh;

	UPROPERTY()
	float Speed = 300.0f;

	bool IsMoving = false;

	FVector MoveToVector;

public:
	void MoveTrooper(FVector newPos);

	static void InitNumberOfTroopersForId();

	FVector GetPosition();

	bool IsOnPlayersSide();

	int GetId();

	void InitTrooper(FVector position, bool onPlayersSide);
};

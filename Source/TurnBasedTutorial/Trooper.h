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
	static uint8 NumberOfTroopersForId;

	virtual void BeginPlay() override;

	virtual void Tick(float const DeltaTime) override;

	UPROPERTY()
	FVector Position;

	UPROPERTY()
	bool bOnPlayersSide;

	UPROPERTY()
	uint8 Id;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* Mesh;

	UPROPERTY()
	float Speed = 300.0f;

	bool bIsMoving = false;

	FVector MoveToVector;

public:
	void MoveTrooper(FVector const NewPos);

	static void InitNumberOfTroopersForId();

	FVector GetPosition() const;

	bool IsOnPlayersSide() const;

	uint8 GetId() const;

	void InitTrooper(FVector const NewPosition, bool const bIsOnPlayersSide);
};

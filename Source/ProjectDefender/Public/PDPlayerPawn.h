// (c) Kushagra, 2022

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PDPlayerPawn.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class USpringArmComponent;
class UCameraComponent;

UCLASS(Abstract, Blueprintable)
class PROJECTDEFENDER_API APDPlayerPawn : public APawn
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category="Components", meta=(AllowPrivateAccess="true"))
	USphereComponent* CollisionComponent;

	UPROPERTY(EditAnywhere, Category="Components", meta=(AllowPrivateAccess="true"))
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(EditAnywhere, Category="Components", meta=(AllowPrivateAccess="true"))
	UStaticMeshComponent* MidMesh;
	
	UPROPERTY(EditAnywhere, Category="Components", meta=(AllowPrivateAccess="true"))
	UStaticMeshComponent* EndMesh;
	
	UPROPERTY(EditAnywhere, Category="Components", meta=(AllowPrivateAccess="true"))
	USpringArmComponent* CameraBoom;

	UPROPERTY(EditAnywhere, Category="Components", meta=(AllowPrivateAccess="true"))
	UCameraComponent* LookCamera;
	
public:
	// Sets default values for this pawn's properties
	APDPlayerPawn();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Turret|Controls")
	void TurnRight(float Value);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Turret|Controls")
	void LookUp(float Value);
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};

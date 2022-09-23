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
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess="true"))
	USphereComponent* CollisionComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess="true"))
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess="true"))
	UStaticMeshComponent* MidMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess="true"))
	UStaticMeshComponent* EndMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess="true"))
	USpringArmComponent* CameraBoom;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess="true"))
	UCameraComponent* LookCamera;
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Turret|Camera")
	bool bEnableCameraEdgeRotation;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Turret|Camera", meta=(ClampMin="0.0", ClampMax="1.0", EditCondition="bEnableCameraEdgeRotation"))
	float ScreenHospotZone;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Turret|Controls|Turn")
	bool bCanTurnRight;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Turret|Controls|Turn", meta=(EditCondition="bCanTurnRight"))
	bool bClampTurnRotation;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Turret|Controls|Turn", meta=(EditCondition="bClampTurnRotation"))
	FVector2D TurnRotationLimits;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Turret|Controls|Look")
	bool bCanLookUp;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Turret|Controls|Look", meta=(EditCondition="bCanLookUp"))
	bool bClampLookRotation;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Turret|Controls|Look", meta=(EditCondition="bClampLookRotation"))
	FVector2D LookRotationLimits;
	
	// Sets default values for this pawn's properties
	APDPlayerPawn();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Turret|Controls")
	void TurnRight(float Value);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Turret|Controls")
	void LookUp(float Value);

private:
	void CameraEdgeRotation();
	const FRotator GetCameraBoomYawRotation(float RotationSpeed);
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};

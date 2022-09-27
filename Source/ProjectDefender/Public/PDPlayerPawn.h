// (c) Kushagra, 2022

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PDPlayerPawn.generated.h"

UENUM(BlueprintType, meta=(Bitflags, UseEnumValuesAsMaskValuesInEditor="true"))
enum class EFireMode: uint8
{
	None = 0 UMETA(Hidden),
	Single = 1 << 0 UMETA(DisplayName = "Single"),
	Burst = 1 << 1 UMETA(DisplayName = "Burst"),
	Auto = 1 << 2 UMETA(DisplayName = "Auto"),
	
};

UENUM(BlueprintType, meta=(Bitflags, UseEnumValuesAsMaskValuesInEditor="true"))
enum class EFireType: uint8
{
	None = 0 UMETA(Hidden),
	Projectile = 1 << 0 UMETA(DisplayName = "Projectile"),
	Raycast = 1 << 1 UMETA(DisplayName = "Hitscan")
};

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

	FTimerHandle TimerHandle_FireMode_Auto;
	FTimerHandle TimerHandle_FireMode_Burst;
	uint8 TempBurstFireCount;
	
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

	// TODO Use BitFlags to Enable FireModes
	// UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Turret|General", meta=(Bitmask, BitmaskEnum=EFireMode))
	// uint8 FireModes;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Turret|General")
	EFireType FireType;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Turret|General", DisplayName="Fire Mode")
	EFireMode CurrentFireMode;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Turret|General", meta=(ClampMin="0.0", EditCondition="CurrentFireMode==EFireMode::Auto"))
	float AutoFireDuration;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Turret|General", meta=(ClampMin="0", EditCondition="CurrentFireMode==EFireMode::Burst"))
	uint8 BurstFireCount;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Turret|General", meta=(ClampMin="0.0", EditCondition="CurrentFireMode==EFireMode::Burst"))
	float BurstFireDuration;
	
	// Sets default values for this pawn's properties
	APDPlayerPawn();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Turret|Controls")
	void TurnRight(float Value);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Turret|Controls")
	void LookUp(float Value);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Turret|Firing")
	void PrimaryAttack_Pressed();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Turret|Firing")
	void PrimaryAttack_Released();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Turret|Firing")
	void ProjectileFire();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Turret|Firing")
	void HitscanFire();

private:
	void CameraEdgeRotation();
	FRotator GetCameraBoomYawRotation(float RotationSpeed) const;

	void FireWeapon();

	void FireMode_Auto();
	void FireMode_Burst();
	void ChangeFireMode();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};

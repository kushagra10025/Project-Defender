// (c) Kushagra, 2022

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PDPracticeTargetBase.generated.h"

class UStaticMeshComponent;
class USceneComponent;

UCLASS(Abstract)
class PROJECTDEFENDER_API APDPracticeTargetBase : public AActor
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess="true"))
	USceneComponent* DefaultRootComponent;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess="true"))
	UStaticMeshComponent* TargetBase;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess="true"))
	UStaticMeshComponent* Target;

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Target")
	void OnTargetHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
public:	
	// Sets default values for this actor's properties
	APDPracticeTargetBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

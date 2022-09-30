// (c) Kushagra, 2022


#include "PDPracticeTargetBase.h"

#include "Components/StaticMeshComponent.h"

// Sets default values
APDPracticeTargetBase::APDPracticeTargetBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DefaultRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent = DefaultRootComponent;

	TargetBase = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TargetBase"));
	TargetBase->SetupAttachment(RootComponent);

	Target = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Target"));
	Target->SetupAttachment(RootComponent);
}

void APDPracticeTargetBase::OnTargetHit_Implementation(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Something Hit"));
}

// Called when the game starts or when spawned
void APDPracticeTargetBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void APDPracticeTargetBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	Target->OnComponentHit.AddDynamic(this, &APDPracticeTargetBase::OnTargetHit);
}

// Called every frame
void APDPracticeTargetBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


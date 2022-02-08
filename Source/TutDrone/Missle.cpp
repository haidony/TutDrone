// Fill out your copyright notice in the Description page of Project Settings.


#include "Missle.h"
#include <Components/StaticMeshComponent.h>
#include <GameFramework/ProjectileMovementComponent.h>
#include "Enemy.h"
#include "TutDroneGameModeBase.h"

// Sets default values
AMissle::AMissle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    RootComponent = Mesh;
    Mesh->SetGenerateOverlapEvents(true);
    Mesh->OnComponentBeginOverlap.AddDynamic(this, &AMissle::OverlapHandler);

    MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MoveComp"));
    MovementComp->InitialSpeed = 3000.0f;

}

void AMissle::OverlapHandler(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
    if (OtherActor->IsA<AEnemy>())
    {
        OtherActor->Destroy();
        ATutDroneGameModeBase* gm = Cast<ATutDroneGameModeBase>(GetWorld()->GetAuthGameMode());
        if (gm != nullptr)
        {
            gm->points++;
        }
        this->Destroy();
    }
}

// Called when the game starts or when spawned
void AMissle::BeginPlay()
{
	Super::BeginPlay();

    this->SetLifeSpan(8.0f);
}

// Called every frame
void AMissle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


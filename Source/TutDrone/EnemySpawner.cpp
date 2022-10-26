// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawner.h"
#include <Components/BoxComponent.h>
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include <TimerManager.h>
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AEnemySpawner::AEnemySpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    SpawnArea = CreateDefaultSubobject<UBoxComponent>(TEXT("OutCollision"));
    RootComponent = SpawnArea;
    SpawnArea->SetBoxExtent(FVector(3000.0f, 3000.0f, 100.0f));

}

void AEnemySpawner::SpawnEnemy() const
{
    FVector Origin;
    FVector Extends;
    GetActorBounds(false, Origin, Extends);
    FVector SpawnPoint = UKismetMathLibrary::RandomPointInBoundingBox(Origin, Extends);
    GetWorld()->SpawnActor<AEnemy>(EnemyType, SpawnPoint, FRotator::ZeroRotator);
}

// Called when the game starts or when spawned
void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();
    GetWorld()->GetTimerManager().SetTimer(SpawnHandle, this, &AEnemySpawner::SpawnEnemy, spawnSpan, true);
}

// Called every frame
void AEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


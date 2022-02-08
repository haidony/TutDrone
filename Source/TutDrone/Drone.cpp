#include "Drone.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "PhysicsEngine/PhysicsThrusterComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include <Components/InputComponent.h>
#include <Kismet/GameplayStatics.h>


ADrone::ADrone()
{
	PrimaryActorTick.bCanEverTick = true;

    OutCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("OutCollision"));
    RootComponent = OutCollision;
    OutCollision->SetBoxExtent(FVector(100.0f, 100.0f, 100.0f));
    OutCollision->SetSimulatePhysics(true);
    OutCollision->BodyInstance.bLockXRotation = true;
    OutCollision->BodyInstance.bLockYRotation = true;

    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    Mesh->SetupAttachment(OutCollision);
    Paddle1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Paddle1"));
    Paddle2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Paddle2"));
    Paddle3 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Paddle3"));
    Paddle4 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Paddle4"));
    Paddle1->SetupAttachment(Mesh, TEXT("Paddle1"));
    Paddle2->SetupAttachment(Mesh, TEXT("Paddle2"));
    Paddle3->SetupAttachment(Mesh, TEXT("Paddle3"));
    Paddle4->SetupAttachment(Mesh, TEXT("Paddle4"));

    UpThruster = CreateDefaultSubobject<UPhysicsThrusterComponent>(TEXT("UpThruster"));
    UpThruster->SetupAttachment(RootComponent);
    UpThruster->ThrustStrength = 980.0f;
    UpThruster->SetAutoActivate(true);
    UpThruster->SetWorldRotation(UKismetMathLibrary::MakeRotFromX(-this->GetActorUpVector()));

    ForwardThruster = CreateDefaultSubobject<UPhysicsThrusterComponent>(TEXT("ForwardThruster"));
    ForwardThruster->SetupAttachment(RootComponent);
    ForwardThruster->ThrustStrength = 0.0f;
    ForwardThruster->SetAutoActivate(true);
    ForwardThruster->SetWorldRotation(UKismetMathLibrary::MakeRotFromX(-this->GetActorForwardVector()));
}

void ADrone::BeginPlay()
{
	Super::BeginPlay();
	
}

void ADrone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    if (GetInputAxisValue(TEXT("Lift")) == 0.0f)
    {
        UpThruster->ThrustStrength = 980.0f;
    }
    if (GetInputAxisValue(TEXT("Turn")) == 0.0f)
    {
        OutCollision->AddTorqueInDegrees(FVector::ZeroVector);
    }

    if (GetInputAxisValue(TEXT("Forward")) == 0.0f)
    {
        ForwardThruster->ThrustStrength = 0.0f;

        if (Mesh->GetRelativeRotation().Pitch != 0.0f)
        {
            float currentPitch = Mesh->GetRelativeRotation().Pitch;

            Mesh->AddRelativeRotation(FRotator(-currentPitch * DeltaTime, 0.0f, 0.0f));
            if (FMath::Abs(Mesh->GetRelativeRotation().Pitch) <= KINDA_SMALL_NUMBER)
            {
                Mesh->SetRelativeRotation(FRotator(.0f, .0f, .0f));
            }
        }
    }
    else if (ForwardThruster->ThrustStrength >= ForwardThrustNearMax)
    {
        if (Mesh->GetRelativeRotation().Pitch != 0.0f)
        {
            float currentPitch = Mesh->GetRelativeRotation().Pitch;
            Mesh->AddRelativeRotation(FRotator(-currentPitch * DeltaTime, 0.0f, 0.0f));
            if (FMath::Abs(Mesh->GetRelativeRotation().Pitch) <= KINDA_SMALL_NUMBER)
            {
                Mesh->SetRelativeRotation(FRotator(.0f, .0f, .0f));
            }
        }
    }
    RotatePaddles(DeltaTime);
}

void ADrone::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAxis(TEXT("Lift"), this, &ADrone::Lift);
    PlayerInputComponent->BindAxis(TEXT("Forward"), this, &ADrone::Forward);
    PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ADrone::Turn);
    PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &ADrone::DoFire);
}


void ADrone::Lift(float val)
{
    UpThruster->ThrustStrength += val * LiftAcc * GetWorld()->DeltaTimeSeconds;
    UpThruster->ThrustStrength = FMath::Clamp(UpThruster->ThrustStrength, -LiftThrustMax, LiftThrustMax);
}


void ADrone::Forward(float val)
{
    ForwardThruster->ThrustStrength += val * ForwardAcc * GetWorld()->DeltaTimeSeconds;
    ForwardThruster->ThrustStrength = FMath::Clamp(ForwardThruster->ThrustStrength, -ForwardThrustMax, ForwardThrustMax);

    float pitch = Mesh->GetRelativeRotation().Pitch;
    if (FMath::Abs(pitch) <= 15.0f && ForwardThruster->ThrustStrength < ForwardThrustNearMax)
    {
        Mesh->AddRelativeRotation(FRotator(-20.0f * GetWorld()->DeltaTimeSeconds * val, 0.0f, 0.0f));
    }
}


void ADrone::Turn(float val)
{
    OutCollision->AddTorqueInDegrees(this->GetActorUpVector() * val * TurnStrength);
}

void ADrone::RotatePaddles(float Delta)
{
    Paddle1->AddRelativeRotation(FRotator(0.0f, Delta * PaddleRotateSpeed, 0.0f));
    Paddle2->AddRelativeRotation(FRotator(0.0f, Delta * PaddleRotateSpeed, 0.0f));
    Paddle3->AddRelativeRotation(FRotator(0.0f, Delta * PaddleRotateSpeed, 0.0f));
    Paddle4->AddRelativeRotation(FRotator(0.0f, Delta * PaddleRotateSpeed, 0.0f));
}

void ADrone::DoFire()
{
    FTransform firingpoint = Mesh->GetSocketTransform(TEXT("FiringSocket"));
    GetWorld()->SpawnActor<AMissle>(Bullet, firingpoint);

    UGameplayStatics::PlaySoundAtLocation(this, FireSound, this->GetActorLocation(), 2.0f);
}
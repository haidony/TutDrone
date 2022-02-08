#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Missle.h"
#include "Drone.generated.h"

UCLASS()
class TUTDRONE_API ADrone : public APawn
{
    GENERATED_BODY()
    UPROPERTY(VisibleAnywhere)              //Drone 模块
        class UBoxComponent* OutCollision;
    UPROPERTY(VisibleAnywhere)
        class UStaticMeshComponent *Mesh;
    UPROPERTY(VisibleAnywhere)
        class UStaticMeshComponent *Paddle1;
    UPROPERTY(VisibleAnywhere)
        class UStaticMeshComponent *Paddle2;
    UPROPERTY(VisibleAnywhere)
        class UStaticMeshComponent *Paddle3;
    UPROPERTY(VisibleAnywhere)
        class UStaticMeshComponent *Paddle4;

    UPROPERTY(VisibleAnywhere)              //动力模块
        class UPhysicsThrusterComponent *UpThruster;
    UPROPERTY(VisibleAnywhere)                  
        class UPhysicsThrusterComponent *ForwardThruster;

                                            //飞行受力模块
    UPROPERTY(EditAnywhere/*, BlueprintReadWrite*/, Category = "Init")      //升降限速
        float LiftAcc = 10000.0f;
    UPROPERTY(EditAnywhere/*, BlueprintReadWrite*/, Category = "Init")
        float LiftThrustMax = 2000.0f;

    UPROPERTY(EditAnywhere/*, BlueprintReadWrite*/, Category = "Init")      //前进限速
        float ForwardAcc = 500.0f;
    UPROPERTY(EditAnywhere/*, BlueprintReadWrite*/, Category = "Init")
        float ForwardThrustMax = 2000.0f;
    UPROPERTY(EditAnywhere/*, BlueprintReadWrite*/, Category = "Init")
        float ForwardThrustNearMax = 1900.0f;

    UPROPERTY(EditAnywhere/*, BlueprintReadWrite*/, Category = "Init")      //旋转速度
        float TurnStrength = 500000.0f;

    UPROPERTY(EditAnywhere/*, BlueprintReadWrite*/, Category = "Init")      //paddle旋转速度
        float PaddleRotateSpeed = 3000.0f;

    UPROPERTY(EditAnywhere)
        TSubclassOf<AMissle> Bullet;

    UPROPERTY(EditAnywhere)
        class USoundBase *FireSound;
    UFUNCTION()
        void DoFire();

public:
	ADrone();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
    UFUNCTION()
        void Lift(float val);
    UFUNCTION()
        void Forward(float val);
    UFUNCTION()
        void Turn(float val);
    //Tarray<UStaticMeshComponent*> Paddles
    void RotatePaddles(float Delta);
};

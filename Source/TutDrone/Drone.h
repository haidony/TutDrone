#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Missle.h"
#include "Drone.generated.h"

UCLASS()
class TUTDRONE_API ADrone : public APawn
{
    GENERATED_BODY()
    UPROPERTY(VisibleAnywhere)              //Drone ģ��
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

    UPROPERTY(VisibleAnywhere)              //����ģ��
        class UPhysicsThrusterComponent *UpThruster;
    UPROPERTY(VisibleAnywhere)                  
        class UPhysicsThrusterComponent *ForwardThruster;

                                            //��������ģ��
    UPROPERTY(EditAnywhere/*, BlueprintReadWrite*/, Category = "Init")      //��������
        float LiftAcc = 10000.0f;
    UPROPERTY(EditAnywhere/*, BlueprintReadWrite*/, Category = "Init")
        float LiftThrustMax = 2000.0f;

    UPROPERTY(EditAnywhere/*, BlueprintReadWrite*/, Category = "Init")      //ǰ������
        float ForwardAcc = 500.0f;
    UPROPERTY(EditAnywhere/*, BlueprintReadWrite*/, Category = "Init")
        float ForwardThrustMax = 2000.0f;
    UPROPERTY(EditAnywhere/*, BlueprintReadWrite*/, Category = "Init")
        float ForwardThrustNearMax = 1900.0f;

    UPROPERTY(EditAnywhere/*, BlueprintReadWrite*/, Category = "Init")      //��ת�ٶ�
        float TurnStrength = 500000.0f;

    UPROPERTY(EditAnywhere/*, BlueprintReadWrite*/, Category = "Init")      //paddle��ת�ٶ�
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

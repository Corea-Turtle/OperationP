// Fill out your copyright notice in the Description page of Project Settings.


#include "Champion/Yasuo/OPYasuoWindWall.h"
#include "Champion/OPChampion.h"
#include "Components/CapsuleComponent.h"
#include "Diavolo/OPDiavolo.h"
#include "Projectile/OPProjectile.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"


// Called when the game starts or when spawned
void AOPYasuoWindWall::BeginPlay()
{
	Super::BeginPlay();

	OPProjectileMovementComponent->Velocity = InitialSpeed * GetActorForwardVector();
	GetWorldTimerManager().SetTimer(DestroyTimer, this, &AOPYasuoWindWall::DestroyProjectile, 4.f, false);
	GetWorldTimerManager().SetTimer(SpeedChangeTimer, this, &AOPYasuoWindWall::ChangeSpeed, 0.25f, false);
	
	// if (OPProjectileMovementComponent)
	// {
	// 	StartLocation = GetActorLocation();
	//
	// 	// Spawn Niagara effect
	// 	if (ProjectileEffect)
	// 	{
	// 		ProjectileNiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(
	// 			ProjectileEffect,
	// 			RootComponent,
	// 			NAME_None,
	// 			FVector::ZeroVector,
	// 			FRotator::ZeroRotator,
	// 			EAttachLocation::KeepRelativeOffset,
	// 			true
	// 		);
	// 	}
	// }
}

// Called every frame
void AOPYasuoWindWall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/* Tick�� �޸𸮸� ���� ��ƸԾ, �Ʒ� �κ��� BeginPlay���� Timer�� ���ֽô°� �����ϴ�! */
	
	// float DistanceTravelled = FVector::Dist(StartLocation, GetActorLocation());
	// if (DistanceTravelled >= DistanceToStop)
	// {
	// 	StopProjectile();
	// }
}

void AOPYasuoWindWall::StopProjectile()
{
	// OPProjectileMovementComponent->StopMovementImmediately();
	// Detach the Niagara component to stop it from updating its position
	// if (ProjectileNiagaraComponent)
	// {
	// 	ProjectileNiagaraComponent->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	// }
	//
	// GetWorldTimerManager().SetTimer(DestroyTimerHandle, this, &AOPYasuoWindWall::DestroyProjectile, StopDuration, false);
}

void AOPYasuoWindWall::DestroyProjectile()
{
	Destroy();
}

void AOPYasuoWindWall::ChangeSpeed()
{
	OPProjectileMovementComponent->Velocity = FinalSpeed * GetActorForwardVector();
}

void AOPYasuoWindWall::OnDamageCollisionBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnDamageCollisionBeginOverlap(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	if (OtherActor && OtherActor != this && GetOwner() != nullptr && OtherActor != GetOwner())
	{
		AOPProjectile* OtherProjectile = Cast<AOPProjectile>(OtherActor);
		if (OtherProjectile)
		{
			FVector ImpactDirection = (OtherProjectile->GetActorLocation() - SweepResult.ImpactPoint).GetSafeNormal();
			ImpactDirection.Z += 1.0f; // ��� ���⿡ ���� ���� �߰�
			ImpactDirection = ImpactDirection.GetSafeNormal();


			UProjectileMovementComponent* ProjectileMovement = OtherProjectile->FindComponentByClass<UProjectileMovementComponent>();
			if (ProjectileMovement)
			{
				ProjectileMovement->Velocity = ImpactDirection * WindWallForce; // ����ü�� �ӵ��� ��� �������� ����
			}
		}
		//DestroyProjectile();
	}
}

void AOPYasuoWindWall::InitProjectile(FVector Direction, float Speed)
{
	Direction.Z = 0;
	
	// �ʱ� �ӵ��� �ùٸ��� �����Ǿ����� Ȯ��
	// if (OPProjectileMovementComponent)
	// {
	//
	// 	OPProjectileMovementComponent->InitialSpeed = Speed;
	// 	OPProjectileMovementComponent->MaxSpeed = Speed;
	// 	OPProjectileMovementComponent->Velocity = Direction * Speed;
	//
	// }
	//
	//
	// // ĳ���ͺ��� ��¦ �տ��� �߻�ü�� �����ǵ��� ����
	// SetActorLocation(GetActorLocation() + Direction * 100.f);
}
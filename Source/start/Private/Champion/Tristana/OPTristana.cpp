// Fill out your copyright notice in the Description page of Project Settings.


#include "Champion/Tristana/OPTristana.h"
#include "Animation/OPAnimInstance.h"
#include "Components/CapsuleComponent.h"
#include "Diavolo/OPDiavolo.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Player/OPPlayerController.h"
#include "Champion/Tristana/OPTristanaCannonBall.h"
#include "Champion/Tristana/OPTristanaBusterShot.h"
#include "Champion/Tristana/OPTristanaExplosiveCharge.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Engine/SkeletalMeshSocket.h"

AOPTristana::AOPTristana()
{
	PrimaryActorTick.bCanEverTick = true;
	
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement Component")); // ����ü ������ �����Ϳ� ���� �Ҵ�

	CannonBallSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("CannonBall SpawnPoint"));
	CannonBallSpawnPoint->SetupAttachment(GetMesh(), FName(TEXT("CannonSocket")));

	RapidFireNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("RapidFire Niagara"));
	RapidFireNiagaraComponent->SetupAttachment(GetMesh(), FName(TEXT("CannonSocket")));
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
}

void AOPTristana::BeginPlay()
{
	Super::BeginPlay();

	DefaultAttackSpeed = BasicAttackCooltime;

	Skill_2_LongJumpRange = Skill_2_MaxJumpRange;

	if (RapidFireNiagaraComponent)
	{
		RapidFireNiagaraComponent->SetVisibility(false);
	}
}

void AOPTristana::Tick(float DeltaTime)
{
}

void AOPTristana::Passive()
{

}

void AOPTristana::BasicAttack()
{
	Super::BasicAttack();

	if (!bBasicAttack) return;
	if (OPPlayerController == nullptr) return;

	OPPlayerController->GetHitResultUnderCursor(ECC_Visibility, false, MouseCursorHit);
	if (!MouseCursorHit.bBlockingHit) return;
	TurnCharacterToCursor(MouseCursorHit);

	GetWorldTimerManager().SetTimer(CannonBallSpawnTimer, this, &AOPTristana::BasicAttack_CannonBall, 0.25f, false);

	if (ChampionAnimInstance && BasicAttackAnimMontage)
	{
		ChampionAnimInstance->Montage_Play(BasicAttackAnimMontage, 1.0f);
	}
	
	StopChampionMovement();
    GetWorldTimerManager().SetTimer(ResetMovementTimerHandle, this, &AOPTristana::ResetChampionMovement, 0.7f, false);

	SetbBasicAttack_False();
	GetWorldTimerManager().SetTimer(BasicAttackCooltimeTimerHandle, this, &AOPTristana::SetbBasicAttack_True, BasicAttackCooltime, false);
	//GetWorldTimerManager().SetTimer(MeleeAttackCooltimeTimer, this, &AOPTristana::SetbMeleeAttack, GetMeleeAttackCooltime(), false); //�̺κ��� �� SetTimer�� �������� �������
	
	
	// FVector Start = GetActorLocation();
	// FVector TestDiavoloLocation = TestDiavolo->GetActorLocation();
	//
	// float DistanceToTarget = CalculateMinDistanceToActorEdge(Start, TestDiavoloLocation, LongDistanceAttack_Range);
	//
	// FTimerHandle LongDistanceAttack_EndTimer;
	//
	//
	// if (DistanceToTarget <= LongDistanceAttack_Range)
	// {
	// 	// ���� ���� ���� ������ ����
	// 	FHitResult HitResult;
	// 	FCollisionQueryParams Params;
	// 	Params.AddIgnoredActor(this);
	//
	// 	GetChampionAnimInstance()->Montage_Play(LongDistanceAttackAnimMontage, 1.f);
	//
	// 	if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, TestDiavoloLocation, ECC_Visibility, Params))
	// 	{
	// 		// ������ ����
	// 		if (AActor* HitActor = HitResult.GetActor())
	// 		{
	// 			// �ӽ÷� �α� ���
	// 			UE_LOG(LogTemp, Warning, TEXT("Hit: %s"), *HitActor->GetName());
	// 			//��ƺ��� �������� ��� Ȥ�� ���ݴ��ϴ� ��� �߰�
	// 		}
	// 	}
	// }
	// else
	// {
	// 	// ���� ���� �ۿ� ������ �̵�
	// 	FVector Direction = (TestDiavoloLocation - Start).GetSafeNormal();
	// 	FVector MoveLocation = TestDiavoloLocation - Direction * LongDistanceAttack_Range;
	//
	// 	UE_LOG(LogTemp, Warning, TEXT("Moving to: %s"), *MoveLocation.ToString());
	// 	MoveToLocation(MoveLocation);
	//
	// }
		
}

void AOPTristana::BasicAttack_CannonBall()
{
	if (CannonBallClass == nullptr) return;

	FRotator CannonBallRotator = FRotator(BasicAttack_Angle, GetActorRotation().Yaw, GetActorRotation().Roll);
	FVector CannonBallVelocity = BasicAttack_Speed * CannonBallRotator.Vector();
	
	if (CannonBall = GetWorld()->SpawnActor<AOPTristanaCannonBall>(CannonBallClass, CannonBallSpawnPoint->GetComponentLocation(), GetActorRotation()))
	{
		CannonBall->GetOPProjectileMovementComponent()->Velocity = CannonBallVelocity;
		CannonBall->SetOwner(this);
		FVector LaunchDirection = GetActorForwardVector();
	}
}


void AOPTristana::Q()  //���� �߻� (Rapid Fire) ȿ��: ���� �ð� ���� Ʈ����Ÿ���� ���� �ӵ��� ���� �����մϴ�.
{
	Super::Q();
	//������ �Һٴ� vfx �߰� �ʿ�

	bIsRapidFireActive = true;

	BasicAttackCooltime *= RapidFireValue;

	if (RapidFireNiagaraComponent)
	{
		RapidFireNiagaraComponent->SetVisibility(true);
	}
	GetWorldTimerManager().SetTimer(RapidFireTimerHandle, this, &AOPTristana::EndRapidFire, RapidFireDuration, false);

	GetWorldTimerManager().SetTimer(Skill_1_CooltimeTimerHandle, this, &AOPTristana::SetbSkill_1_True, Skill_1_Cooltime, false);
}


void AOPTristana::EndRapidFire()
{
	bIsRapidFireActive = false;

	BasicAttackCooltime = DefaultAttackSpeed;

	if (RapidFireNiagaraComponent)
	{
		RapidFireNiagaraComponent->SetVisibility(false);
	}

	UE_LOG(LogTemp, Warning, TEXT("Rapid Fire ended: Attack speed normalized."));
}

void AOPTristana::W() //���� ���� (Rocket Jump) ȿ��: Ʈ����Ÿ���� ��ǥ �������� ������ �̿��� �پ�ö�, ���� �������� ���� ���ظ� ������ ������ ��ȭ��ŵ�ϴ�. ����: ���� ������ ���� óġ�ϰų� ��ý�Ʈ�� ����ϸ� ���� ��� �ð��� �ʱ�ȭ�˴ϴ�.
{
	Super::W();

	if (!bSkill_2) return;
	if (OPPlayerController == nullptr) return;

	OPPlayerController->GetHitResultUnderCursor(ECC_Visibility, false, MouseCursorHit);
	if (!MouseCursorHit.bBlockingHit) return;
	UE_LOG(LogTemp, Warning, TEXT("cursor Hit"));
	TurnCharacterToCursor(MouseCursorHit);

	// TestDiavolo = Cast<AOPDiavolo>(MouseCursorHit.GetActor());
	// if (TestDiavolo == nullptr) return;

	// FVector DiavoloLocation = TestDiavolo->GetActorLocation();

	FVector CurrentLocation = GetActorLocation();
	// float Distance = FVector::Dist(CurrentLocation, DiavoloLocation);
	float Distance = FVector::Dist(CurrentLocation, MouseCursorHit.Location);

	if (Distance <= Skill_2_MaxJumpRange)
	{
		Skill_2_FinalLocation = MouseCursorHit.Location;
		FVector Skill_2_Vector = Skill_2_FinalLocation - GetActorLocation();
	
		Skill_2_Vector_XY = Skill_2_Vector.GetSafeNormal();
		Skill_2_Vector_XY.Z = 0.f;
	}

	GetWorldTimerManager().SetTimer(Skill_2_JumpTimerHandle, FTimerDelegate::CreateLambda([&]
	{
		LaunchCharacter(Skill_2_Vector_XY * Skill_2_Velocity_XY + GetActorUpVector() * Skill_2_Velocity_Z, true, true);
	}), 0.25f, false);

	if (ChampionAnimInstance && Skill_2_AnimMontage)
	{
		ChampionAnimInstance->Montage_Play(Skill_2_AnimMontage, 1.0f);
	}
	// TargetLocation = DiavoloLocation;
	// FVector JumpDirection = (TargetLocation - CurrentLocation).GetSafeNormal();
	// LaunchCharacter(JumpDirection * Skill_2_JumpStrength, true, true);

	// Set a timer to handle landing effects
	GetWorldTimerManager().SetTimer(Skill_2_CooltimeTimerHandle, this, &AOPTristana::OnLanding, 1.0f, false, 1.0f);
	
	StopChampionMovement();
    GetWorldTimerManager().SetTimer(ResetMovementTimerHandle, this, &AOPTristana::ResetChampionMovement, 0.7f, false);
    
    SetbSkill_2_False();
    GetWorldTimerManager().SetTimer(Skill_2_CooltimeTimerHandle, this, &AOPTristana::SetbSkill_2_True, Skill_2_Cooltime, false);
}

void AOPTristana::OnLanding()
{
	// Handle landing damage and effects
	TArray<AActor*> IgnoredActors;
	IgnoredActors.Add(this);

	// Apply damage to actors within radius ���� ���� ĳ���͵鿡�� ���ο� ����Ʈ �ο�
	//UGameplayStatics::ApplyRadialDamage(this, Skill_2_LandingDamage, GetActorLocation(), Skill_2_LandingRadius, nullptr, IgnoredActors, this, GetController(), true);
	// Apply slow effect to actors within radius (you need to implement the slow effect)
	// ���ο� ����Ʈ �ֱ�
	// This can be done via a gameplay ability system or custom logic

	// Clear timer
	GetWorldTimerManager().ClearTimer(Skill_2_CooltimeTimerHandle);
}

void AOPTristana::PlaySkill_2_JumpAnimMontage()
{
}


void AOPTristana::E() //���� ȭ��(Explosive Charge) 		ȿ��: �нú��, Ʈ����Ÿ���� óġ�� ���� �����Ͽ� �ֺ� ���鿡�� ���ظ� �����ϴ�.Ȱ��ȭ : Ʈ����Ÿ���� ������ ���߹� ��Ű���� ��ġ�մϴ�.�ð��� �����ų� Ʈ����Ÿ���� ���� Ƚ�� ������ ���ϸ� �����Ͽ� ū ���ظ� �����ϴ�.
{
	Super::E();

	if (!bSkill_3) return;
	if (OPPlayerController == nullptr) return;

	OPPlayerController->GetHitResultUnderCursor(ECC_Visibility, false, MouseCursorHit);
	if (!MouseCursorHit.bBlockingHit) return;
	UE_LOG(LogTemp, Warning, TEXT("cursor Hit"));
	TurnCharacterToCursor(MouseCursorHit);

	TestDiavolo = Cast<AOPDiavolo>(MouseCursorHit.GetActor());
	if (TestDiavolo == nullptr) return;

	FVector DiavoloLocation = TestDiavolo->GetActorLocation();

	FVector CurrentLocation = GetActorLocation();
	float Distance = FVector::Dist(CurrentLocation, DiavoloLocation);

	if (Distance <= Skill_3_MaxThrowRange)
	{
		if (ChampionAnimInstance && Skill_3_AnimMontage)
		{
			ChampionAnimInstance->Montage_Play(Skill_3_AnimMontage, 1.0f);
		}
		GetWorldTimerManager().SetTimer(ExplosiveChargeSpawnTimerHandle, FTimerDelegate::CreateLambda([&]
		{
			UseExplosiveCharge(TestDiavolo);
		}), 0.2f, false);
	}
	
	StopChampionMovement();
    GetWorldTimerManager().SetTimer(ResetMovementTimerHandle, this, &AOPTristana::ResetChampionMovement, 0.7f, false);
        
    SetbSkill_3_False();
    GetWorldTimerManager().SetTimer(Skill_3_CooltimeTimerHandle, this, &AOPTristana::SetbSkill_3_True, Skill_3_Cooltime, false);
}

void AOPTristana::UseExplosiveCharge(AOPDiavolo* Target)
{
	if (ExplosiveChargeClass && Target)
	{
		UE_LOG(LogTemp, Warning, TEXT("UseExplosiveCharge"));
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator();

		// Spawn the explosive charge
		AOPTristanaExplosiveCharge* ExplosiveChargeNow = GetWorld()->SpawnActor<AOPTristanaExplosiveCharge>(ExplosiveChargeClass, Target->GetActorLocation(), FRotator::ZeroRotator, SpawnParams);
		if (ExplosiveChargeNow)
		{
			// ExplosiveChargeNow->AttachToComponent(Target->GetCapsuleComponent(), FAttachmentTransformRules::KeepWorldTransform);
			ExplosiveChargeNow->InitializeCharge(Target, Skill_3_Damage, Skill_3_ExplosionRadius, Skill_3_TimeToExplode);
			CurrentExplosiveCharge = ExplosiveChargeNow;
		}
	}
}


void AOPTristana::R() //�뱸�� źȯ (Buster Shot)ȿ��: Ʈ����Ÿ���� ������ źȯ�� �߻��Ͽ� ��󿡰� ���ظ� ������, ����� �ָ� �о���ϴ�. Ȱ��ȭ : �� ��ų�� �� è�Ǿ��� �о�� �� ȿ�����̸�, Ư�� ������ ���� �ָ� �о�� ���� ��Ȳ���� Ż���ϰų� ���� �и���Ű�� �� ����� �� �ֽ��ϴ�.
{
	Super::R();

	if (!bUlt) return;
	if (OPPlayerController == nullptr) return;

	OPPlayerController->GetHitResultUnderCursor(ECC_Visibility, false, MouseCursorHit);

	if (!MouseCursorHit.bBlockingHit) return;
	TurnCharacterToCursor(MouseCursorHit);

	GetWorldTimerManager().SetTimer(BusterShotClassSpawnTimer, FTimerDelegate::CreateLambda([&]
	{
		Ult_BusterShot();
	}), 0.3f, false);

	SetbUlt_False();
	GetWorldTimerManager().SetTimer(Ult_CooltimeTimerHandle, this, &AOPTristana::SetbUlt_True, GetUlt_Cooltime(), false);

	if (ChampionAnimInstance && Ult_AnimMontage)
	{
		ChampionAnimInstance->Montage_Play(Ult_AnimMontage, 1.0f);
	}
	
	StopChampionMovement();
    GetWorldTimerManager().SetTimer(ResetMovementTimerHandle, this, &AOPTristana::ResetChampionMovement, 0.9f, false);
        
    SetbUlt_False();
    GetWorldTimerManager().SetTimer(Ult_CooltimeTimerHandle, this, &AOPTristana::SetbUlt_True, Ult_Cooltime, false);
}

void AOPTristana::Ult_BusterShot()
{
	UE_LOG(LogTemp, Log, TEXT("Ult_BusterShot"));

	if (BusterShotClass == nullptr) return;

	FRotator BusterShotRotator = FRotator(Ult_Angle, GetActorRotation().Yaw, GetActorRotation().Roll);
	FVector BusterShotVelocity = Ult_Speed * BusterShotRotator.Vector();

	if (BusterShot = GetWorld()->SpawnActor<AOPTristanaBusterShot>(BusterShotClass, CannonBallSpawnPoint->GetComponentLocation(), GetActorRotation()))
	{
		BusterShot->GetOPProjectileMovementComponent()->Velocity = BusterShotVelocity;
		BusterShot->SetOwner(this);
		LaunchCharacter(-BusterShotRotator.Vector() * Ult_Rebound, true, true);
	}
}

/* ������ �̵� ������ ���߿� ����
// ���� ���� ���� ��� ���� ��� ��ġ�� �ִ� �Ÿ��� ���
float AOPTristana::CalculateMinDistanceToActorEdge(FVector ActorLocation, FVector TargetLocation, float Radius)
{
	// ���� ���� ���� ��� ���� ��� ��ġ�� �ִ� �Ÿ��� ���
	FVector Direction = (TargetLocation - ActorLocation).GetSafeNormal();
	FVector EdgePoint = ActorLocation + Direction * Radius;

	return FVector::Dist(EdgePoint, TargetLocation);
}

void AOPTristana::MoveToLocation(FVector TargetLocation)
{
	FVector Start = GetActorLocation();
	FVector Direction = (TargetLocation - Start).GetSafeNormal();
	MoveTargetLocation = TargetLocation - Direction * LongDistanceAttack_Range;
	bIsMoving = true;
}
*/





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

AOPTristana::AOPTristana()
{
	PrimaryActorTick.bCanEverTick = true;
	
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement Component")); // ����ü ������ �����Ϳ� ���� �Ҵ�
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
}

void AOPTristana::BeginPlay()
{
	Super::BeginPlay();

	DefaultAttackSpeed = LongDistanceAttack_Speed;

	Skill_2_LongJumpRange = Skill_2_MaxJumpRange;
}

void AOPTristana::Tick(float DeltaTime)
{
}

void AOPTristana::Passive()
{

}

void AOPTristana::MeleeAttack()
{
	Super::MeleeAttack();

	if (!GetbLongDistanceAttack()) return;
	if (!GetOPPlayerController()) return;

	GetOPPlayerController()->GetHitResultUnderCursor(ECC_Visibility, false, MouseCursorHit);
	if (!MouseCursorHit.bBlockingHit) return;
	UE_LOG(LogTemp, Warning, TEXT("cursor Hit"));
	TurnCharacterToCursor(MouseCursorHit);

	TestDiavolo = Cast<AOPDiavolo>(MouseCursorHit.GetActor());
	if (TestDiavolo == nullptr) return;

	MeleeAttack_CannonBall();

	GetWorldTimerManager().SetTimer(CannonBallClassSpawnTimer, this, &AOPTristana::MeleeAttack_CannonBall, LongDistanceAttack_Speed, false);
	//GetWorldTimerManager().SetTimer(MeleeAttackCooltimeTimer, this, &AOPTristana::SetbMeleeAttack, GetMeleeAttackCooltime(), false); //�̺κ��� �� SetTimer�� �������� �������
	
	
	/* ���� �̵� �� ������ ���� �߰�
	FVector Start = GetActorLocation();
	FVector TestDiavoloLocation = TestDiavolo->GetActorLocation();

	float DistanceToTarget = CalculateMinDistanceToActorEdge(Start, TestDiavoloLocation, LongDistanceAttack_Range);

	FTimerHandle LongDistanceAttack_EndTimer;


	if (DistanceToTarget <= LongDistanceAttack_Range)
	{
		// ���� ���� ���� ������ ����
		FHitResult HitResult;
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this);

		GetChampionAnimInstance()->Montage_Play(LongDistanceAttackAnimMontage, 1.f);

		if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, TestDiavoloLocation, ECC_Visibility, Params))
		{
			// ������ ����
			if (AActor* HitActor = HitResult.GetActor())
			{
				// �ӽ÷� �α� ���
				UE_LOG(LogTemp, Warning, TEXT("Hit: %s"), *HitActor->GetName());
				//��ƺ��� �������� ��� Ȥ�� ���ݴ��ϴ� ��� �߰�
			}
		}
	}
	else
	{
		// ���� ���� �ۿ� ������ �̵�
		FVector Direction = (TestDiavoloLocation - Start).GetSafeNormal();
		FVector MoveLocation = TestDiavoloLocation - Direction * LongDistanceAttack_Range;

		UE_LOG(LogTemp, Warning, TEXT("Moving to: %s"), *MoveLocation.ToString());
		MoveToLocation(MoveLocation);
	}
	*/
}

void AOPTristana::MeleeAttack_CannonBall()
{
	UE_LOG(LogTemp, Log, TEXT("MeleeAttack_CannonBall"));
	ChampionAnimInstance->Montage_Play(GetMeleeAttackAnimMontage(), 1.0f);

	if (CannonBallClass == nullptr) return;

	FVector SpawnLocation = GetActorLocation() + GetActorForwardVector() * 100.f; // ĳ���ͺ��� ��¦ �տ� ����
	FRotator SpawnRotation = GetActorRotation();

	if (CannonBall = GetWorld()->SpawnActor<AOPTristanaCannonBall>(CannonBallClass, SpawnLocation, SpawnRotation))
	{
		CannonBall->SetOwner(this);
		FVector LaunchDirection = GetActorForwardVector();
	}
}


void AOPTristana::Skill_1()  //���� �߻� (Rapid Fire) ȿ��: ���� �ð� ���� Ʈ����Ÿ���� ���� �ӵ��� ���� �����մϴ�.
{
	Super::Skill_1();
	//������ �Һٴ� vfx �߰� �ʿ�

	bIsRapidFireActive = true;
	
	LongDistanceAttack_Speed *= 0.8;

	GetWorldTimerManager().SetTimer(RapidFireTimerHandle, this, &AOPTristana::EndRapidFire, RapidFireDuration, false);
}


void AOPTristana::EndRapidFire()
{
	bIsRapidFireActive = false;

	LongDistanceAttack_Speed = DefaultAttackSpeed;

	UE_LOG(LogTemp, Warning, TEXT("Rapid Fire ended: Attack speed normalized."));
}

void AOPTristana::Skill_2() //���� ���� (Rocket Jump) ȿ��: Ʈ����Ÿ���� ��ǥ �������� ������ �̿��� �پ�ö�, ���� �������� ���� ���ظ� ������ ������ ��ȭ��ŵ�ϴ�. ����: ���� ������ ���� óġ�ϰų� ��ý�Ʈ�� ����ϸ� ���� ��� �ð��� �ʱ�ȭ�˴ϴ�.
{
	Super::Skill_2();

	if (!GetbSkill_2()) return;
	if (!GetOPPlayerController()) return;

	GetOPPlayerController()->GetHitResultUnderCursor(ECC_Visibility, false, MouseCursorHit);
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
		if (Distance <= Skill_2_ShortJumpRange)
		{
			ChampionAnimInstance->Montage_JumpToSection(FName("Short"), Skill_2_AnimMontage);
		}
		
		else if (Distance <= Skill_2_MiddleJumpRange)
		{
			ChampionAnimInstance->Montage_JumpToSection(FName("Middle"), Skill_2_AnimMontage);
		}
		
		else if (Distance <= Skill_2_LongJumpRange)
		{
			ChampionAnimInstance->Montage_JumpToSection(FName("Long"), Skill_2_AnimMontage);
		}
	}
	// TargetLocation = DiavoloLocation;
	// FVector JumpDirection = (TargetLocation - CurrentLocation).GetSafeNormal();
	// LaunchCharacter(JumpDirection * Skill_2_JumpStrength, true, true);

	// Set a timer to handle landing effects
	GetWorldTimerManager().SetTimer(Skill_2_CooltimeTimer, this, &AOPTristana::OnLanding, 1.0f, false, 1.0f);
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
	GetWorldTimerManager().ClearTimer(Skill_2_CooltimeTimer);
}

void AOPTristana::PlaySkill_2_JumpAnimMontage()
{
}


void AOPTristana::Skill_3() //���� ȭ��(Explosive Charge) 		ȿ��: �нú��, Ʈ����Ÿ���� óġ�� ���� �����Ͽ� �ֺ� ���鿡�� ���ظ� �����ϴ�.Ȱ��ȭ : Ʈ����Ÿ���� ������ ���߹� ��Ű���� ��ġ�մϴ�.�ð��� �����ų� Ʈ����Ÿ���� ���� Ƚ�� ������ ���ϸ� �����Ͽ� ū ���ظ� �����ϴ�.
{
	Super::Skill_3();

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


void AOPTristana::Ult() //�뱸�� źȯ (Buster Shot)ȿ��: Ʈ����Ÿ���� ������ źȯ�� �߻��Ͽ� ��󿡰� ���ظ� ������, ����� �ָ� �о���ϴ�. Ȱ��ȭ : �� ��ų�� �� è�Ǿ��� �о�� �� ȿ�����̸�, Ư�� ������ ���� �ָ� �о�� ���� ��Ȳ���� Ż���ϰų� ���� �и���Ű�� �� ����� �� �ֽ��ϴ�.
{
	Super::Ult();

	if (!bUlt) return;
	if (!OPPlayerController) return;

	OPPlayerController->GetHitResultUnderCursor(ECC_Visibility, false, MouseCursorHit);

	if (!MouseCursorHit.bBlockingHit) return;
	TurnCharacterToCursor(MouseCursorHit);

	GetWorldTimerManager().SetTimer(BusterShotClassSpawnTimer, FTimerDelegate::CreateLambda([&]
		{
			Ult_BusterShot();
		}), 0.35f, false);

	SetbUlt_False();
	GetWorldTimerManager().SetTimer(Ult_CooltimeTimer, this, &AOPTristana::SetbUlt_True, GetUlt_Cooltime(), false);

	if (ChampionAnimInstance && Ult_AnimMontage)
	{
		ChampionAnimInstance->Montage_Play(Ult_AnimMontage, 1.0f);
	}
}

void AOPTristana::Ult_BusterShot()
{
	UE_LOG(LogTemp, Log, TEXT("Ult_BusterShot"));

	if (BusterShotClass == nullptr) return;

	FVector SpawnLocation = GetActorLocation() + GetActorForwardVector() * 100.f; // ĳ���ͺ��� ��¦ �տ� ����
	FRotator SpawnRotation = GetActorRotation();

	if (BusterShot = GetWorld()->SpawnActor<AOPTristanaBusterShot>(BusterShotClass, SpawnLocation, SpawnRotation))
	{
		BusterShot->SetOwner(this);
		FVector LaunchDirection = GetActorForwardVector();
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





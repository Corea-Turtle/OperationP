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

AOPTristana::AOPTristana()
{
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement Component")); // ����ü ������ �����Ϳ� ���� �Ҵ�
}

void AOPTristana::BeginPlay()
{
	Super::BeginPlay();
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

	GetWorldTimerManager().SetTimer(CannonBallClassSpawnTimer, this, &AOPTristana::MeleeAttack_CannonBall, 0.25f, false);
	//GetWorldTimerManager().SetTimer(MeleeAttackCooltimeTimer, this, &AOPTristana::SetbMeleeAttack, GetMeleeAttackCooltime(), false);
	
	
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
}

void AOPTristana::Skill_2() //���� ���� (Rocket Jump) ȿ��: Ʈ����Ÿ���� ��ǥ �������� ������ �̿��� �پ�ö�, ���� �������� ���� ���ظ� ������ ������ ��ȭ��ŵ�ϴ�. ����: ���� ������ ���� óġ�ϰų� ��ý�Ʈ�� ����ϸ� ���� ��� �ð��� �ʱ�ȭ�˴ϴ�.
{
}

void AOPTristana::Skill_3() //���� ȭ��(Explosive Charge) 		ȿ��: �нú��, Ʈ����Ÿ���� óġ�� ���� �����Ͽ� �ֺ� ���鿡�� ���ظ� �����ϴ�.Ȱ��ȭ : Ʈ����Ÿ���� ������ ���߹� ��Ű���� ��ġ�մϴ�.�ð��� �����ų� Ʈ����Ÿ���� ���� Ƚ�� ������ ���ϸ� �����Ͽ� ū ���ظ� �����ϴ�.
{
}

void AOPTristana::Ult() //�뱸�� źȯ (Buster Shot)ȿ��: Ʈ����Ÿ���� ������ źȯ�� �߻��Ͽ� ��󿡰� ���ظ� ������, ����� �ָ� �о���ϴ�. Ȱ��ȭ : �� ��ų�� �� è�Ǿ��� �о�� �� ȿ�����̸�, Ư�� ������ ���� �ָ� �о�� ���� ��Ȳ���� Ż���ϰų� ���� �и���Ű�� �� ����� �� �ֽ��ϴ�.
{
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





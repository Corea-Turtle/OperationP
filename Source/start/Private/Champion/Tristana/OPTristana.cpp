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

void AOPTristana::LongDistanceAttack()
{
	Super::LongDistanceAttack();

	if (!GetbLongDistanceAttack()) return;
	if (!GetOPPlayerController()) return;

	GetOPPlayerController()->GetHitResultUnderCursor(ECC_Visibility, false, MouseCursorHit);
	if (!MouseCursorHit.bBlockingHit) return;
	UE_LOG(LogTemp, Warning, TEXT("cursor Hit"));

	TestDiavolo = Cast<AOPDiavolo>(MouseCursorHit.GetActor());
	if (TestDiavolo == nullptr) return;

	FVector Start = GetActorLocation();
	FVector TestDiavoloLocation = TestDiavolo->GetActorLocation();

	float DistanceToTarget = CalculateMinDistanceToActorEdge(Start, TestDiavoloLocation, LongDistanceAttack_Range);

	FTimerHandle LongDistanceAttack_EndTimer;
	GetWorldTimerManager().SetTimer(LongDistanceAttack_EndTimer, FTimerDelegate::CreateLambda([&]
		{
			LongDistanceAttackTrace();
		}), 0.25f, false);

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
}

bool AOPTristana::LongDistanceAttackTrace()
{

	return false;
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



// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Champion/OPChampion.h"
#include "OPTristana.generated.h"

/**
 * 
 */
class AOPTristanaCannonBall;
UCLASS()
class START_API AOPTristana : public AOPChampion
{
	GENERATED_BODY()
public:
	AOPTristana();

protected:
	virtual void BeginPlay() override;

	virtual void Passive() override;
	virtual void LongDistanceAttack() override; //���Ÿ� �⺻����
	bool LongDistanceAttackTrace();
	virtual void Skill_1() override; //���� �߻� (Rapid Fire) ȿ��: ���� �ð� ���� Ʈ����Ÿ���� ���� �ӵ��� ���� �����մϴ�.
	virtual void Skill_2() override; //���� ���� (Rocket Jump) ȿ��: Ʈ����Ÿ���� ��ǥ �������� ������ �̿��� �پ�ö�, ���� �������� ���� ���ظ� ������ ������ ��ȭ��ŵ�ϴ�. ����: ���� ������ ���� óġ�ϰų� ��ý�Ʈ�� ����ϸ� ���� ��� �ð��� �ʱ�ȭ�˴ϴ�.
	virtual void Skill_3() override; //���� ȭ��(Explosive Charge) 		ȿ��: �нú��, Ʈ����Ÿ���� óġ�� ���� �����Ͽ� �ֺ� ���鿡�� ���ظ� �����ϴ�.		Ȱ��ȭ : Ʈ����Ÿ���� ������ ���߹� ��Ű���� ��ġ�մϴ�.�ð��� �����ų� Ʈ����Ÿ���� ���� Ƚ�� ������ ���ϸ� �����Ͽ� ū ���ظ� �����ϴ�.
	virtual void Ult() override; //�뱸�� źȯ (Buster Shot)ȿ��: Ʈ����Ÿ���� ������ źȯ�� �߻��Ͽ� ��󿡰� ���ظ� ������, ����� �ָ� �о���ϴ�. Ȱ��ȭ : �� ��ų�� �� è�Ǿ��� �о�� �� ȿ�����̸�, Ư�� ������ ���� �ָ� �о�� ���� ��Ȳ���� Ż���ϰų� ���� �и���Ű�� �� ����� �� �ֽ��ϴ�.
	

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile Movement component", meta = (AllowPrivateAccess = "true"));
	class UProjectileMovementComponent* ProjectileMovementComponent; //�߻�ü �������� �߰�����

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physical Tests", meta = (AllowPrivateAccess = "true"))
	float LongDistanceAttack_Impulse = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physical Tests", meta = (AllowPrivateAccess = "true"))
	float LongDistanceAttack_Range = 0.f;

	TObjectPtr<AOPTristanaCannonBall> CannonBall; // ��Ÿ ĳ�� ���� �Լ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LongDistanceAttack", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AOPTristanaCannonBall> CannonBallClass; // �� ������ ��Ƽ� ������ ���� ĳ��

	FVector MoveTargetLocation; //�̵� ���� ����
	bool bIsMoving; //�̵� ���� ����

	float CalculateMinDistanceToActorEdge(FVector ActorLocation, FVector TargetLocation, float Radius);
	void MoveToLocation(FVector TargetLocation);
};


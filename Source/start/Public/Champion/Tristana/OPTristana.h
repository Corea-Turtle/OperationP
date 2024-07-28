// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Champion/OPChampion.h"
#include "OPTristana.generated.h"

/**
 * 
 */
class AOPTristanaCannonBall;
class AOPTristanaBusterShot;
class AOPTristanaExplosiveCharge;
UCLASS()
class START_API AOPTristana : public AOPChampion
{
	GENERATED_BODY()
public:
	AOPTristana();

protected:
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Passive() override;
	virtual void MeleeAttack() override; //���Ÿ� �⺻����
	void MeleeAttack_CannonBall();
	virtual void Skill_1() override; //���� �߻� (Rapid Fire) ȿ��: ���� �ð� ���� Ʈ����Ÿ���� ���� �ӵ��� ���� �����մϴ�.
	void EndRapidFire();
	virtual void Skill_2() override; //���� ���� (Rocket Jump) ȿ��: Ʈ����Ÿ���� ��ǥ �������� ������ �̿��� �پ�ö�, ���� �������� ���� ���ظ� ������ ������ ��ȭ��ŵ�ϴ�. ����: ���� ������ ���� óġ�ϰų� ��ý�Ʈ�� ����ϸ� ���� ��� �ð��� �ʱ�ȭ�˴ϴ�.
	void OnLanding();
	void PlaySkill_2_JumpAnimMontage();
	virtual void Skill_3() override; //���� ȭ��(Explosive Charge) 		ȿ��: �нú��, Ʈ����Ÿ���� óġ�� ���� �����Ͽ� �ֺ� ���鿡�� ���ظ� �����ϴ�.		Ȱ��ȭ : Ʈ����Ÿ���� ������ ���߹� ��Ű���� ��ġ�մϴ�.�ð��� �����ų� Ʈ����Ÿ���� ���� Ƚ�� ������ ���ϸ� �����Ͽ� ū ���ظ� �����ϴ�.
	// Function to use Explosive Charge
	void UseExplosiveCharge(AOPDiavolo* Target);

	virtual void Ult() override; //�뱸�� źȯ (Buster Shot)ȿ��: Ʈ����Ÿ���� ������ źȯ�� �߻��Ͽ� ��󿡰� ���ظ� ������, ����� �ָ� �о���ϴ�. Ȱ��ȭ : �� ��ų�� �� è�Ǿ��� �о�� �� ȿ�����̸�, Ư�� ������ ���� �ָ� �о�� ���� ��Ȳ���� Ż���ϰų� ���� �и���Ű�� �� ����� �� �ֽ��ϴ�.
	void Ult_BusterShot();

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile Movement component", meta = (AllowPrivateAccess = "true"));
	class UProjectileMovementComponent* ProjectileMovementComponent; //�߻�ü �������� �߰�����

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physical Tests", meta = (AllowPrivateAccess = "true"))
	float LongDistanceAttack_Impulse = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physical Tests", meta = (AllowPrivateAccess = "true"))
	float LongDistanceAttack_Range = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physical Tests", meta = (AllowPrivateAccess = "true"))
	float LongDistanceAttack_Speed = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physical Tests", meta = (AllowPrivateAccess = "true"))
	float Skill_2_MaxJumpRange = 1000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physical Tests", meta = (AllowPrivateAccess = "true"))
	float Skill_2_ShortJumpRange = 350.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physical Tests", meta = (AllowPrivateAccess = "true"))
	float Skill_2_MiddleJumpRange = 700.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physical Tests", meta = (AllowPrivateAccess = "true"))
	float Skill_2_LongJumpRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physical Tests", meta = (AllowPrivateAccess = "true"))
	float Skill_2_Velocity_XY = 0.f; // ??? 4 ???

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physical Tests", meta = (AllowPrivateAccess = "true"))
	float Skill_2_Velocity_Z = 0.f; // ??? 4 ???

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physical Tests", meta = (AllowPrivateAccess = "true"))
	float Skill_2_JumpStrength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physical Tests", meta = (AllowPrivateAccess = "true"))
	float Skill_2_LandingDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physical Tests", meta = (AllowPrivateAccess = "true"))
	float Skill_2_LandingRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physical Tests", meta = (AllowPrivateAccess = "true"))
	float Skill_2_SlowDuration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physical Tests", meta = (AllowPrivateAccess = "true"))
	AOPDiavolo* Skill_3_Target;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physical Tests", meta = (AllowPrivateAccess = "true"))
	float Skill_3_Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physical Tests", meta = (AllowPrivateAccess = "true"))
	float Skill_3_ExplosionRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physical Tests", meta = (AllowPrivateAccess = "true"))
	float Skill_3_TimeToExplode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physical Tests", meta = (AllowPrivateAccess = "true"))
	float Skill_3_MaxThrowRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physical Tests", meta = (AllowPrivateAccess = "true"))
	float Ult_Angle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physical Tests", meta = (AllowPrivateAccess = "true"))
	float Ult_Impulse;


	TObjectPtr<AOPTristanaCannonBall> CannonBall; // ��Ÿ ĳ�� ���� �Լ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MeleeAttack", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AOPTristanaCannonBall> CannonBallClass; // �� ������ ��Ƽ� ������ ���� ĳ��

	FTimerHandle CannonBallClassSpawnTimer; // ĳ�� ������ ������ ���� Ÿ�̸�
	FTimerHandle Skill_2_JumpTimerHandle;
	FVector Skill_2_FinalLocation;
	FVector Skill_2_Vector_XY;

	TObjectPtr<AOPTristanaBusterShot> BusterShot; // �����ͼ� ���� �Լ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ult", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AOPTristanaBusterShot> BusterShotClass; // �� ������ ��Ƽ� ������ ���� �����ͼ�

	FTimerHandle BusterShotClassSpawnTimer; // �����ͼ� ������ ������ ���� Ÿ�̸�


	TObjectPtr<AOPTristanaExplosiveCharge> ExplosiveCharge; // ���� ȭ�� ���� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill3", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AOPTristanaExplosiveCharge> ExplosiveChargeClass; // �� ������ ��Ƽ� ������ ���� ����ȭ��

	// Current active explosive charge
	AOPTristanaExplosiveCharge* CurrentExplosiveCharge;

	FTimerHandle ExplosiveChargeClassSpawnTimer; // ĳ�� ������ ������ ���� Ÿ�̸�



	//skill1 rapid fire
	float DefaultAttackSpeed;

	FTimerHandle RapidFireTimerHandle;

	bool bIsRapidFireActive = false;

	float RapidFireDuration;

	//skill rocket jump
	FVector TargetLocation;


	// Timer handle for explosion
	FTimerHandle ExplosionTimerHandle;
	FTimerHandle ExplosiveChargeSpawnTimerHandle;


	// Handle Landing

	//FVector MoveTargetLocation; //�̵� ���� ����
	//bool bIsMoving; //�̵� ���� ����

	//float CalculateMinDistanceToActorEdge(FVector ActorLocation, FVector TargetLocation, float Radius);
	//void MoveToLocation(FVector TargetLocation);
};



// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Champion/OPChampion.h"
#include "OPLeeSin.generated.h"

UCLASS()
class START_API AOPLeeSin : public AOPChampion
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AOPLeeSin();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Passive() override;
	virtual void MeleeAttack() override;

	virtual void Skill_1() override;

	void Skill_1_SonicWave();//�Ҵп��̺�

	virtual void Skill_2() override;

	virtual void Skill_3() override;

	virtual void Ult() override;

private:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SonicWave Projectile", meta = (AllowPrivateAccess = "true"));
	class UProjectileMovementComponent* SonicWave; // �Ҵп��̺길 �������� �߰��Ǵ� �߻�ü

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dragon's Rage", meta = (AllowPrivateAccess = "true"));
	class UProjectileMovementComponent* DragonRage; // DragonRage�� �������� �߰��Ǵ� �߻�ü


	int32 MeleeAttackComboCount = 0; // ��Ÿ ���� ��� �� 4���� ����� ������ ���� �޺�ī��Ʈ
	FTimerHandle MeleeAttackComboCountTimer; // ��Ÿ ����� ���� Ÿ�̸�



	FORCEINLINE void ResetMeleeAttackComboCount() { MeleeAttackComboCount = 0; }

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


};

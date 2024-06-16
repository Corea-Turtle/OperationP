// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Champion/OPChampion.h"
#include "OPLeeSin.generated.h"

class AOPLeeSinSonicWave;
class AOPLeeSinDragonsRage;
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

	void Skill_Ult_DragonsRage();//���� �г�

private:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile Movement component", meta = (AllowPrivateAccess = "true"));
	class UProjectileMovementComponent* ProjectileMovementComponent; //  �������� �߰��Ǵ� ����ü

	int32 MeleeAttackComboCount = 0; // ��Ÿ ���� ��� �� 4���� ����� ������ ���� �޺�ī��Ʈ
	FTimerHandle MeleeAttackComboCountTimer; // ��Ÿ ����� ���� Ÿ�̸�

	int32 Skill_1_Stack = 0; // Skill_1 ����  1: �Ҵп��̺� -> 2: �̵���
	FTimerHandle Skill_1_CastTimer; // �̵��� �����ð��� ���� Ÿ�̸�
	FTimerHandle Skill_1_StackTimer; // �Ҵп��̺� ���� ���� -> �̵��� ���� Ÿ�̸�

	int32 Skill_3_Stack = 0; // Skill_3 ����  1: ��ġ�� -> 2: �ɸ� ���� �̵��ӵ� ���� 
	FTimerHandle Skill_3_CastTimer; // �̵��ӵ� ����  �����ð��� ���� Ÿ�̸�
	FTimerHandle Skill_3_StackTimer; // ��ġ�� ���� ���� -> �̵��ӵ� ����  �����ð� Ÿ�̸�

	TObjectPtr<AOPLeeSinSonicWave> SonicWave; // ���ĸ� ���� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill 1", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AOPLeeSinSonicWave> SonicWaveClass; // �� ������ ��Ƽ� ������ ���� ����

	FTimerHandle SonicWaveSpawnTimer; // ȸ���� ������ ������ ���� Ÿ�̸�
	FTimerHandle DragonsRageSpawnTimer; // ȸ���� ������ ������ ���� Ÿ�̸�

	TObjectPtr<AOPLeeSinDragonsRage> DragonsRage; // ���Ǻг븦 ���� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ult", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AOPLeeSinDragonsRage> DragonsRageClass; // ���� �г밡 ������ ��� class


	FORCEINLINE void ResetMeleeAttackComboCount() { MeleeAttackComboCount = 0; }

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


};

// Fill out your copyright notice in the Description page of Project Settings.


#include "Champion/LeeSin/OPLeeSin.h"
#include "Animation/OPAnimInstance.h"
#include "Components/CapsuleComponent.h"
#include "Diavolo/OPDiavolo.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Player/OPPlayerController.h"
#include "Sound/SoundCue.h"

// Sets default values
AOPLeeSin::AOPLeeSin()
{
	SonicWave = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Sonic Wave")); // ����ü ������ �����Ϳ� ���� �Ҵ�
	DragonRage = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Dragon's Rage")); // ����ü ������ �����Ϳ� ���� �Ҵ�
}

// Called when the game starts or when spawned
void AOPLeeSin::BeginPlay()
{
	Super::BeginPlay();
	
}

void AOPLeeSin::Passive()
{
	Super::Passive();
}

void AOPLeeSin::MeleeAttack()
{
	Super::MeleeAttack();

	if (!GetbMeleeAttack()) return; // ��Ÿ ��Ÿ�� �� return
	if (!GetOPPlayerController()) return; // �÷��̾� ��Ʈ�ѷ��� nullptr �� return

	switch (MeleeAttackComboCount) // 4���� ����� ��Ÿ����
	{
	case 0:
		GetChampionAnimInstance()->Montage_Play(MeleeAttackAnimMontage, 1.f);
		GetChampionAnimInstance()->Montage_JumpToSection(FName("1"), MeleeAttackAnimMontage);
		GetWorldTimerManager().SetTimer(MeleeAttackComboCountTimer, this, &AOPLeeSin::ResetMeleeAttackComboCount, 5.f, false);
		MeleeAttackComboCount++;
		break;

	case 1:
		GetChampionAnimInstance()->Montage_Play(MeleeAttackAnimMontage, 1.f);
		GetChampionAnimInstance()->Montage_JumpToSection(FName("2"), MeleeAttackAnimMontage);
		GetWorldTimerManager().ClearTimer(MeleeAttackComboCountTimer);
		GetWorldTimerManager().SetTimer(MeleeAttackComboCountTimer, this, &AOPLeeSin::ResetMeleeAttackComboCount, 5.f, false);
		MeleeAttackComboCount++;
		break;

	case 2:
		GetChampionAnimInstance()->Montage_Play(MeleeAttackAnimMontage, 1.f);
		GetChampionAnimInstance()->Montage_JumpToSection(FName("3"), MeleeAttackAnimMontage);
		GetWorldTimerManager().ClearTimer(MeleeAttackComboCountTimer);
		GetWorldTimerManager().SetTimer(MeleeAttackComboCountTimer, this, &AOPLeeSin::ResetMeleeAttackComboCount, 5.f, false);
		MeleeAttackComboCount++;
		break;

	case 3:
		GetChampionAnimInstance()->Montage_Play(MeleeAttackAnimMontage, 1.f);
		GetChampionAnimInstance()->Montage_JumpToSection(FName("4"), MeleeAttackAnimMontage);
		MeleeAttackComboCount = 0;
		break;

	default:
		;
	}

	// ��Ÿ ��Ÿ�� ������ ���� �� ��
	SetbMeleeAttack_False();
	GetWorldTimerManager().SetTimer(MeleeAttackCooltimeTimer, this, &AOPLeeSin::SetbMeleeAttack_True, GetMeleeAttackCooltime(), false);
}

void AOPLeeSin::Skill_1()
{
	Super::Skill_1();

	if (!GetbSkill_1()) return;
	if (!GetOPPlayerController()) return;
}

void AOPLeeSin::Skill_1_SonicWave()
{
}

void AOPLeeSin::Skill_2()
{
}

void AOPLeeSin::Skill_3()
{
}

void AOPLeeSin::Ult()
{
}

// Called every frame
void AOPLeeSin::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AOPLeeSin::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


// Fill out your copyright notice in the Description page of Project Settings.


#include "Champion/LeeSin/OPLeeSin.h"
#include "Animation/OPAnimInstance.h"
#include "Champion/LeeSin/OPLeeSinSonicWave.h"
#include "Champion/LeeSin/OPLeeSinDragonsRage.h"
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
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement Component")); // ����ü ������ �����Ϳ� ���� �Ҵ�
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

	// ECC_Visibility ä�ο� ���� ������ overlap �Ǵ� block�� ���Ϳ� hit ���� �� GetHitResultUnderCursor�� �� ���Ϳ� ���� HitResult�� MouseCursorHit�� ����.

	GetOPPlayerController()->GetHitResultUnderCursor(ECC_Visibility, false, MouseCursorHit);

	if (MouseCursorHit.bBlockingHit) // ���� ������ block�̶�� �� Hit �������� ĳ���͸� ����
	{
		TurnCharacterToCursor(MouseCursorHit);
	}

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

	GetOPPlayerController()->GetHitResultUnderCursor(ECC_Visibility, false, MouseCursorHit);

	if (MouseCursorHit.bBlockingHit)
	{
		TurnCharacterToCursor(MouseCursorHit);
	}

	check(GetChampionAnimInstance());
	check(GetSkill_1_AnimMontage());



}

void AOPLeeSin::Skill_1_SonicWave()
{

}

void AOPLeeSin::Skill_2()//�Ʊ����� �̵��ؼ� ��ȣ��
{
	Super::Skill_2();

	if (!GetbSkill_2()) return;
	if (!GetOPPlayerController()) return;

	GetOPPlayerController()->GetHitResultUnderCursor(ECC_Visibility, false, MouseCursorHit);
	if (!MouseCursorHit.bBlockingHit) return;

}

void AOPLeeSin::Skill_3()
{
}

void AOPLeeSin::Ult()
{
	Super::Ult();

	if (!GetbUlt()) return;
	if (!GetOPPlayerController()) return;

	GetOPPlayerController()->GetHitResultUnderCursor(ECC_Visibility, false, MouseCursorHit);

	if (MouseCursorHit.bBlockingHit)
	{
		TurnCharacterToCursor(MouseCursorHit);
	}

	check(GetChampionAnimInstance());
	check(GetSkill_2_AnimMontage());

	GetChampionAnimInstance()->Montage_Play(GetUlt_AnimMontage(), 1.0f);

	GetOPPlayerController()->GetHitResultUnderCursor(ECC_Visibility, false, MouseCursorHit);
	UE_LOG(LogTemp, Log, TEXT("Skill_Ult_DragonsRage"));
	GetWorldTimerManager().SetTimer(DragonsRageSpawnTimer, this, &AOPLeeSin::Skill_Ult_DragonsRage, 0.25f, false);

	Skill_Ult_DragonsRage();
}

void AOPLeeSin::Skill_Ult_DragonsRage()
{
	if (DragonsRageClass == nullptr) return;
	// ���� ��ġ�� ������ �����ɴϴ�.
	FVector CurrentLocation = GetActorLocation();
	FVector ForwardVector = GetActorForwardVector();

	// ForwardVector�� ������ �Ÿ��� ���Ͽ� ���� ���� ���� ��ġ�� ����մϴ�.
	// ���⼭ 100.0f�� ������ �Ÿ���, ���ϴ� ��� ������ �� �ֽ��ϴ�.
	FVector SpawnLocation = CurrentLocation + ForwardVector * 100.0f;

	DragonsRage = GetWorld()->SpawnActor<AOPLeeSinDragonsRage>(DragonsRageClass, SpawnLocation, GetActorRotation());
	DragonsRage->SetOwner(this);
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


// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAnim.h"
#include "Enemy.h"
#include "TPSPlayer.h"
#include "Weapon_Sword.h"
#include "PlayerFire.h"
#include "DoomStone.h"
#include "BuildableItem.h"

#include <Components/AudioComponent.h>
#include <Kismet/KismetSystemLibrary.h>
#include <Kismet/KismetMathLibrary.h>
#include <Kismet/GameplayStatics.h>


void UEnemyAnim::AnimNotify_OnAttack()
{
	TArray<AActor*> actors;
	me->targetSensor->GetOverlappingActors(actors);

	AttackToTargets(actors, AttackDamage);
}


void UEnemyAnim::AnimNotify_DamagedEnd()
{

}

void UEnemyAnim::playHitSound(bool IsDeath)
{
	FVector pos = me->GetActorLocation();
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitSound, pos, 0.5);

	if (IsDeath) {
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), DeathSound, pos, 0.5);

	}
}

void UEnemyAnim::AttackToTargets(TArray<AActor*> actors, int damage)
{
	if (actors.IsEmpty()) {
		me->fsm->isInAttackRange = false;
		return;
	}

	for (auto target : actors) {
		if (target->ActorHasTag("Player")) {
			ATPSPlayer* tempPlayer = Cast<ATPSPlayer>(target);

			if (tempPlayer->playerFire->currWeapon->weaponType == WeaponType::Sword) {
				AWeapon_Sword* tempSword = Cast<AWeapon_Sword>(tempPlayer->playerFire->currWeapon);

				if (tempSword->isBlocking == true) {
					tempSword->ReflectAttack(me);

					return;
				}
			}

			tempPlayer->OnHitEvent(damage, me->GetActorLocation());
		}

		else if (target->ActorHasTag("DoomStone"))
			Cast<ADoomstone>(target)->OnHitEvent(damage);

		else if (target->ActorHasTag("BuildableItem"))
			Cast <ABuildableItem>(target)->DamageProcess(damage);
	}

	bAttackPlay = false;
}

void UEnemyAnim::LongRangeAttack()
{
}

void UEnemyAnim::NativeBeginPlay()
{

}

void UEnemyAnim::PlayDamageAnim(bool IsDeath, AActor* attacker)
{

	if (IsDeath) {
		Montage_Play(AM_Die);
		return;
	}

	bool bossAttack = Montage_IsPlaying(AM_Attack) && bHasAbilitySkill;
	if (Montage_IsPlaying(AM_Skill) || bossAttack)
		return;

	Montage_Stop(1.f, AM_Attack);
	Montage_Play(AM_Damaged);
	if (attacker) {
		FName sectionName = "";
		FVector dir = (attacker->GetActorLocation() - me->GetActorLocation()).GetSafeNormal();

		float forward = FVector::DotProduct(dir, me->GetActorForwardVector());
		float Right = FVector::DotProduct(dir, me->GetActorRightVector());
		float AbsForward = FMath::Abs(forward);
		float AbsRight = FMath::Abs(Right);

		if (AbsForward > AbsRight) {
			if (forward > 0)
				sectionName = FName("Forward");
			else
				sectionName = FName("Backward");
		}
		else {
			if (Right > 0)
				sectionName = FName("Right");
			else
				sectionName = FName("Left");
		}

		Montage_JumpToSection(sectionName, AM_Damaged);
	}
}

void UEnemyAnim::PlayAttackAnim(bool isLongRangeAttack, bool startMotion)
{

	if (Montage_IsPlaying(AM_Skill))
		return;

	if (me->fsm->stoneStatue->isDestory)
		return;

	if (me->fsm->target->ActorHasTag("BuildableItem") && Cast <ABuildableItem>(me->fsm->target)->isDestroy) return;
		

	FRotator LookRot = UKismetMathLibrary::FindLookAtRotation(me->GetActorLocation(), me->fsm->target->GetActorLocation());
	me->SetActorRotation(FRotator(0, LookRot.Yaw, 0));

	if (isLongRangeAttack) {
		Montage_Play(AM_Skill);
		Montage_JumpToSection(longRangeSkill);
		return;
	}

	else if (me->isOverlapingTargets() == false) {
		me->fsm->isInAttackRange = false;
		return;
	}



	bool UseMeleeSkill = FMath::RandRange(0, 10) == 0;
	bool MeleeSkillCoolTimeisDone = GetWorld()->GetTimeSeconds() - lastMeleeSkillUseTime > MeleeSkillCoolTime;

	if (bHasAbilitySkill && UseMeleeSkill && MeleeSkillCoolTimeisDone) {
		Montage_Play(AM_Skill);
		Montage_JumpToSection(meleeSkill);
		lastMeleeSkillUseTime = GetWorld()->GetTimeSeconds();
		return;
	}

	FName sectionName = FName(FString::Printf(TEXT("Attack_%d"), currAttackSection++));

		if (currAttackSection > attackAMSectionCount)
			currAttackSection = 1;
	
	if (startMotion) {
		sectionName = FName("StartMotion");
		currAttackSection = 1;

		if (GetWorld()->GetTimeSeconds() - lastStartMotionTime < 7)
			return;
		lastStartMotionTime = GetWorld()->GetTimeSeconds();

	}
	
	if (Montage_IsPlaying(AM_Attack) == false) {
		Montage_Play(AM_Attack);
		Montage_JumpToSection(sectionName, AM_Attack);
	}
}

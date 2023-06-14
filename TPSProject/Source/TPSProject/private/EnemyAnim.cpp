// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAnim.h"
#include "Enemy.h"
#include "TPSPlayer.h"
#include "DoomStone.h"
#include "BuildableItem.h"

#include <Kismet/KismetSystemLibrary.h>
#include <Kismet/GameplayStatics.h>

void UEnemyAnim::OnEndAttackAnimation()
{
	if (target->ActorHasTag("Player"))
		Cast<ATPSPlayer>(target)->OnHitEvent(AttackDamage);

	else if (target->ActorHasTag("DoomStone"))
		Cast<ADoomstone>(target)->OnHitEvent(AttackDamage);

	else if (target->ActorHasTag("BuildableItem"))
		Cast <ABuildableItem>(target)->DamageProcess(AttackDamage);

	bAttackPlay = false;
}

void UEnemyAnim::AnimNotify_AttackEnd()
{
	OnEndAttackAnimation();
}

void UEnemyAnim::AnimNotify_DamagedEnd()
{
	Montage_Stop(0.f, AM_Damaged);
}

void UEnemyAnim::AnimNotify_DieENd()
{
	isDead = true;

}

void UEnemyAnim::playHitSound(bool IsDeath)
{
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitSound, me->GetActorLocation());

	if (IsDeath) {
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), DeathSound, me->GetActorLocation());
	}
}

void UEnemyAnim::PlayDamageAnim(FName sectionName)
{
	Montage_Play(AM_Damaged);
	Montage_JumpToSection(sectionName, AM_Damaged);
}

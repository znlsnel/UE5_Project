// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAnim.h"
#include "Enemy.h"
#include "TPSPlayer.h"
#include "Weapon_Sword.h"
#include "PlayerFire.h"
#include "DoomStone.h"
#include "BuildableItem.h"

#include <Kismet/KismetSystemLibrary.h>
#include <Kismet/GameplayStatics.h>

void UEnemyAnim::OnEndAttackAnimation(int Damage)
{
	int damage = 0;

	if (Damage != -1)
		damage = Damage;
	else
		damage = AttackDamage;

	if (target->ActorHasTag("Player")) {
		ATPSPlayer* tempPlayer = Cast<ATPSPlayer>(target);

		if (tempPlayer->playerFire->currWeapon->weaponType == WeaponType::Sword) {
			AWeapon_Sword* tempSword = Cast<AWeapon_Sword>(tempPlayer->playerFire->currWeapon);

			if (tempSword->isBlocking == true) {
				me->OnDamage(10, "", tempPlayer);
				tempSword->BlockEffect->Activate(true);
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), tempSword->BlockSound, tempSword->GetActorLocation());
				return;
			}
		}

		tempPlayer->OnHitEvent(damage, me->GetActorLocation());
	}

	else if (target->ActorHasTag("DoomStone"))
		Cast<ADoomstone>(target)->OnHitEvent(damage);

	else if (target->ActorHasTag("BuildableItem"))
		Cast <ABuildableItem>(target)->DamageProcess(damage);

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
	FVector pos = me->GetActorLocation();
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitSound, pos);

	if (IsDeath) {
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), DeathSound, pos);
	}
}

void UEnemyAnim::PlayDamageAnim(FName sectionName)
{
	Montage_Play(AM_Damaged);
	Montage_JumpToSection(sectionName, AM_Damaged);
}

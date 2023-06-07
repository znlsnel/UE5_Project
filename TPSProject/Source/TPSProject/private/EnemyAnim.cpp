// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAnim.h"
#include "Enemy.h"
#include "TPSPlayer.h"
#include "DoomStone.h"
#include "BuildableItem.h"

#include <Kismet/KismetSystemLibrary.h>

void UEnemyAnim::OnEndAttackAnimation()
{
	if (me->GetNetMode() != NM_DedicatedServer || target == nullptr) return;


	if (target->ActorHasTag("Player"))
		Cast<ATPSPlayer>(target)->OnHitEvent(AttackDamage);

	else if (target->ActorHasTag("DoomStone"))
		Cast<ADoomstone>(target)->OnHitEvent(AttackDamage);

	else if (target->ActorHasTag("BuildableItem"))
		Cast <ABuildableItem>(target)->DamageProcess(AttackDamage);

	bAttackPlay = false;
}

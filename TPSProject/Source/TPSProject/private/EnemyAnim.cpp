// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAnim.h"
#include "Enemy.h"
#include "TPSPlayer.h"

void UEnemyAnim::OnEndAttackAnimation()
{
	if (me->GetNetMode() != NM_DedicatedServer) return;
	if (target)
		target->OnHitEvent(AttackDamage);
	bAttackPlay = false;
}

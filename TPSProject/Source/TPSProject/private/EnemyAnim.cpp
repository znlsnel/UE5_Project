// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAnim.h"
#include "Enemy.h"

void UEnemyAnim::OnEndAttackAnimation()
{
	bAttackPlay = false;
}

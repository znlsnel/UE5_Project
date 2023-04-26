// Copyright Epic Games, Inc. All Rights Reserved.


#include "TPSProjectGameModeBase.h"
#include "EnemyManager.h"

ATPSProjectGameModeBase::ATPSProjectGameModeBase()
{
	EnemyManager = CreateDefaultSubobject<AEnemyManager>(TEXT("EnemyManager"));
}

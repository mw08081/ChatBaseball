// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/CBPlayerState.h"

#include "Net/UnrealNetwork.h"

#include "Player/CBPlayerController.h"

ACBPlayerState::ACBPlayerState()
{
	bReplicates = true;
}

void ACBPlayerState::BeginPlay()
{
	PlayerName = TEXT("None");
	
	MaxTryCnt = 5;
	CurTryCnt = 0;
	
	MaxTime = 30;
	RemainTime = -1;
}

void ACBPlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, PlayerName);
	DOREPLIFETIME(ThisClass, CurTryCnt);
	DOREPLIFETIME(ThisClass, MaxTryCnt);
	DOREPLIFETIME(ThisClass, RemainTime);
}

int32 ACBPlayerState::GetCurTryCnt() const
{
	return this->CurTryCnt;
}

int32 ACBPlayerState::GetMaxTryCnt() const
{
	return this->MaxTryCnt;
}

int32 ACBPlayerState::GetRemainTime() const
{
	return this->RemainTime;
}

void ACBPlayerState::IncreaseTryCnt()
{
	CurTryCnt++;

	// 시작 시그널
	if (CurTryCnt == 1)
	{
		SetRemainTime();
		GetWorld()->GetTimerManager().SetTimer(TurnTimerHandle, this, &ThisClass::SetRemainTime, 1, true);
	}
}

void ACBPlayerState::SetRemainTime()
{
	if (GetWorld()->GetTimerManager().IsTimerActive(TurnTimerHandle) == true)
	{
		RemainTime--;
	}
	else
	{
		RemainTime = MaxTime;
	}
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/CBPlayerState.h"

#include "Net/UnrealNetwork.h"

ACBPlayerState::ACBPlayerState()
{
	bReplicates = true;
}

void ACBPlayerState::BeginPlay()
{
	MaxTryCnt = 5;
	CurTryCnt = 0;
	PlayerName = TEXT("None");
}

void ACBPlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, PlayerName);
	DOREPLIFETIME(ThisClass, CurTryCnt);
	DOREPLIFETIME(ThisClass, MaxTryCnt);
}

int32 ACBPlayerState::GetCurTryCnt() const
{
	return this->CurTryCnt;
}

int32 ACBPlayerState::GetMaxTryCnt() const
{
	return this->MaxTryCnt;
}

void ACBPlayerState::IncreaseTryCnt()
{
	CurTryCnt++;
}

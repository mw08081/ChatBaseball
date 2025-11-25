// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/CBPlayerState.h"

#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"

#include "Player/CBPlayerController.h"
#include "Game/CBGameModeBase.h"

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
	DOREPLIFETIME(ThisClass, RemainTime);
}

void ACBPlayerState::SetCurTryCnt(int32 Value)
{
	this->CurTryCnt = Value;
}

void ACBPlayerState::SetRemainTime(int32 Value)
{
	this->RemainTime = Value;
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
	UpdateGameStateString();

	ClearTurnTimer();
	if (CurTryCnt < MaxTryCnt)
	{
		StartTimer();
	}
}

// 타이머 재설정 작업중
void ACBPlayerState::StartTimer()
{
	if (GetWorld()->GetTimerManager().IsTimerActive(TurnTimerHandle) == true)
	{
		RemainTime--;

		if (RemainTime < 0)
		{
			IncreaseTryCnt();

			// EditableTextBox의 commit으로 IncreaseTryCnt()가 호출된다면 JudgeGame()이 호출됨
			// 타이머 초과로 인해 IncreaseTryCnt()가 호출된다면, JudgeGame()을 해줘야함(특히 마지막 시도일경우)
			AGameModeBase* GameModeBase = GetWorld()->GetAuthGameMode<AGameModeBase>();
			if (IsValid(GameModeBase))
			{
				ACBGameModeBase* CBGameModeBase = Cast<ACBGameModeBase>(GameModeBase);
				if (IsValid(CBGameModeBase))
				{
					CBGameModeBase->JudgeGame(Cast<ACBPlayerController>(GetOwner()), "OUT");
				}
			}
		}
	}
	else
	{
		RemainTime = MaxTime;
		GetWorld()->GetTimerManager().SetTimer(TurnTimerHandle, this, &ThisClass::StartTimer, 1, true);
	}
}

void ACBPlayerState::ClearTurnTimer()
{
	GetWorld()->GetTimerManager().ClearTimer(TurnTimerHandle);
}

void ACBPlayerState::UpdateGameStateString()
{
	ACBPlayerController* CBPlayerController = Cast<ACBPlayerController>(GetOwner());
	if (IsValid(CBPlayerController))
	{
		if (CurTryCnt < MaxTryCnt)
		{
			CBPlayerController->GameStateString = FString::Printf(TEXT("Try count: %d / %d"), CurTryCnt, MaxTryCnt);
		}
		else
		{
			CBPlayerController->GameStateString = "You can't try anymore...";
			ClearTurnTimer();
		}
	}
}
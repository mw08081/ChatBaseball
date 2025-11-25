// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/CBGameModeBase.h"

#include "Game/CBGameStateBase.h"

#include "Player/CBPlayerController.h"
#include "Player/CBPlayerState.h"

void ACBGameModeBase::BeginPlay()
{
	Super::BeginPlay();
}

void ACBGameModeBase::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);

	ACBPlayerController* CBPlayerController = Cast<ACBPlayerController>(NewPlayer);
	if (IsValid(CBPlayerController))
	{
		AllPlayerControllers.Add(CBPlayerController);

		CBPlayerController->GameStateString = "Connected to game server!!!";

		ACBPlayerState* CBPlayerState = CBPlayerController->GetPlayerState<ACBPlayerState>();
		if (IsValid(CBPlayerState))
		{
			CBPlayerState->PlayerName = "Player" + FString::FromInt(AllPlayerControllers.Num());
		}

		ACBGameStateBase* CBGameStateBase = GetGameState<ACBGameStateBase>();
		if (IsValid(CBGameStateBase))
		{
			CBGameStateBase->MulticastRPC_BroadcastLoginMsg(CBPlayerState->PlayerName);
		}
	}
}

void ACBGameModeBase::PrintInChatMsg(ACBPlayerController* InChatController, const FString& InChatMsg)
{
	for (auto e : AllPlayerControllers)
	{
		e->ClientRPC_PrintChatMsg(InChatMsg);
	}

	ACBPlayerState* CBPlayerState = InChatController->GetPlayerState<ACBPlayerState>();
	CBPlayerState->IncreaseTryCnt();

	UE_LOG(LogTemp, Warning, TEXT("%d / %d"), CBPlayerState->GetCurTryCnt(), CBPlayerState->GetMaxTryCnt());

	// 세자리 숫자인가
		// IncreaseTryCnt()
		// 결과 판단
		
		// 게임 판단
		
		
	// 아니다
		// 클라이언트RPC
}

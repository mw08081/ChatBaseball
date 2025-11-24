// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/CBGameStateBase.h"

#include "Kismet/GameplayStatics.h"

#include "Player/CBPlayerController.h"

void ACBGameStateBase::MulticastRPC_BroadcastLoginMsg_Implementation(const FString& InPlayerName)
{
	if (HasAuthority() == false)
	{
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (IsValid(PlayerController) == false) return;
		
		ACBPlayerController* CBPlayerController = Cast<ACBPlayerController>(PlayerController);
		if (IsValid(CBPlayerController) == false) return;

		FString Msg = InPlayerName + TEXT(" has joined in the game");
		CBPlayerController->PrintInChatMsg(Msg);
	}
}

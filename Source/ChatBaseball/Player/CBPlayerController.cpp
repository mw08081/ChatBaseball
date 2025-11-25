// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/CBPlayerController.h"

#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

#include "Game/CBGameModeBase.h"
#include "Player/CBPlayerState.h"
#include "UI/ChatInput.h"
#include "UI/GameStateBoard.h"


ACBPlayerController::ACBPlayerController()
{
	bReplicates = true;
}

void ACBPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalController() == false)
	{
		return;
	}

	FInputModeUIOnly InputModeUIOnly;
	SetInputMode(InputModeUIOnly);

	if (IsValid(GameStateBoardWidgetClass))
	{
		GameStateBoardWidgetInstance = CreateWidget<UGameStateBoard>(this, GameStateBoardWidgetClass);

		if (IsValid(GameStateBoardWidgetInstance) == false) return;
		GameStateBoardWidgetInstance->AddToViewport();

		GetWorld()->GetTimerManager().SetTimer(UpdateRemainTimeHandle, this, &ThisClass::UpdateGameeStateBoard, 1, true);
	}
	
	if (IsValid(ChatInputWidgetClass))
	{ 
		ChatInputWidgetInstance = CreateWidget<UChatInput>(this, ChatInputWidgetClass);

		if (IsValid(ChatInputWidgetInstance) == false) return;
		ChatInputWidgetInstance->AddToViewport();
	}
}

void ACBPlayerController::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, GameStateString);
}

void ACBPlayerController::UpdateGameeStateBoard()
{
	ACBPlayerState* CBPlayerState = GetPlayerState<ACBPlayerState>();
	if (IsValid(CBPlayerState) && IsValid(GameStateBoardWidgetInstance))
	{
		int32 RemainTime = CBPlayerState->GetRemainTime();
		FString Value = (RemainTime == -1) ? ("Type the answer to start game!!!") : (FString::FromInt(RemainTime));

		GameStateBoardWidgetInstance->SetTextBox_Timer(Value);
	}

	if (IsValid(GameStateBoardWidgetInstance))
	{
		GameStateBoardWidgetInstance->SetTextBox_GameState(GameStateString);
	}
}

void ACBPlayerController::CommitInChatMsg(const FString& InChatMsg)
{
	ServerRPC_PrintChatMsg(InChatMsg);
}

void ACBPlayerController::PrintInChatMsg(const FString& InChatMsg)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, InChatMsg);
	}
}

void ACBPlayerController::ClientRPC_PrintChatMsg_Implementation(const FString& InChatMsg)
{
	PrintInChatMsg(InChatMsg);
}

void ACBPlayerController::ServerRPC_PrintChatMsg_Implementation(const FString& InChatMsg)
{
	AGameModeBase* GameModeBase = UGameplayStatics::GetGameMode(this);
	if (IsValid(GameModeBase) == false) return;

	ACBGameModeBase* CBGameMode = Cast<ACBGameModeBase>(GameModeBase);
	if (IsValid(CBGameMode) == false) return;

	ACBPlayerState* CBPlayerState = GetPlayerState<ACBPlayerState>();
	if (IsValid(CBPlayerState) == false) return;

	if (CBPlayerState->GetCurTryCnt() < CBPlayerState->GetMaxTryCnt())
	{
		FString Cont = CBPlayerState->PlayerName + ": " + InChatMsg;
		CBGameMode->PrintInChatMsg(this, Cont);
	}
	else
	{
		return;
	}
}

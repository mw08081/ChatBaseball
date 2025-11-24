// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/CBPlayerController.h"

#include "Kismet/GameplayStatics.h"

#include "Game/CBGameModeBase.h"
#include "Player/CBPlayerState.h"
#include "UI/ChatInput.h"


ACBPlayerController::ACBPlayerController()
{
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

	if (IsValid(ChatInputWidgetClass) == false) return;
	ChatInputWidgetInstance = CreateWidget<UChatInput>(this, ChatInputWidgetClass);

	if (IsValid(ChatInputWidgetInstance) == false) return;
	ChatInputWidgetInstance->AddToViewport();
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
	AGameModeBase* GameMode = UGameplayStatics::GetGameMode(this);
	if (IsValid(GameMode) == false) return;

	ACBGameModeBase* CBGameMode = Cast<ACBGameModeBase>(GameMode);
	if (IsValid(CBGameMode) == false) return;

	ACBPlayerState* CBPlayerState = GetPlayerState<ACBPlayerState>();
	if (IsValid(CBPlayerState) == false) return;

	FString Cont = CBPlayerState->PlayerName + ": " + InChatMsg;
	CBGameMode->PrintInChatMsg(this, Cont);
}

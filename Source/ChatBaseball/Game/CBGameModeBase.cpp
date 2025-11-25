// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/CBGameModeBase.h"

#include "Game/CBGameStateBase.h"

#include "Player/CBPlayerController.h"
#include "Player/CBPlayerState.h"

void ACBGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	FinishedPlayerCount = 0;

	GenerateRandomNumber();
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
	if (CheckIsMsgForGame(InChatMsg))
	{
		ACBPlayerState* CBPlayerState = InChatController->GetPlayerState<ACBPlayerState>();
		if (IsValid(CBPlayerState))
		{
			FString Result = JudgeInputString(InChatMsg);
			InChatController->ClientRPC_PrintChatMsg(Result);

			if (Result.LeftChop(2) != "3S")
			{
				CBPlayerState->IncreaseTryCnt();
			}
			JudgeGame(InChatController, Result);
		}
	}
	else
	{
		for (auto e : AllPlayerControllers)
		{
			e->ClientRPC_PrintChatMsg(InChatMsg);
		}
	}
}

void ACBGameModeBase::GenerateRandomNumber()
{
	TSet<int32> RandomNumbers;
	while (RandomNumbers.Num() < 3)
	{
		RandomNumbers.Add(FMath::RandHelper(10));
	}

	AnswerNumber = "";
	for (auto e : RandomNumbers)
	{
		AnswerNumber += FString::FromInt(e);
	}
	UE_LOG(LogTemp, Error, TEXT("Answer Number: %s"), *AnswerNumber);
}

bool ACBGameModeBase::CheckIsMsgForGame(const FString& InChatMsg)
{
	FString Value = InChatMsg.Right(3);
	for (auto e : Value)
	{
		if (!(e >= '0' && e <= '9'))
		{
			return false;
		}
	}

	return true;
}

FString ACBGameModeBase::JudgeInputString(const FString& InChatMsg)
{
	int32 S = 0, B = 0;
	FString Value = InChatMsg.Right(3);

	for (int i = 0; i < Value.Len(); i++)
	{
		FString TempString = FString::Printf(TEXT("%c"), Value[i]);
		if (AnswerNumber.Contains(TempString))
		{
			if (AnswerNumber[i] == Value[i])
			{
				S++;
			}
			else
			{
				B++;
			}
		}
	}

	return (S == 0 && B == 0) ? ("OUT") : (FString::Printf(TEXT("%dS%dB"), S, B));
}

void ACBGameModeBase::JudgeGame(ACBPlayerController* InChatController, const FString& Result)
{
	ACBPlayerState* CBPlayerState = InChatController->GetPlayerState<ACBPlayerState>();
	if (IsValid(CBPlayerState) == false)
	{
		return;
	}

	if (Result.LeftChop(2) == "3S")
	{
		InChatController->GameStateString = "You correct the answer!!!";
		ResetGame(CBPlayerState->PlayerName + TEXT(" has won the game!!!"));
	}
	else
	{
		int32 TryCnt = CBPlayerState->GetCurTryCnt();
		int32 MaxCnt = CBPlayerState->GetMaxTryCnt();

		if (TryCnt >= MaxCnt)
		{
		   InChatController->GameStateString = "You can't try anymore...Wait another player";
		   CBPlayerState->ClearTurnTimer();

		   FinishedPlayerCount++;
		   if (FinishedPlayerCount == AllPlayerControllers.Num())
		   {
		      ResetGame("Draw");
		   }
		}
	}
}

void ACBGameModeBase::ResetGame(const FString& GameStateString)
{
	FinishedPlayerCount = 0;
	GenerateRandomNumber();

	for (auto e : AllPlayerControllers)
	{
		ACBPlayerState* CBPlayerState = e->GetPlayerState<ACBPlayerState>();
		if (IsValid(CBPlayerState))
		{
			CBPlayerState->SetRemainTime(-1);
			CBPlayerState->SetCurTryCnt(0);

			e->GameStateString = GameStateString;
		}
	}
}

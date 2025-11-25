// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CBGameModeBase.generated.h"

class ACBPlayerController;
/**
 * 
 */
UCLASS()
class CHATBASEBALL_API ACBGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;
	virtual void OnPostLogin(AController* NewPlayer) override;

protected:
	TArray<ACBPlayerController*> AllPlayerControllers;
	FString AnswerNumber;

	int32 FinishedPlayerCount;

public:
	void PrintInChatMsg(ACBPlayerController* InChatController, const FString& InChatMsg);

protected:
	void GenerateRandomNumber();

	bool CheckIsMsgForGame(const FString& InChatMsg);
	FString JudgeInputString(const FString& InChatMsg);

/// <summary>
/// Manage Whole Game
/// </summary>
public:
	/// <summary>
	/// Can be called external function
	/// </summary>
	/// <param name="InChatController"></param>
	/// <param name="Result"></param>
	void JudgeGame(ACBPlayerController* InChatController, const FString& Result);
protected:
	void ResetGame(const FString& GameStateString);
};

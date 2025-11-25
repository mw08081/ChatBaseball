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

public:
	void PrintInChatMsg(ACBPlayerController* InChatController, const FString& InChatMsg);

protected:
	void GenerateRandomNumber();

	bool CheckIsMsgForGame(const FString& InChatMsg);
	FString JudgeInputString(const FString& InChatMsg);
};

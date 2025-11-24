// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CBPlayerController.generated.h"


class UChatInput;
/**
 * 
 */
UCLASS()
class CHATBASEBALL_API ACBPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ACBPlayerController();

	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UChatInput> ChatInputWidgetClass;

	UPROPERTY()
	TObjectPtr<UChatInput> ChatInputWidgetInstance;

public:
	void CommitInChatMsg(const FString& InChatMsg);
	void PrintInChatMsg(const FString& InChatMsg);

	UFUNCTION(Server, Reliable)
	void ServerRPC_PrintChatMsg(const FString& InChatMsg);

	UFUNCTION(Client, Reliable)
	void ClientRPC_PrintChatMsg(const FString& InChatMsg);
};

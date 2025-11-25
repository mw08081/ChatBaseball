// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CBPlayerController.generated.h"


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
	//virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

/// <summary>
/// GameStateBoard Widget
/// </summary>
public:
	UPROPERTY(Replicated)
	FString GameStateString;

protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UGameStateBoard> GameStateBoardWidgetClass;

	UPROPERTY()
	TObjectPtr<class UGameStateBoard> GameStateBoardWidgetInstance;

	FTimerHandle UpdateRemainTimeHandle;

protected:
	void UpdateGameeStateBoard();


/// <summary>
/// ChatInput Widget
/// </summary>
protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UChatInput> ChatInputWidgetClass;

	UPROPERTY()
	TObjectPtr<class UChatInput> ChatInputWidgetInstance;

public:
	void CommitInChatMsg(const FString& InChatMsg);
	void PrintInChatMsg(const FString& InChatMsg);

public:
	UFUNCTION(Server, Reliable)
	void ServerRPC_PrintChatMsg(const FString& InChatMsg);

	UFUNCTION(Client, Reliable)
	void ClientRPC_PrintChatMsg(const FString& InChatMsg);
};

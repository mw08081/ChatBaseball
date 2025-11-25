// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameStateBoard.generated.h"

class UTextBlock;

/**
 * 
 */
UCLASS()
class CHATBASEBALL_API UGameStateBoard : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBox_Timer;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBox_GameState;
	

public:
	void SetTextBox_Timer(const FString& Value);
	void SetTextBox_GameState(const FString& GameState);
};

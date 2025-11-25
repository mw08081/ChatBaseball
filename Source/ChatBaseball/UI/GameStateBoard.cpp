// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GameStateBoard.h"

#include "Components/TextBlock.h"

void UGameStateBoard::NativeConstruct()
{
}

void UGameStateBoard::NativeDestruct()
{
}

void UGameStateBoard::SetTextBox_Timer(const FString& Value)
{
	TextBox_Timer->SetText(FText::FromString(Value));
}

void UGameStateBoard::SetTextBox_GameState(const FString& GameState)
{
	TextBox_GameState->SetText(FText::FromString(GameState));
}

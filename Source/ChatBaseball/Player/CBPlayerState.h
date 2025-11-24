// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "CBPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class CHATBASEBALL_API ACBPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	ACBPlayerState();

	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

public:
	UPROPERTY(Replicated)
	FString PlayerName;

protected:
	UPROPERTY(Replicated)
	int32 CurTryCnt;

	UPROPERTY(Replicated)
	int32 MaxTryCnt;

public:
	int32 GetCurTryCnt() const;
	int32 GetMaxTryCnt() const;

public:
	void IncreaseTryCnt();
	
};

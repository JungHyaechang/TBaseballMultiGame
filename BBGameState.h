// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "BBGameState.generated.h"


UCLASS()
class MULTIPLAYNETWORK_API ABBGameState : public AGameState
{
	GENERATED_BODY()
	
	
public:
	ABBGameState();

	UFUNCTION(BlueprintCallable)
	int32 GetRemainingTime() const { return RemainingTime; }

public:
	// 남은 시간
	UPROPERTY(Replicated, BlueprintReadOnly)
	int32 RemainingTime;

	UPROPERTY(Replicated, BlueprintReadOnly)
	APlayerState* CurrentTurnPlayerState;

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
};

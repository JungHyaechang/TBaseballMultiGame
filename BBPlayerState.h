// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "BBPlayerState.generated.h"


UCLASS()
class MULTIPLAYNETWORK_API ABBPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	ABBPlayerState();

	UFUNCTION(BlueprintCallable)
	void AddTry();

	UFUNCTION(BlueprintCallable)
	void AddWin();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	int32 GetTryCount() const { return TryCount; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	int32 GetWinCount() const { return WinCount; }

	UFUNCTION()
	void OnRep_WinCount();

public:
	UPROPERTY(Replicated, BlueprintReadOnly)
	int32 TryCount = 0;

	UPROPERTY(ReplicatedUsing = OnRep_WinCount, BlueprintReadOnly)
	int32 WinCount = 0;

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

};

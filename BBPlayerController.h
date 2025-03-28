// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BBPlayerController.generated.h"



UCLASS()
class MULTIPLAYNETWORK_API ABBPlayerController : public APlayerController
{
	GENERATED_BODY()
	
	
public:
	virtual void BeginPlay() override;

	UFUNCTION(Client, Reliable)
	void Client_UpdateAllWinCounts();

	void ApplyWinscountToChatUIAfterDelay();

};

// Fill out your copyright notice in the Description page of Project Settings.


#include "BBGameState.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"

ABBGameState::ABBGameState()
{
	RemainingTime = 0;
}

void ABBGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ABBGameState, RemainingTime);
}

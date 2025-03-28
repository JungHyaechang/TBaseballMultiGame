// Fill out your copyright notice in the Description page of Project Settings.


#include "BBPlayerState.h"
#include "Net/UnrealNetwork.h"


ABBPlayerState::ABBPlayerState()
{
	bReplicates = true;
}


void ABBPlayerState::AddTry()
{
	TryCount++;
}

void ABBPlayerState::AddWin()
{
	WinCount++;
}

void ABBPlayerState::OnRep_WinCount()
{
	UE_LOG(LogTemp, Warning, TEXT("Wincount Replicated Success : %d"), WinCount);
}

void ABBPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ABBPlayerState, WinCount);
}

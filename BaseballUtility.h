// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class MULTIPLAYNETWORK_API BaseballUtility
{
public:
	BaseballUtility();
	~BaseballUtility();

	static FString GenerateRandomNumber();    
	static FString JudgeGuess(const FString& Answer, const FString& Guess, int32& Strike, int32& Ball);
};

// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseballUtility.h"


BaseballUtility::BaseballUtility()
{
}

BaseballUtility::~BaseballUtility()
{
}

FString BaseballUtility::GenerateRandomNumber()
{
    TArray<int32> Digits = { 1,2,3,4,5,6,7,8,9 };
    FString Result;

    for (int32 i = 0; i < 3; i++)
    {
        int32 Index = FMath::RandRange(0, Digits.Num() - 1);
        Result.AppendInt(Digits[Index]);
        Digits.RemoveAt(Index);
    }

    return Result;
}

FString BaseballUtility::JudgeGuess(const FString& Answer, const FString& Guess, int32& Strike, int32& Ball)
{
    Strike = 0;
    Ball = 0;

    for (int32 i = 0; i < 3; i++)
    {
        if (Answer[i] == Guess[i])
        {
            Strike++;
        }
        else if (Answer.Contains(Guess.Mid(i, 1)))
        {
            Ball++;
        }
    }

    if (Strike == 0 && Ball == 0)
    {
        return TEXT("OUT");
    }

    return FString::Printf(TEXT("%d Strike / %d Ball"), Strike, Ball);
}



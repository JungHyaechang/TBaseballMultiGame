// Fill out your copyright notice in the Description page of Project Settings.


#include "ChatEntryWidget.h"
#include "Components/TextBlock.h"

void UChatEntryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (MessageText)
	{
		MessageText->SetText(FText::FromString(InMessage));
	}
}

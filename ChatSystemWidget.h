// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ChatSystemWidget.generated.h"

class UScrollBox;
class UChatEntryWidget;
class UChatComponent;


UCLASS()
class MULTIPLAYNETWORK_API UChatSystemWidget : public UUserWidget
{
	GENERATED_BODY()
	
	
public:
	UFUNCTION(BlueprintCallable)
	void Scroll(bool bUp);

	UFUNCTION(BlueprintCallable)
	void AddChatMessage(const FString& Message);

	UFUNCTION(BlueprintCallable)
	bool IsChatHovered() const;
	
protected:
	virtual void NativeConstruct() override;
	

protected:
	UPROPERTY(meta = (BindWidget))
	UScrollBox* ChatScrollBox;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Chat")
	TSubclassOf<UChatEntryWidget> ChatEntryClass;

	UPROPERTY()
	UChatComponent* ChatComponentRef;

};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ChatBoxWidget.generated.h"

class UEditableTextBox;
class UScrollBox;
class UChatEntryWidget;
class UChatComponent;

UCLASS()
class MULTIPLAYNETWORK_API UChatBoxWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void AddChatMessage(const FString& Message);

	UFUNCTION(BlueprintCallable)
	void FocusChat();

	UFUNCTION(BlueprintCallable)
	void ExitChat();

	UFUNCTION(BlueprintCallable)
	void Scroll(bool bUp);

	UFUNCTION(BlueprintCallable)
	bool IsChatHovered() const;
protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnChatCommitted(const FText& Text, ETextCommit::Type CommitMethod);

protected:
	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* InputTextBox;

	UPROPERTY(meta = (BindWidget))
	UScrollBox* ChatScrollBox;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Chat")
	TSubclassOf<UChatEntryWidget> ChatEntryClass;

	UPROPERTY()
	UChatComponent* ChatComponentRef;

	bool bChatFocused = false;
};

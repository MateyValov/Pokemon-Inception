// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/WrapBox.h"
#include "MoveButtonWidget.h"
#include "../ButtonClick.h"
#include "FightWidget.generated.h"


UCLASS()
class POKEMONINCEPTION_API UFightWidget : public UUserWidget
{
	GENERATED_BODY()
	
private:
	UFUNCTION()
	void OnBackClicked();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UButton* Back;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UWrapBox* WrapBox;

	virtual void NativeConstruct() override;

public:
	UFUNCTION()
	void AddToWrapBox(UMoveButtonWidget* MoveButton);

	void ClearWrapBox();

	FButtonClicked BackClicked;
};

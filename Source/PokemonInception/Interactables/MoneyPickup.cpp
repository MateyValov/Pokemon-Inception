// Fill out your copyright notice in the Description page of Project Settings.


#include "MoneyPickup.h"
#include "Components/StaticMeshComponent.h"
#include "../Player/PlayerCharacterController.h"
#include "../UI/OverworldUI/OverworldHUD.h"

AMoneyPickup::AMoneyPickup()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	Action = "pick up";
	money = 100;
}

void AMoneyPickup::Interact(APlayerController* Controller)
{
	APlayerCharacterController* PlayerController = Cast<APlayerCharacterController>(Controller);
	if (PlayerController == nullptr) {
		return;
	}
	
	AOverworldHUD* Hud = Cast<AOverworldHUD>(PlayerController->GetHUD());
	if (Hud == nullptr) {
		return;
	}
	
	Hud->OnScreenMessage("You got " + FString::FromInt(money) + "$!");
	PlayerController->RecieveMoney(money);
	Destroy();
	
}
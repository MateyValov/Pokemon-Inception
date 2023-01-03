 // Fill out your copyright notice in the Description page of Project Settings.


#include "BattleHUD.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "../../GameModes/BattleGameMode.h"
#include "../../Player/PlayerCharacterController.h"
#include "Engine/Engine.h"

void ABattleHUD::BeginPlay()
{
	Super::BeginPlay();
	
	ABattleGameMode* GameMode = Cast<ABattleGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	BattleStartWidget = CreateWidget<UBattleStartWidget>(UGameplayStatics::GetGameInstance(GetWorld()), BattleStartWidgetClass);
	FightWidget = CreateWidget<UFightWidget>(UGameplayStatics::GetGameInstance(GetWorld()), FightWidgetClass);
	PokemonWidget = CreateWidget<UPokemonWidget>(UGameplayStatics::GetGameInstance(GetWorld()), PokemonWidgetClass);
	PlayerPokemonStatusWidget = CreateWidget<UPokemonStatusWidget>(UGameplayStatics::GetGameInstance(GetWorld()), PokemonStatusWidgetClass);
	OpponentPokemonStatusWidget = CreateWidget<UPokemonStatusWidget>(UGameplayStatics::GetGameInstance(GetWorld()), OpponentStatusWidgetClass);
	BagWidget = CreateWidget<UBagWidget>(UGameplayStatics::GetGameInstance(GetWorld()), BagWidgetClass);
	TextBoxWidget = CreateWidget<UTextBoxWidget>(UGameplayStatics::GetGameInstance(GetWorld()), TextBoxWidgetClass);

	BattleStartWidget->FightClicked.AddDynamic(this, &ABattleHUD::ShowFightWidget);
	BattleStartWidget->PokemonClicked.AddDynamic(this, &ABattleHUD::ShowPokemon);
	BattleStartWidget->BagClicked.AddDynamic(this, &ABattleHUD::ShowBag);
	BattleStartWidget->RunClicked.AddDynamic(GameMode, &ABattleGameMode::Run);

	FightWidget->BackClicked.AddDynamic(this, &ABattleHUD::ShowBattleStartWidget);
	PokemonWidget->BackClicked.AddDynamic(this, &ABattleHUD::ShowBattleStartWidget);
	BagWidget->BackClicked.AddDynamic(this, &ABattleHUD::ShowBattleStartWidget);

	GameMode->MessageUpdate.AddDynamic(TextBoxWidget, &UTextBoxWidget::ReturnMessage);
	GameMode->WidgetUpdate.AddDynamic(this, &ABattleHUD::ShowWidget);
	GameMode->ItemSlotDelegate.AddDynamic(BagWidget, &UBagWidget::AddToWrapBox);
	GameMode->ItemInfoDelegate.AddDynamic(BagWidget, &UBagWidget::ShowInfo);
	GameMode->PokemonSlotDelegate.AddDynamic(PokemonWidget, &UPokemonWidget::AddToWrapBox);

	ShowText("A wild Pokemon appeared!", Cast<UUserWidget>(BattleStartWidget));
}

TSubclassOf<UItemSlotWidget> ABattleHUD::GetItemSlotWidgetClass()
{
	return ItemSlotWidgetClass; 
}

TSubclassOf<UItemInfoWidget> ABattleHUD::GetItemInfoWidgetClass()
{
	return ItemInfoWidgetClass;
}

TSubclassOf<UPokemonSlotWidget> ABattleHUD::GetPokemonSlotWidgetClass()
{
	return PokemonSlotWidgetClass;
}

void ABattleHUD::Clear()
{
	UWidgetLayoutLibrary::RemoveAllWidgets(this);
}

void ABattleHUD::ShowWidget(class UUserWidget* Widget)
{
	Clear();

	if (PlayerOwner && Widget) {
		Widget->AddToViewport();

		UBattleStartWidget* BTWidget = Cast<UBattleStartWidget>(Widget);
		if (BTWidget != nullptr) {
			ShowPlayerPokemonStatus();
			ShowOpponentPokemonStatus();
		}

		PlayerOwner->bShowMouseCursor = true;
		PlayerOwner->SetInputMode(FInputModeUIOnly());
	}
}

void ABattleHUD::ShowText(FString Message, class UUserWidget* NextWidget)
{
	Clear();
	ABattleGameMode* GameMode = Cast<ABattleGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	if (PlayerOwner && TextBoxWidget) {
		TextBoxWidget->AddToViewport();
		PlayerOwner->bShowMouseCursor = false;
		PlayerOwner->SetInputMode(FInputModeUIOnly());
		GameMode->DisplayMessage(Message, NextWidget);
	}
}

void ABattleHUD::ShowFightWidget()
{
	Clear();

	if (PlayerOwner && FightWidget) {
		ShowPlayerPokemonStatus();
		ShowOpponentPokemonStatus();
		FightWidget->AddToViewport();
		PlayerOwner->bShowMouseCursor = true;
		PlayerOwner->SetInputMode(FInputModeUIOnly());
	}
}

void ABattleHUD::ShowBag()
{
	Clear();
	ABattleGameMode* GameMode = Cast<ABattleGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GameMode == nullptr) {
		return;
	}

	if (PlayerOwner && BagWidget) {
		BagWidget->ClearWrapBox();
		BagWidget->ClearInfoBox();
		GameMode->FillBagWidget();
		BagWidget->AddToViewport();
		PlayerOwner->bShowMouseCursor = true;
		PlayerOwner->SetInputMode(FInputModeUIOnly());
	}
}

void ABattleHUD::ShowPokemon()
{
	Clear();
	ABattleGameMode* GameMode = Cast<ABattleGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GameMode == nullptr) {
		return;
	}

	if (PlayerOwner && PokemonWidget) {
		PokemonWidget->ClearWrapBox();
		GameMode->ShowPokemonInMenu();
		PokemonWidget->AddToViewport();
		PlayerOwner->bShowMouseCursor = true;
		PlayerOwner->SetInputMode(FInputModeUIOnly());
	}
}

void ABattleHUD::ShowPlayerPokemonStatus()
{
	if (PlayerOwner && PlayerPokemonStatusWidget) {
		RefreshPlayerPokemonStatus();
		PlayerPokemonStatusWidget->AddToViewport();
		PlayerOwner->bShowMouseCursor = true;
		PlayerOwner->SetInputMode(FInputModeUIOnly());
	}
}

void ABattleHUD::ShowOpponentPokemonStatus()
{
	if (PlayerOwner && OpponentPokemonStatusWidget) {
		RefreshOpponentPokemonStatus();
		OpponentPokemonStatusWidget->AddToViewport();
		PlayerOwner->bShowMouseCursor = true;
		PlayerOwner->SetInputMode(FInputModeUIOnly());
	}
}

void ABattleHUD::RefreshPlayerPokemonStatus()
{
	if (PlayerOwner && PlayerPokemonStatusWidget) {
		APlayerCharacterController* Controller = Cast<APlayerCharacterController>(PlayerOwner);
		PlayerPokemonStatusWidget->SetPokemonHP(Controller->GetPokemonParty()[0].CurrHP, Controller->GetPokemonParty()[0].MaxHP);
		PlayerPokemonStatusWidget->SetPokemonLevel(Controller->GetPokemonParty()[0].Level);
		PlayerPokemonStatusWidget->SetPokemonName(Controller->GetPokemonParty()[0].SpeciesData.Name);
	}
}

void ABattleHUD::RefreshOpponentPokemonStatus()
{
	ABattleGameMode* GameMode = Cast<ABattleGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GameMode == nullptr) {
		return;
	}

	FPokemonStruct Opponent = GameMode->GetCurrentOpponent();
	OpponentPokemonStatusWidget->SetPokemonHP(Opponent.CurrHP, Opponent.MaxHP);
	OpponentPokemonStatusWidget->SetPokemonLevel(Opponent.Level);
	OpponentPokemonStatusWidget->SetPokemonName(Opponent.SpeciesData.Name);
}

void ABattleHUD::ShowBattleStartWidget()
{
	Clear();

	if (PlayerOwner && BattleStartWidget) {
		BattleStartWidget->AddToViewport();
		ShowPlayerPokemonStatus();
		ShowOpponentPokemonStatus();
		PlayerOwner->bShowMouseCursor = true;
		PlayerOwner->SetInputMode(FInputModeUIOnly());
	}
}

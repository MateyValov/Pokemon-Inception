// Fill out your copyright notice in the Description page of Project Settings.


#include "ChasePlayer.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "../WildPokemon_AIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BB_Keys.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"


UChasePlayer::UChasePlayer()
{
	NodeName = TEXT("Chase Player");
}

EBTNodeResult::Type UChasePlayer::ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory)
{
	AWildPokemon_AIController* Controller = Cast<AWildPokemon_AIController>(ownerComp.GetAIOwner());
	FVector const PlayerLocation = Controller->getBlackboard()->GetValueAsVector(bb_keys::targetLocation);

	AWildPokemon* PossesedPokemon = Cast<AWildPokemon>(Controller->GetPawn());
	if (IsValid(PossesedPokemon)) {
		PossesedPokemon->GetCharacterMovement()->MaxWalkSpeed = PossesedPokemon->RunningSpeed;
	}

	UAIBlueprintHelperLibrary::SimpleMoveToLocation(Controller, PlayerLocation);

	FinishLatentTask(ownerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}

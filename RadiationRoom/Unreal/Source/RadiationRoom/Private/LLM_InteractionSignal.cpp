// Fill out your copyright notice in the Description page of Project Settings.


#include "LLM_InteractionSignal.h"
#include "GameFramework/Actor.h"

ULLM_InteractionSignal::ULLM_InteractionSignal()
{

}

UWorld* ULLM_InteractionSignal::GetWorld() const
{
	if (IsValid(instigator)){
		return instigator->GetWorld();
	}
	else return nullptr;
}

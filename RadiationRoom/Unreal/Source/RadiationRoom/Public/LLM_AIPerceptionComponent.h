// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Perception/AIPerceptionComponent.h"
#include "LLM_AIPerceptionComponent.generated.h"

/**
 * 
 */
UCLASS()
class RADIATIONROOM_API ULLM_AIPerceptionComponent : public UAIPerceptionComponent
{
	GENERATED_BODY()
	
public:
	ULLM_AIPerceptionComponent(const FObjectInitializer& ObjectInitializer);
};

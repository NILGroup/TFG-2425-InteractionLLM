// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "./LLMCreateComponent.h"
#include "Projectile.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RADIATIONROOM_API UProjectile : public ULLMCreateComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UProjectile();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	float _speed = 0.0f;
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void DeserializeComponent(FString  jsonObject) override;
};

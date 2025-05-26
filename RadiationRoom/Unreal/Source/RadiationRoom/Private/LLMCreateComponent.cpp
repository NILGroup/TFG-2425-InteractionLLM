// Fill out your copyright notice in the Description page of Project Settings.


#include "LLMCreateComponent.h"

// Sets default values for this component's properties
ULLMCreateComponent::ULLMCreateComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void ULLMCreateComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void ULLMCreateComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

FString ULLMCreateComponent::GetSerializedComponent()
{
	return _serializedComponent = "{\"name\":\"" + _name + "\", \"additional_info\":" + _serializedComponent + "}";
}

FString ULLMCreateComponent::GetName()
{
	return _name;
}

FString ULLMCreateComponent::GetDescription()
{
	return _description;
}


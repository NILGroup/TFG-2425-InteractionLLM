// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LLMCreateComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RADIATIONROOM_API ULLMCreateComponent : public UChildActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ULLMCreateComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	FString _serializedComponent;
	FString _name;
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UFUNCTION(BlueprintCallable)
	FString GetSerializedComponent();
	UFUNCTION(BlueprintCallable)
	virtual void DeserializeComponent(FString jsonObject) {};
	UFUNCTION(BlueprintCallable)
	FString GetName();
};

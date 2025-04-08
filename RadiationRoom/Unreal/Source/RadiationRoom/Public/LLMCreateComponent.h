// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LLMCreateComponent.generated.h"


UCLASS(Blueprintable)
class RADIATIONROOM_API ULLMCreateComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ULLMCreateComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString _serializedComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString _name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString _description;
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UFUNCTION(BlueprintCallable)
	FString GetSerializedComponent();
	UFUNCTION(BlueprintCallable)
	virtual void DeserializeComponent(FString jsonObject) {};
	UFUNCTION(BlueprintCallable)
	FString GetName();
	UFUNCTION(BlueprintCallable)
	FString GetDescription();
};

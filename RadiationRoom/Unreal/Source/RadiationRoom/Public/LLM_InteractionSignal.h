// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/World.h"
#include "LLM_InteractionSignal.generated.h"

USTRUCT(BlueprintType, Blueprintable)
struct FSignalAnswer {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString prompt;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<AActor*> affectedActors;
};
/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class RADIATIONROOM_API ULLM_InteractionSignal : public UObject
{
	GENERATED_BODY()

public:
	ULLM_InteractionSignal();

	UFUNCTION(BlueprintCallable)
	UWorld* GetWorld() const override;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Functions")
	bool ExecuteSignal();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Functions")
	FSignalAnswer GetSignalPrompt();
protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Properties", meta = (AllowPrivateAccess = "true"))
	FString name;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Properties", meta = (AllowPrivateAccess = "true"))
	FString description;

	UPROPERTY(BlueprintReadWrite, Category = "Properties", meta = (AllowPrivateAccess = "true", ExposeOnSpawn = "true"))
	TObjectPtr<class AActor> instigator;
};

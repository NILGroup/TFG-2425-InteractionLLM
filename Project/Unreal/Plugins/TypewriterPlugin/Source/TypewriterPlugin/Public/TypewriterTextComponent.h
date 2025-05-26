/*
* Typewriter Plugin - Unreal Engine 5 Typewriter Plugin
*
* Copyright (C) 2022 Mykhailo Oliinyk <m19tes@gmail.com> All Rights Reserved.
*/

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TypewriterTextComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCompleted);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class UTypewriterTextComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UTypewriterTextComponent();

private:
	UPROPERTY()
	class URichTextBlock* RichText;
	
	UPROPERTY()
	bool isActiveWriting;
	
	UPROPERTY()
	FTimerHandle TimerHandle;
	
	UPROPERTY()
	FTimerHandle TimerDelay;
	
	UPROPERTY()
	FText Text;
	UPROPERTY()
	class UTextBlock* TextBlock;
	
	UPROPERTY()
	float CharsPerSecond;
	
	UPROPERTY()
	float PunctuationDel;
	
	UPROPERTY()
	int32 CharNum;
	
	UPROPERTY()
	int32 Len;
	
	UPROPERTY()
	USoundBase* Sound;
	
	UPROPERTY()
	class UAudioComponent* AudioComponent;
	
	UPROPERTY()
	bool isRichText;
	
	UPROPERTY()
	int32 Acurracy;
	
	UPROPERTY()
	float Acceleration;
	
	UPROPERTY()
	bool bInAceleration;
	
	UPROPERTY()
	FString ChachedString;
	
	UPROPERTY()
	bool CanCallCompletedDelegate;


	UFUNCTION()
	void StartTimer();
	UFUNCTION()
	void StopTimer(bool Pause, bool Destroy);
	UFUNCTION()
	void PrintText();
	UFUNCTION()
	void PunctuationDelay();


public:
	UFUNCTION(BlueprintCallable, Category="TypeWriter" )
	void StartTypewriting(const FText InText, UTextBlock* InTextBlock, const float InCharsPerSec,
	                      const float InPunctuationDel, USoundBase* InSound, const int32 StartFrom = 0);

	UFUNCTION(BlueprintCallable, Category="TypeWriter")
	void StartRichTypewriting(const FText InText, URichTextBlock* InRichTextBlockTextBlock, const float InCharsPerSec,
	                          const float InPunctuationDel, USoundBase* InSound, int32 InAccuracy, const int32 StartFrom = 0);

	UFUNCTION(BlueprintCallable, Category="TypeWriter")
	void StopPrinting(float InAcceleration);

	UPROPERTY(BlueprintAssignable, VisibleAnywhere, Category="TypeWriter")
	FOnCompleted OnCompleted;
};

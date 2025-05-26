/*
* Typewriter Plugin - Unreal Engine 5 Typewriter Plugin
*
* Copyright (C) 2022 Mykhailo Oliinyk <m19tes@gmail.com> All Rights Reserved.
*/

#pragma once


#include "CoreMinimal.h"
#include "TypewriterTextComponent.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CPP_TypewriterLibrary.generated.h"

/**
 * 
 */
UCLASS()
class UCPP_TypewriterLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()
	UFUNCTION(BlueprintCallable,
		meta = (DeprecatedFunction, DeprecationMessage="Use the WriteText function", WorldContext = "Object",
			HideSelfPin, CharsPerSec
			=10.f, PunctuationDelay=0.5f), Category = "TypeWriter")
	static UTypewriterTextComponent* PrintText(UObject* Object, const FText Text, const float CharsPerSec,
	                                           const float PunctuationDelay, USoundBase* Sound,
	                                           class UTextBlock* TextBlock);


	UFUNCTION(BlueprintCallable,
		meta = (DeprecatedFunction, DeprecationMessage="Use the WriteText function", WorldContext = "Object",
			HideSelfPin, CharsPerSec
			=10.f, PunctuationDelay=0.5f, Accuracy=10),
		Category = "TypeWriter")
	static UTypewriterTextComponent* PrintRichText(UObject* Object, const FText Text, const float CharsPerSec,
	                                               const float PunctuationDelay,
	                                               const int32 Accuracy, USoundBase* Sound,
	                                               class URichTextBlock* RichTextBlock);


	UFUNCTION(BlueprintCallable,
		meta = (WorldContext = "Object", AdvancedDisplay="Accuracy, Sound, StartFrom", HideSelfPin, CharsPerSec=10.f,
			PunctuationDelay=0.5f, Accuracy=10),
		Category = "TypeWriter")
	static UTypewriterTextComponent* WriteText(UObject* Object, const FText Text, const float CharsPerSec,
	                                           const float PunctuationDelay,
	                                           class UTextLayoutWidget* TextBlock, const int32 StartFrom,
	                                           const int32 Accuracy, USoundBase* Sound);


	UFUNCTION(BlueprintCallable, meta = (WorldContext = "Object", HideSelfPin), Category = "TypeWriter")
	static void StopPrinting(UObject* Object, float Acceleration);

	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (WorldContext = "Object", HideSelfPin), Category = "TypeWriter")
	static UTypewriterTextComponent* GetTypeWriterComponent(UObject* Object);
};

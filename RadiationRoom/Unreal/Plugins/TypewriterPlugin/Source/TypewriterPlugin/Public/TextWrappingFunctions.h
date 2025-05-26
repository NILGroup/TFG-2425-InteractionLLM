/*
* Typewriter Plugin - Unreal Engine 5 Typewriter Plugin
*
* Copyright (C) 2022 Mykhailo Oliinyk <m19tes@gmail.com> All Rights Reserved.
*/

#pragma once

#include "CoreMinimal.h"
#include "Components/TextBlock.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "TextWrappingFunctions.generated.h"

/**
 * 
 */
UCLASS()
class TYPEWRITERPLUGIN_API UTextWrappingFunctions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()


public:
	
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (DisplayName = "Justify Text", Keywords = "Justify Text"),
		Category = "Justify Text")
	static FText JustifyText(const FText Text, const float Width, const FSlateFontInfo Font);


	static void GetJustifiedText(const FString& CachedString, const int Index,
	                             const TSharedRef<class FSlateFontMeasure> FontMeasure,
	                             FString& GlobalString, const FSlateFontInfo Font, const float Width);
};

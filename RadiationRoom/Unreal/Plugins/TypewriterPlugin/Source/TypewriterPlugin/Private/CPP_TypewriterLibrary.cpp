/*
* Typewriter Plugin - Unreal Engine 5 Typewriter Plugin
*
* Copyright (C) 2022 Mykhailo Oliinyk <m19tes@gmail.com> All Rights Reserved.
*/

#include "CPP_TypewriterLibrary.h"
#include "TypewriterTextComponent.h"
#include "Components/RichTextBlock.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TextBlock.h"
#include "GameFramework/PlayerController.h"


UCPP_TypewriterLibrary::UCPP_TypewriterLibrary(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

UTypewriterTextComponent* UCPP_TypewriterLibrary::PrintText(UObject* Object, const FText Text, const float CharsPerSec,
                                                            const float PunctuationDelay, USoundBase* Sound,
                                                            UTextBlock* TextBlock)
{
	if (const UWorld* World = GEngine->GetWorldFromContextObjectChecked(Object))
	{
		if (APlayerController* PC = UGameplayStatics::GetPlayerController(World, 0))
		{
			if (UActorComponent* TextComponent = PC->GetComponentByClass(UTypewriterTextComponent::StaticClass()))
			{
				UTypewriterTextComponent* Component = Cast<UTypewriterTextComponent>(TextComponent);
				Component->StartTypewriting(Text, TextBlock, CharsPerSec, PunctuationDelay, Sound);
				return Component;
			}
			UActorComponent* TextComponentPC = PC->AddComponentByClass(
				UTypewriterTextComponent::StaticClass(), false, FTransform::Identity, false);
			UTypewriterTextComponent* Component = Cast<UTypewriterTextComponent>(TextComponentPC);
			if (Component)
				Component->StartTypewriting(Text, TextBlock, CharsPerSec, PunctuationDelay, Sound);
			return Component;
		}
	}
	return nullptr;
}

void UCPP_TypewriterLibrary::StopPrinting(UObject* Object, float Acceleration)
{
	if (const UWorld* World = GEngine->GetWorldFromContextObjectChecked(Object))
	{
		if (const APlayerController* PC = UGameplayStatics::GetPlayerController(World, 0))
		{
			if (UActorComponent* TextComponent = PC->GetComponentByClass(UTypewriterTextComponent::StaticClass()))
			{
				UTypewriterTextComponent* Component = Cast<UTypewriterTextComponent>(TextComponent);
				Component->StopPrinting(Acceleration);
			}
		}
	}
}

UTypewriterTextComponent* UCPP_TypewriterLibrary::PrintRichText(UObject* Object, const FText Text,
                                                                const float CharsPerSec,
                                                                const float PunctuationDelay, const int32 Accuracy,
                                                                USoundBase* Sound,
                                                                URichTextBlock* RichTextBlock)
{
	if (const UWorld* World = GEngine->GetWorldFromContextObjectChecked(Object))
	{
		if (APlayerController* PC = UGameplayStatics::GetPlayerController(World, 0))
		{
			if (UActorComponent* TextComponent = PC->GetComponentByClass(UTypewriterTextComponent::StaticClass()))
			{
				UTypewriterTextComponent* Component = Cast<UTypewriterTextComponent>(TextComponent);
				if (Component)
					Component->StartRichTypewriting(Text, RichTextBlock, CharsPerSec, PunctuationDelay, Sound,
					                                Accuracy);
				return Component;
			}
			UActorComponent* TextComponentPC = PC->AddComponentByClass(
				UTypewriterTextComponent::StaticClass(), false, FTransform::Identity, false);
			UTypewriterTextComponent* Component = Cast<UTypewriterTextComponent>(TextComponentPC);
			if (Component)
				Component->StartRichTypewriting(Text, RichTextBlock, CharsPerSec, PunctuationDelay, Sound, Accuracy);
			return Component;
		}
	}
	return nullptr;
}

UTypewriterTextComponent* UCPP_TypewriterLibrary::WriteText(UObject* Object, const FText Text, const float CharsPerSec,
                                                            const float PunctuationDelay, UTextLayoutWidget* TextBlock,
                                                            const int32 StartFrom, const int32 Accuracy,
                                                            USoundBase* Sound)
{
	if (const UWorld* World = GEngine->GetWorldFromContextObjectChecked(Object))
	{
		if (APlayerController* PC = UGameplayStatics::GetPlayerController(World, 0))
		{
			if (UActorComponent* TextComponent = PC->GetComponentByClass(UTypewriterTextComponent::StaticClass()))
			{
				UTypewriterTextComponent* Component = Cast<UTypewriterTextComponent>(TextComponent);
				if (Component)
				{
					if (URichTextBlock* RichTextBlock = Cast<URichTextBlock>(TextBlock))
					{
						Component->StartRichTypewriting(Text, RichTextBlock, CharsPerSec, PunctuationDelay, Sound,
						                                Accuracy, StartFrom);
					}
					else if (UTextBlock* TextBlockRef = Cast<UTextBlock>(TextBlock))
					{
						Component->StartTypewriting(Text, TextBlockRef, CharsPerSec, PunctuationDelay, Sound,
						                            StartFrom);
					}
				}
				return Component;
			}
			UActorComponent* TextComponentPC = PC->AddComponentByClass(
				UTypewriterTextComponent::StaticClass(), false, FTransform::Identity, false);
			UTypewriterTextComponent* Component = Cast<UTypewriterTextComponent>(TextComponentPC);
			if (Component) {
				if (URichTextBlock* RichTextBlock = Cast<URichTextBlock>(TextBlock))
				{
					Component->StartRichTypewriting(Text, RichTextBlock, CharsPerSec, PunctuationDelay, Sound,
						Accuracy, StartFrom);
				}
				else if (UTextBlock* TextBlockRef = Cast<UTextBlock>(TextBlock))
				{
					Component->StartTypewriting(Text, TextBlockRef, CharsPerSec, PunctuationDelay, Sound, StartFrom);
				}
			}
				

			return Component;
		}
	}
	return nullptr;
}

UTypewriterTextComponent* UCPP_TypewriterLibrary::GetTypeWriterComponent(UObject* Object)
{
	if (const UWorld* World = GEngine->GetWorldFromContextObjectChecked(Object))
	{
		if (APlayerController* PC = UGameplayStatics::GetPlayerController(World, 0))
		{
			if (UActorComponent* TextComponent = PC->GetComponentByClass(UTypewriterTextComponent::StaticClass()))
			{
				UTypewriterTextComponent* Component = Cast<UTypewriterTextComponent>(TextComponent);
				if (Component)
					return Component;
			}
			UActorComponent* TextComponentPC = PC->AddComponentByClass(
				UTypewriterTextComponent::StaticClass(), false, FTransform::Identity, false);
			UTypewriterTextComponent* Component = Cast<UTypewriterTextComponent>(TextComponentPC);
			if (Component)
				return Component;
		}
	}
	return nullptr;
}

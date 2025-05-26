/*
* Typewriter Plugin - Unreal Engine 5 Typewriter Plugin
*
* Copyright (C) 2022 Mykhailo Oliinyk <m19tes@gmail.com> All Rights Reserved.
*/

#include "TypewriterTextComponent.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "Kismet/KismetTextLibrary.h"
#include "Components/TextBlock.h"
#include "Components/RichTextBlock.h"
#include "Kismet/KismetStringLibrary.h"


// Sets default values for this component's properties
UTypewriterTextComponent::UTypewriterTextComponent():
isActiveWriting(false),
Acceleration(1.f),
bInAceleration(false),
CanCallCompletedDelegate(true)
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UTypewriterTextComponent::StartTypewriting(const FText InText, UTextBlock* InTextBlock,
                                                const float InCharsPerSec, const float InPunctuationDel,
                                                USoundBase* InSound, const int32 StartFrom)
{
	if (isActiveWriting)
	{
		StopTimer(false, true);
		if (IsValid(TextBlock))
		{
			TextBlock->SetText(Text);
			TextBlock = nullptr;
		}
		if (IsValid(RichText))
		{
			RichText->SetText(Text);
			RichText = nullptr;
		}
		isActiveWriting = false;
	}
	CanCallCompletedDelegate = true;
	bInAceleration = false;
	Acceleration = 1.f;
	isActiveWriting = true;
	isRichText = false;
	Sound = InSound;
	Text = InText;
	TextBlock = InTextBlock;
	CharsPerSecond = 1 / InCharsPerSec;
	PunctuationDel = InPunctuationDel;
	CharNum = StartFrom;
	ChachedString = Text.ToString();
	Len = ChachedString.Len();
	if (Len == -1)
	{
		if (IsValid(TextBlock)) TextBlock->SetText(Text);
		return;
	}
	if (IsValid(TextBlock)) TextBlock->SetText(FText());
	StartTimer();
}


void UTypewriterTextComponent::PrintText()
{
	FString LeftString = ChachedString.Left(CharNum);
	const FString LastChar = LeftString.Right(1);
	if (isRichText)
	{

		int32 TempAcurracy = Acurracy;
		const FString RightString = ChachedString.RightChop(CharNum).Left(Acurracy+1);
		const int32 TempSpaceIndex = UKismetStringLibrary::FindSubstring(RightString,TEXT("\n"));
	
		if(TempSpaceIndex >= 0)
		{
			TempAcurracy = TempSpaceIndex-1;
		}
		if (IsValid(RichText))
			RichText->SetText(UKismetTextLibrary::Conv_StringToText(LeftString.Append(FString(TEXT("<I>")))
			.Append(ChachedString.RightChop(CharNum).Left(TempAcurracy)).Append(FString(TEXT("</>")))));
	}
	else
	{
		if (IsValid(TextBlock)) TextBlock->SetText(UKismetTextLibrary::Conv_StringToText(LeftString));
		else StopTimer(false, true);
	}
	
	while(ChachedString.Left(CharNum).Right(1) == " " && ChachedString.Left(CharNum+1).Right(1) == " " && CharNum < Len)
	{
		CharNum++;
	}
	
	CharNum++; 
	if (CharNum > Len)
	{
		StopTimer(false, true);
		if(CanCallCompletedDelegate)
		{
			OnCompleted.Broadcast();
			CanCallCompletedDelegate = false;
		}
		
	}
	else if (PunctuationDel > 0.0f && (LastChar == "!" || LastChar == "." || LastChar == "," || LastChar == "?"))
	{
		StopTimer(true, false);
		PunctuationDelay();
	}
}


void UTypewriterTextComponent::PunctuationDelay()
{
	GetWorld()->GetTimerManager().SetTimer(TimerDelay, this, &UTypewriterTextComponent::StartTimer,
	                                       PunctuationDel / Acceleration, false, -1);
}

void UTypewriterTextComponent::StartTimer()
{
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UTypewriterTextComponent::PrintText,
	                                       CharsPerSecond / Acceleration, true, 0);

	if (AudioComponent != nullptr)
	{
		if (AudioComponent->bIsPaused)
		{
			AudioComponent->SetPaused(false);
			return;
		}
	}
	if (Sound != nullptr)
		AudioComponent = UGameplayStatics::SpawnSound2D(this, Sound, 1.f, 1.f,
		                                                0, nullptr, false, true);
}

void UTypewriterTextComponent::StopTimer(bool Pause, bool Destroy)
{
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	TimerHandle.Invalidate();
	if (AudioComponent != nullptr)
	{
		if (Destroy)
		{
			AudioComponent->Deactivate();
			return;
		}
		if (AudioComponent->IsActive())
			AudioComponent->SetPaused(Pause);
	}
}

void UTypewriterTextComponent::StopPrinting(float InAcceleration)
{
	GetWorld()->GetTimerManager().ClearTimer(TimerDelay);
	TimerDelay.Invalidate();

	if (!bInAceleration && InAcceleration > 1.f)
	{
		bInAceleration = true;
		Acceleration = InAcceleration;
		StopTimer(true, false);
		StartTimer();
		return;
	}
	StopTimer(false, true);

	if (!isRichText)
	{
		if (IsValid(TextBlock))
		{
			TextBlock->SetText(Text);
		}
	}
	else
	{
		if (IsValid(RichText))
		{
			RichText->SetText(Text);
		}
		
	}
	if(CanCallCompletedDelegate)
	{
		OnCompleted.Broadcast();
		CanCallCompletedDelegate = false;
	}
}


void UTypewriterTextComponent::StartRichTypewriting(const FText InText, URichTextBlock* InRichTextBlockTextBlock,
                                                    const float InCharsPerSec,
                                                    const float InPunctuationDel, USoundBase* InSound,
                                                    int32 InAccuracy, const int32 StartFrom)
{
	if (isActiveWriting)
	{
		StopTimer(false, true);
		if (IsValid(TextBlock))
		{
			TextBlock->SetText(Text);
			TextBlock = nullptr;
		}
		if (IsValid(RichText))
		{
			RichText->SetText(Text);
			RichText = nullptr;
		}
		isActiveWriting = false;
	}
	CanCallCompletedDelegate = true;
	isActiveWriting = true;
	bInAceleration = false;
	Acurracy = InAccuracy;
	Acceleration = 1.f;
	isRichText = true;
	Sound = InSound;
	Text = InText;
	RichText = InRichTextBlockTextBlock;
	CharsPerSecond = 1 / InCharsPerSec;
	PunctuationDel = InPunctuationDel;
	CharNum = StartFrom;
	ChachedString = Text.ToString();
	Len = ChachedString.Len();
	if (Len == -1)
	{
		if (IsValid(TextBlock)) TextBlock->SetText(Text);
		return;
	}
	if (IsValid(RichText)) RichText->SetText(FText());
	StartTimer();
}

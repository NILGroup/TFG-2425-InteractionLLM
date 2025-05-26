/*
* Typewriter Plugin - Unreal Engine 5 Typewriter Plugin
*
* Copyright (C) 2022 Mykhailo Oliinyk <m19tes@gmail.com> All Rights Reserved.
*/


#include "TextWrappingFunctions.h"
#include "Components/TextBlock.h"
#include "Fonts/FontMeasure.h"
#include "Framework/Application/SlateApplication.h"


FText UTextWrappingFunctions::JustifyText(const FText Text, const float Width, const FSlateFontInfo Font)
{
	const TSharedRef<FSlateFontMeasure> FontMeasure = FSlateApplication::Get().GetRenderer()->
	                                                                           GetFontMeasureService();
	const FString StringText = Text.ToString();

	FString GlobalString;
	GetJustifiedText(StringText, 0, FontMeasure, GlobalString, Font,
	                 Width);
	return FText::FromString(GlobalString);
}

void UTextWrappingFunctions::GetJustifiedText(const FString& CachedString, const int Index,
                                              const TSharedRef<FSlateFontMeasure> FontMeasure,
                                              FString& GlobalString, const FSlateFontInfo Font, const float Width)
{
	FString TempString;
	TArray<int32> Spaces;
	const int32 TempIndex = Index;
	for (int x = Index; x < CachedString.Len(); x++)
	{
		TempString += CachedString[x];
		if (CachedString[x] == ' ')
		{
			Spaces.Add(x - TempIndex + 1);
		}

		if (FontMeasure->Measure(TempString, Font).X > Width)
		{
			int32 Len;
			FString str;
			if (Spaces.IsValidIndex(0))
			{
				str = TempString.Left(Spaces[Spaces.Num() - 1]);
			}
			else
			{
				str = TempString;
			}
			if (Spaces.Num() > 1)
			{
				Len = Index + (Spaces[Spaces.Num() - 1]);
				const auto LastChar = str[str.Len() - 2];
				FString LastString;
				LastString += LastChar;

				//if (LastString != "." && LastString != "!" && LastString != "?" && LastString != ":" && LastString != ",")
				//{
				Spaces.RemoveAt(Spaces.Num() - 1);
				int32 count = 0;
				while (FontMeasure->Measure(str, Font).X < Width)
				{
					if (Spaces.IsValidIndex(count))
					{
						str.InsertAt(Spaces[count] + count, " ");
						Spaces[count] = Spaces[count] + count;
						count++;
					}
					else
					{
						str.InsertAt(Spaces[0], " ");
						Spaces[0] = Spaces[0];
						count = 1;
					}
				}
				//}
			}
			else
			{
				if (Spaces.IsValidIndex(0))
				{
					Len = Index + (Spaces[Spaces.Num() - 1]);
				}
				else
				{
					Len = Index + (x - TempIndex + 1);
				}
			}

			str += FString("\n");
			GlobalString.Append(str);
			GetJustifiedText(CachedString, Len, FontMeasure, GlobalString, Font, Width);
			break;
		}

		if (x == CachedString.Len() - 1)
		{
			GlobalString.Append(TempString);
		}
	}
}

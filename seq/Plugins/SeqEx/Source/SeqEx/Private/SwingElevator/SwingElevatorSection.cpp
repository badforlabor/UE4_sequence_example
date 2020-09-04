// Copy-right is reserved by iqiyi VR-group


#include "SwingElevator/SwingElevatorSection.h"
#include "SwingElevator/SwingElevatorEvalTemplate.h"
#include "UObject/StructOnScope.h"
#include "UObject/SequencerObjectVersion.h"
#include "Channels/MovieSceneChannelProxy.h"
#include "Styling/SlateColor.h"
#include "Evaluation/MovieScenePropertyTemplate.h"

#if WITH_EDITOR
struct FSwingColorSectionEditorData
{
	FSwingColorSectionEditorData()
	{
		MetaData[0].SetIdentifiers("RotZ", FText::FromString(TEXT("RotZ")));
		MetaData[0].SortOrder = 0;
		MetaData[0].Color = FCommonChannelData::RedChannelColor;
		MetaData[0].bCanCollapseToTrack = false;

		MetaData[1].SetIdentifiers("RotX", FText::FromString(TEXT("RotX")));
		MetaData[1].SortOrder = 1;
		MetaData[1].Color = FCommonChannelData::GreenChannelColor;
		MetaData[1].bCanCollapseToTrack = false;

		MetaData[2].SetIdentifiers("Height", FText::FromString(TEXT("Height")));
		MetaData[2].SortOrder = 2;
		MetaData[2].Color = FCommonChannelData::BlueChannelColor;
		MetaData[2].bCanCollapseToTrack = false;

		MetaData2.SetIdentifiers("IntV", FText::FromString(TEXT("IntV")));
		MetaData2.SortOrder = 3;
		MetaData2.bCanCollapseToTrack = false;

		//ExternalValues[0].OnGetExternalValue = ExtractChannelR;
		//ExternalValues[1].OnGetExternalValue = ExtractChannelG;
		//ExternalValues[2].OnGetExternalValue = ExtractChannelB;
		//ExternalValues[0].OnGetCurrentValueAndWeight = GetChannelRValueAndWeight;
		//ExternalValues[1].OnGetCurrentValueAndWeight = GetChannelGValueAndWeight;
		//ExternalValues[2].OnGetCurrentValueAndWeight = GetChannelBValueAndWeight;

	}

	static FLinearColor GetPropertyValue(UObject& InObject, FTrackInstancePropertyBindings& Bindings)
	{
		const FName SlateColorName("SlateColor");

		UStructProperty* ColorStructProperty = Cast<UStructProperty>(Bindings.GetProperty(InObject));
		if (ColorStructProperty != nullptr)
		{
			if (ColorStructProperty->Struct->GetFName() == SlateColorName)
			{
				return Bindings.GetCurrentValue<FSlateColor>(InObject).GetSpecifiedColor();
			}

			if (ColorStructProperty->Struct->GetFName() == NAME_LinearColor)
			{
				return Bindings.GetCurrentValue<FLinearColor>(InObject);
			}

			if (ColorStructProperty->Struct->GetFName() == NAME_Color)
			{
				return Bindings.GetCurrentValue<FColor>(InObject);
			}
		}
		return FLinearColor(0.f, 0.f, 0.f, 0.f);
	}

	static TOptional<float> ExtractChannelR(UObject& InObject, FTrackInstancePropertyBindings* Bindings)
	{
		return Bindings ? GetPropertyValue(InObject, *Bindings).R : TOptional<float>();
	}
	static TOptional<float> ExtractChannelG(UObject& InObject, FTrackInstancePropertyBindings* Bindings)
	{
		return Bindings ? GetPropertyValue(InObject, *Bindings).G : TOptional<float>();
	}
	static TOptional<float> ExtractChannelB(UObject& InObject, FTrackInstancePropertyBindings* Bindings)
	{
		return Bindings ? GetPropertyValue(InObject, *Bindings).B : TOptional<float>();
	}
	static TOptional<float> ExtractChannelA(UObject& InObject, FTrackInstancePropertyBindings* Bindings)
	{
		return Bindings ? GetPropertyValue(InObject, *Bindings).A : TOptional<float>();
	}

	static void GetChannelRValueAndWeight(UObject* Object, UMovieSceneSection*  SectionToKey, FFrameNumber KeyTime, FFrameRate TickResolution, FMovieSceneRootEvaluationTemplateInstance& RootTemplate,
		float& OutValue, float OutWeight)
	{
		GetChannelValueAndWeight(0, Object, SectionToKey, KeyTime, TickResolution, RootTemplate, OutValue, OutWeight);
	}
	static void GetChannelGValueAndWeight(UObject* Object, UMovieSceneSection*  SectionToKey, FFrameNumber KeyTime, FFrameRate TickResolution, FMovieSceneRootEvaluationTemplateInstance& RootTemplate,
		float& OutValue, float OutWeight)
	{
		GetChannelValueAndWeight(1, Object, SectionToKey, KeyTime, TickResolution, RootTemplate, OutValue, OutWeight);
	}
	static void GetChannelBValueAndWeight(UObject* Object, UMovieSceneSection*  SectionToKey, FFrameNumber KeyTime, FFrameRate TickResolution, FMovieSceneRootEvaluationTemplateInstance& RootTemplate,
		float& OutValue, float OutWeight)
	{
		GetChannelValueAndWeight(2, Object, SectionToKey, KeyTime, TickResolution, RootTemplate, OutValue, OutWeight);
	}
	static void GetChannelAValueAndWeight(UObject* Object, UMovieSceneSection*  SectionToKey, FFrameNumber KeyTime, FFrameRate TickResolution, FMovieSceneRootEvaluationTemplateInstance& RootTemplate,
		float& OutValue, float OutWeight)
	{
		GetChannelValueAndWeight(3, Object, SectionToKey, KeyTime, TickResolution, RootTemplate, OutValue, OutWeight);
	}

	static void GetChannelValueAndWeight(int32 Index, UObject* Object, UMovieSceneSection*  SectionToKey, FFrameNumber KeyTime, FFrameRate TickResolution, FMovieSceneRootEvaluationTemplateInstance& RootTemplate,
		float& OutValue, float& OutWeight)
	{
		OutValue = 0.0f;
		OutWeight = 1.0f;

		UMovieSceneTrack* Track = SectionToKey->GetTypedOuter<UMovieSceneTrack>();

		if (Track)
		{
			FMovieSceneEvaluationTrack EvalTrack = Track->GenerateTrackTemplate();
			FMovieSceneInterrogationData InterrogationData;
			RootTemplate.CopyActuators(InterrogationData.GetAccumulator());

			FMovieSceneContext Context(FMovieSceneEvaluationRange(KeyTime, TickResolution));
			EvalTrack.Interrogate(Context, InterrogationData, Object);

			FLinearColor Val(0.0f, 0.0f, 0.0f, 0.0f);
			for (const FLinearColor& InColor : InterrogationData.Iterate<FLinearColor>(FMovieScenePropertySectionTemplate::GetColorInterrogationKey()))
			{
				Val = InColor;
				break;
			}
			switch (Index)
			{
			case 0:
				OutValue = Val.R;
				break;
			case 1:
				OutValue = Val.G;
				break;
			case 2:
				OutValue = Val.B;
				break;
			case 3:
				OutValue = Val.A;
				break;
			}

		}
		OutWeight = MovieSceneHelpers::CalculateWeightForBlending(SectionToKey, KeyTime);
	}

	FMovieSceneChannelMetaData      MetaData[4];
	TMovieSceneExternalValue<float> ExternalValues[4];
	FMovieSceneChannelMetaData		MetaData2;
	TMovieSceneExternalValue<int> ExternalValues2;
};
#endif

void FSwingMovieSceneColorKeyStruct::PropagateChanges(const FPropertyChangedEvent& ChangeEvent)
{
	KeyStructInterop.Apply(Time);
	
}


USwingElevatorSection::USwingElevatorSection()
{
	EvalOptions.EnableAndSetCompletionMode
	(GetLinkerCustomVersion(FSequencerObjectVersion::GUID) < FSequencerObjectVersion::WhenFinishedDefaultsToRestoreState ?
		EMovieSceneCompletionMode::KeepState :
		GetLinkerCustomVersion(FSequencerObjectVersion::GUID) < FSequencerObjectVersion::WhenFinishedDefaultsToProjectDefault ?
		EMovieSceneCompletionMode::RestoreState :
		EMovieSceneCompletionMode::ProjectDefault);
	BlendType = EMovieSceneBlendType::Absolute;
	bSupportsInfiniteRange = true;

	FMovieSceneChannelProxyData Channels;

#if WITH_EDITOR

	static FSwingColorSectionEditorData EditorData;
	Channels.Add(RedCurve, EditorData.MetaData[0], EditorData.ExternalValues[0]);
	Channels.Add(GreenCurve, EditorData.MetaData[1], EditorData.ExternalValues[1]);
	Channels.Add(BlueCurve, EditorData.MetaData[2], EditorData.ExternalValues[2]);
	//Channels.Add(DeviceIdCurve, EditorData.MetaData2, EditorData.ExternalValues2);

#else

	Channels.Add(RedCurve);
	Channels.Add(GreenCurve);
	Channels.Add(BlueCurve);
	//Channels.Add(DeviceIdCurve);

#endif

	ChannelProxy = MakeShared<FMovieSceneChannelProxy>(MoveTemp(Channels));

}
FMovieSceneEvalTemplatePtr USwingElevatorSection::GenerateTemplate() const
{
	return FSwingElevatorEvalTemplate(this, RedCurve, GreenCurve, BlueCurve, DeviceIdCurve);
}

TSharedPtr<FStructOnScope> USwingElevatorSection::GetKeyStruct(TArrayView<const FKeyHandle> KeyHandles)
{
	TSharedRef<FStructOnScope> KeyStruct = MakeShareable(new FStructOnScope(FSwingMovieSceneColorKeyStruct::StaticStruct()));
	auto Struct = (FSwingMovieSceneColorKeyStruct*)KeyStruct->GetStructMemory();

	Struct->KeyStructInterop.Add(FMovieSceneChannelValueHelper(ChannelProxy->MakeHandle<FMovieSceneFloatChannel>(0), &Struct->Color.R, KeyHandles));
	Struct->KeyStructInterop.Add(FMovieSceneChannelValueHelper(ChannelProxy->MakeHandle<FMovieSceneFloatChannel>(1), &Struct->Color.G, KeyHandles));
	Struct->KeyStructInterop.Add(FMovieSceneChannelValueHelper(ChannelProxy->MakeHandle<FMovieSceneFloatChannel>(2), &Struct->Color.B, KeyHandles));

	//Struct->KeyStructInterop.Add(FMovieSceneChannelValueHelper(ChannelProxy->MakeHandle<FMovieSceneIntegerChannel>(3), &Struct->IntValue, KeyHandles));

	Struct->KeyStructInterop.SetStartingValues();
	Struct->Time = Struct->KeyStructInterop.GetUnifiedKeyTime().Get(0);

	return KeyStruct;
}


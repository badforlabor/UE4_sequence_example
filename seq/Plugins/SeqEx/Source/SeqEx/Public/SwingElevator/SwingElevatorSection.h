// Copy-right is reserved by iqiyi VR-group

#pragma once

#include "CoreMinimal.h"
#include "Curves/KeyHandle.h"
#include "Curves/RichCurve.h"
#include "MovieSceneSection.h"
#include "MovieSceneKeyStruct.h"
#include "Channels/MovieSceneIntegerChannel.h"
#include "Channels/MovieSceneFloatChannel.h"
#include "SwingElevatorSection.generated.h"

USTRUCT()
struct FSwingMovieSceneColorKeyStruct
	: public FMovieSceneKeyStruct
{
	GENERATED_BODY()

		/** The key's color value. */
	UPROPERTY(EditAnywhere, Category = Key, meta = (InlineColorPicker))
		FLinearColor Color;

	UPROPERTY(EditAnywhere, Category = Key)
		int IntValue;

	/** The key's time. */
	UPROPERTY(EditAnywhere, Category = Key)
		FFrameNumber Time;

	FMovieSceneKeyStructHelper KeyStructInterop;

	virtual void PropagateChanges(const FPropertyChangedEvent& ChangeEvent) override;
};
template<> struct TStructOpsTypeTraits<FSwingMovieSceneColorKeyStruct> : public TStructOpsTypeTraitsBase2<FSwingMovieSceneColorKeyStruct> { enum { WithCopy = false }; };

/**
 * 
 */
UCLASS()
class SEQEX_API USwingElevatorSection : public UMovieSceneSection
{
	GENERATED_BODY()

public:
	USwingElevatorSection();
	/* UMovieSceneSection interface */
	virtual FMovieSceneEvalTemplatePtr GenerateTemplate() const override;

public:
	FString GetTitle()		const { return TEXT("Elevator"); }
protected:

	//~ UMovieSceneSection interface
	virtual TSharedPtr<FStructOnScope> GetKeyStruct(TArrayView<const FKeyHandle> KeyHandles) override;

public:
	UPROPERTY(EditAnywhere)
		int32 DeviceId;

	/** Red curve data */
	UPROPERTY()
		FMovieSceneIntegerChannel DeviceIdCurve;

	/** Red curve data */
	UPROPERTY()
		FMovieSceneFloatChannel RedCurve;

	/** Green curve data */
	UPROPERTY()
		FMovieSceneFloatChannel GreenCurve;

	/** Blue curve data */
	UPROPERTY()
		FMovieSceneFloatChannel BlueCurve;
};

// Copy-right is reserved by iqiyi VR-group

#pragma once

#include "CoreMinimal.h"
#include "MovieSceneSection.h"
#include "MovieSceneObjectBindingId.h"
#include "PlayAudioSection.generated.h"

/**
 * 
 */
UCLASS()
class SEQEX_API UPlayAudioSection : public UMovieSceneSection
{
	GENERATED_BODY()

public:
	/* UMovieSceneSection interface */
	virtual FMovieSceneEvalTemplatePtr GenerateTemplate() const override;

public:
	FString GetTitle()		const { return TEXT(""); }	

	UPROPERTY()
		FMovieSceneObjectBindingID BindingId;

	UPROPERTY(EditAnywhere)
		class USoundBase* Sound;
	UPROPERTY(EditAnywhere)
		bool notDestory;
};

// Copy-right is reserved by iqiyi VR-group

#pragma once

#include "CoreMinimal.h"
#include "MovieSceneTrack.h"
#include "PlayAudioTrack.generated.h"

/**
 * 
 */
UCLASS()
class SEQEX_API UPlayAudioTrack : public UMovieSceneTrack
{
	GENERATED_BODY()

public:
	UPlayAudioTrack(const FObjectInitializer& InInitializer)
		: Super(InInitializer)
	{
#if WITH_EDITORONLY_DATA
		SetColorTint(FColor(0, 156, 255, 65));
#endif
	}

	// 继承实现得API
	virtual UMovieSceneSection* CreateNewSection() override;
	virtual bool SupportsMultipleRows() const override { return true; }
	virtual FName GetTrackName() const override;
	virtual void RemoveAllAnimationData() override { Sections.Empty(); }
	virtual bool HasSection(const UMovieSceneSection& Section) const override { return Sections.Contains(&Section); }
	virtual void AddSection(UMovieSceneSection& Section) override { Sections.Add(&Section); }
	virtual void RemoveSection(UMovieSceneSection& Section) override { Sections.Remove(&Section); }
	virtual bool IsEmpty() const override { return Sections.Num() == 0; }
	virtual const TArray<UMovieSceneSection*>& GetAllSections() const override { return Sections; }

#if WITH_EDITORONLY_DATA
	virtual FText GetDisplayName() const override;
#endif
	
protected:
	virtual FMovieSceneEvalTemplatePtr CreateTemplateForSection(const UMovieSceneSection& InSection) const override;

public:
	class UPlayAudioSection* AddNewEvent(const FFrameNumber& Time);

protected:
	UPROPERTY()
		TArray<UMovieSceneSection*> Sections;
};

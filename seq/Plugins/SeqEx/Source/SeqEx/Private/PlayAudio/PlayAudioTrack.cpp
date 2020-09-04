// Copy-right is reserved by iqiyi VR-group

#include "PlayAudio/PlayAudioTrack.h"
#include "PlayAudio/PlayAudioSection.h"
#include "PlayAudio/PlayAudioEvalTemplate.h"
#include "IMovieScenePlayer.h"
#include "MovieScene.h"

FMovieSceneEvalTemplatePtr UPlayAudioTrack::CreateTemplateForSection(const UMovieSceneSection &InSection) const
{
	return InSection.GenerateTemplate();
}

UMovieSceneSection *UPlayAudioTrack::CreateNewSection()
{
	return NewObject<UMovieSceneSection>(this, UPlayAudioTrack::StaticClass(), NAME_None, RF_Transactional);
}

UPlayAudioSection *UPlayAudioTrack::AddNewEvent(const FFrameNumber &Time)
{
	UPlayAudioSection *NewSection = NewObject<UPlayAudioSection>(this);
	ensure(NewSection);

	NewSection->InitialPlacement(GetAllSections(), Time, 2, SupportsMultipleRows());
	AddSection(*NewSection);

	return NewSection;
}

#if WITH_EDITORONLY_DATA
FText UPlayAudioTrack::GetDisplayName() const
{
	return NSLOCTEXT("PlayAudioTrack", "TrackName", "PlayAudio");
}
#endif

FName UPlayAudioTrack::GetTrackName() const
{
	static FName TrackName("PlayAudio");
	return TrackName;
}
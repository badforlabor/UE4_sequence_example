// Copy-right is reserved by iqiyi VR-group


#include "SwingElevator/SwingElevatorTrack.h"
#include "SwingElevator/SwingElevatorSection.h"
#include "SwingElevator/SwingElevatorEvalTemplate.h"
#include "IMovieScenePlayer.h"
#include "MovieScene.h"


FMovieSceneEvalTemplatePtr USwingElevatorTrack::CreateTemplateForSection(const UMovieSceneSection& InSection) const
{
	return InSection.GenerateTemplate();
}

UMovieSceneSection* USwingElevatorTrack::CreateNewSection()
{
	return NewObject<UMovieSceneSection>(this, USwingElevatorTrack::StaticClass(), NAME_None, RF_Transactional);
}

void USwingElevatorTrack::AddNewEvent(const FFrameNumber& Time)
{
	USwingElevatorSection* NewSection = NewObject<USwingElevatorSection>(this);
	ensure(NewSection);

	NewSection->InitialPlacement(GetAllSections(), Time, 2, SupportsMultipleRows());
	AddSection(*NewSection);
}

#if WITH_EDITORONLY_DATA
FText USwingElevatorTrack::GetDisplayName() const
{
	return NSLOCTEXT("SwingElevatorTrack", "TrackName", "SwingElevators");
}
#endif

FName USwingElevatorTrack::GetTrackName() const
{
	static FName TrackName("SwingElevators");
	return TrackName;
}
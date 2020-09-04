// Copy-right is reserved by iqiyi VR-group

#include "PlayAudio/PlayAudioSection.h"
#include "PlayAudioEvalTemplate.h"

FMovieSceneEvalTemplatePtr UPlayAudioSection::GenerateTemplate() const
{
	return FPlayAudioEvalTemplate(this);
}

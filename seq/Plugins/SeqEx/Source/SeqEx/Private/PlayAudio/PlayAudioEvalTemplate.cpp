// Copy-right is reserved by iqiyi VR-group


#include "PlayAudioEvalTemplate.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "Components/AudioComponent.h"
#include "SeqExPrivate.h"

FPlayAudioEvalTemplate::FPlayAudioEvalTemplate()
{
	Section = NULL;
}

FPlayAudioEvalTemplate::FPlayAudioEvalTemplate(const UPlayAudioSection* InSection)
	: Section(InSection)
{}

void FPlayAudioEvalTemplate::Evaluate(const FMovieSceneEvaluationOperand& Operand, const FMovieSceneContext& Context,
	const FPersistentEvaluationData& PersistentData, FMovieSceneExecutionTokens& ExecutionTokens) const
{
	SeqExLog_Debug(FString::Printf(TEXT("FPlayAudioEvalTemplate::Evaluate")));

	if (!Section)
	{
		return;
	}
	if (!GWorld || !GWorld->IsGameWorld())
	{
		return;
	}
	if ((Context.GetStatus() != EMovieScenePlayerStatus::Playing && Context.GetStatus() != EMovieScenePlayerStatus::Scrubbing) || Context.HasJumped() || Context.GetDirection() == EPlayDirection::Backwards)
	{
		// stopped, recording, etc
		if (Section->AudioComp)
		{
			Section->AudioComp->Stop();
		}
		const_cast<UPlayAudioSection*>(Section)->playing = false;
		return;
	}

	if (Section->AudioComp)
	{
		Section->AudioComp->SetWorldLocation(Section->AudioComp->GetOwner()->GetActorLocation());
	}
	if (!Section->playing && Section->AudioComp)
	{
		Section->AudioComp->Sound = Section->Sound;
		//Section->AudioComp->Play();

		const float ImageSequenceTime =
			FFrameTime::FromDecimal((Context.GetTime() - FFrameTime(Section->GetInclusiveStartFrame())).AsDecimal()) / Context.GetFrameRate();
		Section->AudioComp->Play(ImageSequenceTime);
		const_cast<UPlayAudioSection*>(Section)->playing = true;
	}
}

//FString ToString(const FMovieSceneObjectBindingID& ID, IMovieScenePlayer& Player)
//{
//	for (TWeakObjectPtr<> WeakEventContext : Player.FindBoundObjects(ID.GetGuid(), ID.GetSequenceID()))
//	{
//		if (UObject* EventContext = WeakEventContext.Get())
//		{
//			return EventContext->GetFullName();
//		}
//	}
//	return TEXT("None");
//}

void FPlayAudioEvalTemplate::Setup(FPersistentEvaluationData& PersistentData, IMovieScenePlayer& Player) const 
{
	if (!Section)
	{
		return;
	}
	if (!GWorld || !GWorld->IsGameWorld())
	{
		return;
	}
	// 触发
	SeqExLog_Debug(FString::Printf(TEXT("FPlayAudioEvalTemplate::Setup:%s. Sound=%s"), *ToString(Section->BindingId, Player), *Section->Sound->GetFullName()));
	//SetBindingActorVisible(Section->BindingId, Player, false);
	AActor* Actor = NULL;

	auto ID = Section->BindingId;
	for (TWeakObjectPtr<> WeakEventContext : Player.FindBoundObjects(ID.GetGuid(), ID.GetSequenceID()))
	{
		if (UObject* EventContext = WeakEventContext.Get())
		{
			Actor = Cast<AActor>(EventContext);
			if (Actor != NULL)
			{
				break;
			}
		}
	}

	if (Actor)
	{
		UAudioComponent* AudioComp = NewObject<UAudioComponent>(Actor, UAudioComponent::StaticClass());
		const_cast<UPlayAudioSection*>(Section)->playing = false;
		const_cast<UPlayAudioSection*>(Section)->AudioComp = AudioComp;
		AudioComp->OnComponentCreated();
		AudioComp->RegisterComponent();
		//auto AudioComp = Cast<UAudioComponent>(Actor->CreateComponentFromTemplate(UAudioComponent::StaticClass()));
		if (false && AudioComp)
		{
			AudioComp->Sound = Section->Sound;
			AudioComp->Play();
		}
	}
}

void FPlayAudioEvalTemplate::TearDown(FPersistentEvaluationData& PersistentData, IMovieScenePlayer& Player) const
{
	if (!Section)
	{
		return;
	}

	if (!GWorld || !GWorld->IsGameWorld())
	{
		return;
	}

	if (GWorld->IsGameWorld() && (Player.AsUObject() == NULL || Player.AsUObject()->GetWorld() == NULL || !Player.AsUObject()->GetWorld()->IsValidLowLevel()))
	{
		return;
	}
	
	if (Section->AudioComp && !Section->notDestory)
	{
		Section->AudioComp->Stop();
		Section->AudioComp->Sound = nullptr;
	}

	SeqExLog_Debug(FString::Printf(TEXT("FPlayAudioEvalTemplate::TearDown")));
}

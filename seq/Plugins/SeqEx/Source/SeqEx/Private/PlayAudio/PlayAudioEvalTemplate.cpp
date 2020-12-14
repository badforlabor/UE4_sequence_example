// Copy-right is reserved by iqiyi VR-group


#include "PlayAudioEvalTemplate.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "Components/AudioComponent.h"
#include "SeqExPrivate.h"

struct FPlayAudioRuntimeData : IPersistentEvaluationData
{
	class UAudioComponent* AudioComp = nullptr;
	bool playing = false;
	EPlayDirection PlayDir = EPlayDirection::Forwards;
};

struct FPlayAudioTokenToken : IMovieSceneExecutionToken
{
	const UPlayAudioSection* Section;

	FPlayAudioTokenToken(const UPlayAudioSection* InSection)
		: Section(InSection)
	{}

	virtual void Execute(const FMovieSceneContext& Context, const FMovieSceneEvaluationOperand& Operand, FPersistentEvaluationData& PersistentData, IMovieScenePlayer& Player) override
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

		auto& RuntimeData = PersistentData.GetOrAddTrackData<FPlayAudioRuntimeData>();

		if ((Context.GetStatus() != EMovieScenePlayerStatus::Playing && Context.GetStatus() != EMovieScenePlayerStatus::Scrubbing) || Context.HasJumped())
		{
			// stopped, recording, etc
			if (RuntimeData.AudioComp)
			{
				RuntimeData.AudioComp->Stop();
			}
			RuntimeData.playing = false;
			SeqExLog_Debug(Section, FString::Printf(TEXT("播放音频, 停止播放")));
			return;
		}

		if (RuntimeData.PlayDir != Context.GetDirection())
		{
			// 重新计算时间
			RuntimeData.PlayDir = Context.GetDirection();
			if (RuntimeData.AudioComp)
			{
				RuntimeData.AudioComp->Stop();
			}
			RuntimeData.playing = false;
			SeqExLog_Debug(Section, FString::Printf(TEXT("播放音频, 更换方向:%d. this=%p, Section=%p"), RuntimeData.PlayDir, this, Section));
		}

		if (RuntimeData.AudioComp)
		{
			RuntimeData.AudioComp->SetWorldLocation(RuntimeData.AudioComp->GetOwner()->GetActorLocation());
		}
		if (!RuntimeData.playing && RuntimeData.AudioComp)
		{
			RuntimeData.AudioComp->Sound = Section->Sound;
			//Section->AudioComp->Play();

			auto T1 = (Context.GetTime() - FFrameTime(Section->GetInclusiveStartFrame())).AsDecimal();
			if (Context.GetDirection() == EPlayDirection::Backwards)
			{
				auto T2 = (FFrameTime(Section->GetExclusiveEndFrame()) - Context.GetTime()).AsDecimal();
				T1 = FMath::Min<double>(T1, FMath::Abs<double>(T2));
			}

			const float ImageSequenceTime =
				FFrameTime::FromDecimal(T1) / Context.GetFrameRate();

			RuntimeData.AudioComp->Play(ImageSequenceTime);
			RuntimeData.playing = true;
			SeqExLog_Debug(Section, FString::Printf(TEXT("播放音频：%.2f. Start=%.2f, End=%.2f"), ImageSequenceTime,
				Context.GetFrameRate().AsSeconds(FFrameTime(Section->GetInclusiveStartFrame())),
				Context.GetFrameRate().AsSeconds(FFrameTime(Section->GetExclusiveEndFrame()))));
		}
	}
};

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

	ExecutionTokens.Add(FPlayAudioTokenToken(Section));
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

	auto& RuntimeData = PersistentData.GetOrAddTrackData<FPlayAudioRuntimeData>();

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
		RuntimeData.playing = false;
		RuntimeData.AudioComp = AudioComp;
		AudioComp->OnComponentCreated();
		AudioComp->RegisterComponent();
		//auto AudioComp = Cast<UAudioComponent>(Actor->CreateComponentFromTemplate(UAudioComponent::StaticClass()));
		if (false && AudioComp)
		{
			AudioComp->Sound = Section->Sound;
			AudioComp->Play();
		}
	}
	SeqExLog_Debug(Section, FString::Printf(TEXT("播放音频, Setup. this=%p, Section=%p, Actor=%p, Player=%p"), 
		this, Section, Actor, Player.AsUObject()));
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
	
	auto RuntimeDataPtr = PersistentData.FindTrackData<FPlayAudioRuntimeData>();
	if (RuntimeDataPtr != nullptr)
	{
		auto& RuntimeData = *RuntimeDataPtr;
		if (RuntimeData.AudioComp && !Section->notDestory)
		{
			RuntimeData.AudioComp->Stop();
			RuntimeData.AudioComp->Sound = nullptr;
		}
	}

	SeqExLog_Debug(Section, FString::Printf(TEXT("播放音频, TearDown")));
}

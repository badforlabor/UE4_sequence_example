// Copy-right is reserved by iqiyi VR-group

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Evaluation/MovieSceneEvalTemplate.h"
#include "PlayAudio/PlayAudioSection.h"
#include "PlayAudioEvalTemplate.generated.h"


/**
 * 
 */
USTRUCT()
struct FPlayAudioEvalTemplate : public FMovieSceneEvalTemplate
{
	GENERATED_BODY()
public:
	FPlayAudioEvalTemplate();
	FPlayAudioEvalTemplate(const UPlayAudioSection* InSection);

	// 运行时播放得计算处理
	virtual void Evaluate(const FMovieSceneEvaluationOperand& Operand, const FMovieSceneContext& Context, const FPersistentEvaluationData& PersistentData, FMovieSceneExecutionTokens& ExecutionTokens) const override;
	virtual UScriptStruct& GetScriptStructImpl() const override { return *StaticStruct(); }
	virtual void Setup(FPersistentEvaluationData& PersistentData, IMovieScenePlayer& Player) const override;
	virtual void TearDown(FPersistentEvaluationData& PersistentData, IMovieScenePlayer& Player) const override;
	virtual void SetupOverrides() override { EnableOverrides(RequiresSetupFlag | RequiresTearDownFlag); }

	UPROPERTY()
		const UPlayAudioSection* Section = nullptr;
};

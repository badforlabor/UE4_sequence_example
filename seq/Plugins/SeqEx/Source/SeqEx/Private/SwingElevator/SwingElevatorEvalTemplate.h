// Copy-right is reserved by iqiyi VR-group

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Evaluation/MovieSceneEvalTemplate.h"
#include "SwingElevator/SwingElevatorSection.h"
#include "SwingElevatorEvalTemplate.generated.h"


/**
 * 
 */
USTRUCT()
struct FSwingElevatorEvalTemplate : public FMovieSceneEvalTemplate
{
	GENERATED_BODY()
public:
	FSwingElevatorEvalTemplate();
	FSwingElevatorEvalTemplate(const USwingElevatorSection* InSection, const FMovieSceneFloatChannel& InRedChannel, const FMovieSceneFloatChannel& InGreenChannel, const FMovieSceneFloatChannel& InBlueChannel, const FMovieSceneIntegerChannel& InDeviceIdCurve);

	// 运行时播放得计算处理
	virtual void Evaluate(const FMovieSceneEvaluationOperand& Operand, const FMovieSceneContext& Context, const FPersistentEvaluationData& PersistentData, FMovieSceneExecutionTokens& ExecutionTokens) const override;
	virtual UScriptStruct& GetScriptStructImpl() const override { return *StaticStruct(); }
	virtual void Setup(FPersistentEvaluationData& PersistentData, IMovieScenePlayer& Player) const override;
	virtual void TearDown(FPersistentEvaluationData& PersistentData, IMovieScenePlayer& Player) const override;
	virtual void SetupOverrides() override { EnableOverrides(RequiresSetupFlag | RequiresTearDownFlag); }

	UPROPERTY()
		const USwingElevatorSection* Section = nullptr;
	UPROPERTY(Transient)
		const UObject* PlayerObj = nullptr;

private:
	UPROPERTY()
		FMovieSceneFloatChannel RedChannel;

	UPROPERTY()
		FMovieSceneFloatChannel GreenChannel;

	UPROPERTY()
		FMovieSceneFloatChannel BlueChannel;

	UPROPERTY()
		FMovieSceneIntegerChannel DeviceIdCurve;
};

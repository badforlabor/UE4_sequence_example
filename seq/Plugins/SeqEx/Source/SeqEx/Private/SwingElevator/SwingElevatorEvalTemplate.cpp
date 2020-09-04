// Copy-right is reserved by iqiyi VR-group


#include "SwingElevatorEvalTemplate.h"
#include "SeqExPrivate.h"

FSwingElevatorEvalTemplate::FSwingElevatorEvalTemplate()
{
	Section = NULL;
}

FSwingElevatorEvalTemplate::FSwingElevatorEvalTemplate(const USwingElevatorSection* InSection, 
	const FMovieSceneFloatChannel& InRedChannel, const FMovieSceneFloatChannel& InGreenChannel, 
	const FMovieSceneFloatChannel& InBlueChannel, const FMovieSceneIntegerChannel& InDeviceIdCurve)
	: Section(InSection)
	, RedChannel(InRedChannel)
	, GreenChannel(InGreenChannel)
	, BlueChannel(InBlueChannel)
	, DeviceIdCurve(InDeviceIdCurve)
{}

void FSwingElevatorEvalTemplate::Evaluate(const FMovieSceneEvaluationOperand& Operand, const FMovieSceneContext& Context,
	const FPersistentEvaluationData& PersistentData, FMovieSceneExecutionTokens& ExecutionTokens) const
{
	float R = 0, G = 0, B = 0;
	auto InTime = Context.GetTime();
	RedChannel.Evaluate(InTime, R);
	GreenChannel.Evaluate(InTime, G);
	BlueChannel.Evaluate(InTime, B);

	SeqExLog_Debug(nullptr, FString::Printf(TEXT("FSwingElevatorEvalTemplate::Evaluate: %.2f, %.2f, %.2f"), R, G, B));

	if (!Section || !PlayerObj)
	{
		return;
	}
	//if (!UIvrHelper::IsServerSide2(PlayerObj))
	//{
	//	return;
	//}

	//UHardwareHelper::SwingElevator(PlayerObj, Section->DeviceId, R, G, B);
}

void FSwingElevatorEvalTemplate::Setup(FPersistentEvaluationData& PersistentData, IMovieScenePlayer& Player) const 
{
	if (!Section)
	{
		return;
	}
	
	if (Player.AsUObject() == NULL || Player.AsUObject()->GetWorld() == NULL)
	{
		return;
	}

	// 触发
	SeqExLog_Debug(FString::Printf(TEXT("FSwingElevatorEvalTemplate::Setup。 %d"), Section->DeviceId));

	const_cast<FSwingElevatorEvalTemplate*>(this)->PlayerObj = Player.AsUObject();
}

void FSwingElevatorEvalTemplate::TearDown(FPersistentEvaluationData& PersistentData, IMovieScenePlayer& Player) const
{
	if (!Section)
	{
		return;
	}

	if (Player.AsUObject() == NULL || Player.AsUObject()->GetWorld() == NULL)
	{
		return;
	}

	const_cast<FSwingElevatorEvalTemplate*>(this)->PlayerObj = nullptr;

	// 结束
	SeqExLog_Debug(FString::Printf(TEXT("FSwingElevatorEvalTemplate::TearDown")));
}

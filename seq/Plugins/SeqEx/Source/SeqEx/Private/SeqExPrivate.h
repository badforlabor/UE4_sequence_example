
#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "UObject/Object.h"
#include "UObject/Class.h"
#include "Templates/SubclassOf.h"
#include "Engine.h"
#include "Engine/EngineTypes.h"

#include "Evaluation/MovieSceneEvalTemplate.h"

extern void SeqExLog_Debug(const FString& msg);
extern void SeqExLog_Debug(const UObject* WorldContextObject, const FString& msg);

extern FString ToString(const FMovieSceneObjectBindingID& ID, IMovieScenePlayer& Player);
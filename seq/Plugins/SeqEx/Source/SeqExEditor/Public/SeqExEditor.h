﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Modules/ModuleInterface.h"
#include "Modules/ModuleManager.h"
#include "UnrealEd.h"

DECLARE_LOG_CATEGORY_EXTERN(SeqExEditor, All, All)

class FSeqExEditorModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:

	//FDelegateHandle PlayAudioTrackEditorHandle;
	//FDelegateHandle SwingElevatorTrackEditorHandle;
	TArray< FDelegateHandle> SeqHandles;
};

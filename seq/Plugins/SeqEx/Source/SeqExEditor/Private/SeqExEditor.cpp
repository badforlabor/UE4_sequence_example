#include "SeqExEditor.h"
#include "Modules/ModuleManager.h"
#include "Modules/ModuleInterface.h"
#include "ISequencerModule.h"
#include "SwingElevator/SwingElevatorTrackEditor.h"
#include "PlayAudio/PlayAudioTrackEditor.h"

IMPLEMENT_GAME_MODULE(FSeqExEditorModule, SeqExEditor);

DEFINE_LOG_CATEGORY(SeqExEditor)

#define LOCTEXT_NAMESPACE "SeqExEditor"

void FSeqExEditorModule::StartupModule()
{
	UE_LOG(SeqExEditor, Warning, TEXT("SeqExEditor: Log Started"));

	ISequencerModule& SequencerModule = FModuleManager::LoadModuleChecked<ISequencerModule>(TEXT("Sequencer"));
	//auto PlayAudioTrackEditorHandle = SequencerModule.RegisterTrackEditor(FOnCreateTrackEditor::CreateStatic(&FPlayAudioTrackEditor::CreateTrackEditor));
	//auto SwingElevatorTrackEditorHandle = SequencerModule.RegisterTrackEditor(FOnCreateTrackEditor::CreateStatic(&FSwingElevatorTrackEditor::CreateTrackEditor));

	SeqHandles.Add( SequencerModule.RegisterTrackEditor(FOnCreateTrackEditor::CreateStatic(&FPlayAudioTrackEditor::CreateTrackEditor)) );
	SeqHandles.Add( SequencerModule.RegisterTrackEditor(FOnCreateTrackEditor::CreateStatic(&FSwingElevatorTrackEditor::CreateTrackEditor)) );
}

void FSeqExEditorModule::ShutdownModule()
{
	UE_LOG(SeqExEditor, Warning, TEXT("SeqExEditor: Log Ended"));

	if (FModuleManager::Get().IsModuleLoaded(TEXT("Sequencer")))
	{
		ISequencerModule& SequencerModule = FModuleManager::GetModuleChecked<ISequencerModule>(TEXT("Sequencer"));

		//SequencerModule.UnRegisterTrackEditor(PlayAudioTrackEditorHandle);
		//SequencerModule.UnRegisterTrackEditor(SwingElevatorTrackEditorHandle);
		for (auto Handle : SeqHandles)
		{
			SequencerModule.UnRegisterTrackEditor(Handle);
		}
	}
}

#undef LOCTEXT_NAMESPACE
#include "PlayAudioTrackEditor.h"
#include "MovieScene.h"
#include "MovieSceneCommonHelpers.h"
#include "RenderUtils.h"
#include "Textures/SlateTextureData.h"
#include "Slate/SlateTextures.h"
#include "Rendering/DrawElements.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "Framework/Application/SlateApplication.h"
#include "SequencerUtilities.h"
#include "ISequencerSection.h"
#include "ScopedTransaction.h"
#include "SequencerSectionPainter.h"
#include "IContentBrowserSingleton.h"
#include "IAssetRegistry.h"
#include "AssetRegistryModule.h"
#include "ContentBrowserModule.h"
#include "Async/Async.h"
#include "PlayAudio/PlayAudioSection.h"
#include "GameFramework/Actor.h"

#define LOCTEXT_NAMESPACE "PlayAudioTrackEditor"

class FPlayAudioSection
	: public ISequencerSection
{
public:
	FPlayAudioSection(UMovieSceneSection& InSection, TWeakPtr<ISequencer> InSequencer)
		: Section(Cast<UPlayAudioSection>(&InSection))
        , Sequencer(InSequencer)
	{
    }
    ~FPlayAudioSection()
    {
    }
	virtual UMovieSceneSection* GetSectionObject() override { return Section; }
	virtual FText GetSectionTitle() const override
	{
        if (Section != nullptr)
        {
			return FText::FromString(Section->GetTitle());
        }
		return FText::GetEmpty();
	}
    virtual void BuildSectionContextMenu(FMenuBuilder& MenuBuilder, const FGuid& ObjectBinding) override 
    {
	}
	virtual int32 OnPaintSection(FSequencerSectionPainter& InPainter) const override
	{
		int32 LayerId = InPainter.PaintSectionBackground();
		return LayerId;
	}

private:
	UPlayAudioSection* Section;
	TWeakPtr<ISequencer> Sequencer;
};


FPlayAudioTrackEditor::FPlayAudioTrackEditor(TSharedRef<ISequencer> InSequencer)
	: FMovieSceneTrackEditor(InSequencer)
{
}

TSharedRef<ISequencerTrackEditor> FPlayAudioTrackEditor::CreateTrackEditor(TSharedRef<ISequencer> InSequencer)
{
	return MakeShareable(new FPlayAudioTrackEditor(InSequencer));
}

bool FPlayAudioTrackEditor::SupportsType(TSubclassOf<UMovieSceneTrack> Type) const
{
    return Type == UPlayAudioTrack::StaticClass();
}

void FPlayAudioTrackEditor::BuildTrackContextMenu(FMenuBuilder& MenuBuilder, UMovieSceneTrack* Track)
{
}

TSharedRef<ISequencerSection> FPlayAudioTrackEditor::MakeSectionInterface(UMovieSceneSection& SectionObject, UMovieSceneTrack& Track, FGuid ObjectBinding)
{
	return MakeShareable(new FPlayAudioSection(SectionObject, GetSequencer()));
}

const FSlateBrush* FPlayAudioTrackEditor::GetIconBrush() const
{
	return nullptr;
}

void FPlayAudioTrackEditor::BuildAddTrackMenu(FMenuBuilder& MenuBuilder)
{
	//MenuBuilder.AddMenuEntry(
	//	LOCTEXT("AddPlayAudioTrack", "PlayAudio"),
	//	LOCTEXT("AddPlayAudioTrackTooltip", "Adds PlayAudio track."),
	//	FSlateIcon("PlayAudio", "EventIcon"),
	//	FUIAction(FExecuteAction::CreateLambda([=]
	//	{
	//		auto FocusedMovieScene = GetFocusedMovieScene();
	//		if (FocusedMovieScene == nullptr)
	//		{
	//			return;
	//		}
	//		const FScopedTransaction Transaction(LOCTEXT("AddPlayAudioTrack_Transaction", "Add PlayAudio Track"));
	//		FocusedMovieScene->Modify();
	//		auto NewTrack = FocusedMovieScene->AddMasterTrack<UPlayAudioTrack>();
	//		ensure(NewTrack);
	//		GetSequencer()->NotifyMovieSceneDataChanged(EMovieSceneDataChangeType::MovieSceneStructureItemAdded);
	//	}))
	//);
}
TSharedPtr<SWidget> FPlayAudioTrackEditor::BuildOutlinerEditWidget(const FGuid& ObjectBinding, UMovieSceneTrack* Track, const FBuildEditWidgetParams& Params)
{
	auto Callback = [=]() -> TSharedRef<SWidget>
	{
		FMenuBuilder MenuBuilder(true, nullptr);

		const FScopedTransaction Transaction(LOCTEXT("AddPlayAudio_Transaction", "Add PlayAudio"));
		Track->Modify();
		FFrameNumber KeyTime = GetSequencer()->GetGlobalTime().Time.FrameNumber;
		auto OneTrack = Cast<UPlayAudioTrack>(Track);
		UPlayAudioSection* Section = OneTrack->AddNewEvent(KeyTime);
		Section->BindingId = FMovieSceneObjectBindingID(ObjectBinding, GetSequencer()->GetFocusedTemplateID());
		GetSequencer()->NotifyMovieSceneDataChanged(EMovieSceneDataChangeType::MovieSceneStructureItemAdded);
		
		return MenuBuilder.MakeWidget();
	};

	return SNew(SHorizontalBox)
		+ SHorizontalBox::Slot()
		.AutoWidth()
		.VAlign(VAlign_Center)
		[
			FSequencerUtilities::MakeAddButton(LOCTEXT("Rooms", "PlayAudio"), FOnGetContent::CreateLambda(Callback), Params.NodeIsHovered, GetSequencer())
		];
}

void FPlayAudioTrackEditor::BuildObjectBindingTrackMenu(FMenuBuilder& MenuBuilder, const TArray<FGuid>& ObjectBindings, const UClass* ObjectClass)
{
	if (ObjectClass->IsChildOf(AActor::StaticClass()))
	{
		MenuBuilder.AddMenuEntry(
			LOCTEXT("AddPlayAudioTrack", "PlayAudio"),
			LOCTEXT("AddPlayAudioTrackTooltip", "Adds PlayAudio track."),
			FSlateIcon("PlayAudio", "EventIcon"),
			FUIAction(FExecuteAction::CreateLambda([=]
			{
				auto FocusedMovieScene = GetFocusedMovieScene();
				if (FocusedMovieScene == nullptr)
				{
					return;
				}
				const FScopedTransaction Transaction(LOCTEXT("AddPlayAudioTrack_Transaction", "Add PlayAudio Track"));
				FocusedMovieScene->Modify();

				auto NewTrack = FocusedMovieScene->AddTrack<UPlayAudioTrack>(ObjectBindings[0]);
				ensure(NewTrack);
				GetSequencer()->NotifyMovieSceneDataChanged(EMovieSceneDataChangeType::MovieSceneStructureItemAdded);
			}))
		);
	}
}

#undef LOCTEXT_NAMESPACE

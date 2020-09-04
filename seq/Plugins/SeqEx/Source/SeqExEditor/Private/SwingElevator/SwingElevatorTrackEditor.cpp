#include "SwingElevatorTrackEditor.h"
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
#include "SwingElevator/SwingElevatorSection.h"
#include "GameFramework/Actor.h"

#define LOCTEXT_NAMESPACE "SwingElevatorTrackEditor"

class FSwingElevatorSection
	: public ISequencerSection
{
public:
	FSwingElevatorSection(UMovieSceneSection& InSection, TWeakPtr<ISequencer> InSequencer)
		: Section(Cast<USwingElevatorSection>(&InSection))
        , Sequencer(InSequencer)
	{
    }
    ~FSwingElevatorSection()
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
	USwingElevatorSection* Section;
	TWeakPtr<ISequencer> Sequencer;
};


FSwingElevatorTrackEditor::FSwingElevatorTrackEditor(TSharedRef<ISequencer> InSequencer)
	: FMovieSceneTrackEditor(InSequencer)
{
}

TSharedRef<ISequencerTrackEditor> FSwingElevatorTrackEditor::CreateTrackEditor(TSharedRef<ISequencer> InSequencer)
{
	return MakeShareable(new FSwingElevatorTrackEditor(InSequencer));
}

bool FSwingElevatorTrackEditor::SupportsType(TSubclassOf<UMovieSceneTrack> Type) const
{
    return Type == USwingElevatorTrack::StaticClass();
}

void FSwingElevatorTrackEditor::BuildTrackContextMenu(FMenuBuilder& MenuBuilder, UMovieSceneTrack* Track)
{
}

TSharedRef<ISequencerSection> FSwingElevatorTrackEditor::MakeSectionInterface(UMovieSceneSection& SectionObject, UMovieSceneTrack& Track, FGuid ObjectBinding)
{
	return MakeShareable(new FSwingElevatorSection(SectionObject, GetSequencer()));
}

const FSlateBrush* FSwingElevatorTrackEditor::GetIconBrush() const
{
	return nullptr;
}

void FSwingElevatorTrackEditor::BuildAddTrackMenu(FMenuBuilder& MenuBuilder)
{
	MenuBuilder.AddMenuEntry(
		LOCTEXT("AddSwingElevatorTrack", "SwingElevator"),
		LOCTEXT("AddSwingElevatorTrackTooltip", "Adds SwingElevator track."),
		FSlateIcon("SwingElevator", "EventIcon"),
		FUIAction(FExecuteAction::CreateLambda([=]
		{
			auto FocusedMovieScene = GetFocusedMovieScene();
			if (FocusedMovieScene == nullptr)
			{
				return;
			}
			const FScopedTransaction Transaction(LOCTEXT("AddSwingElevatorTrack_Transaction", "Add SwingElevator Track"));
			FocusedMovieScene->Modify();
			auto NewTrack = FocusedMovieScene->AddMasterTrack<USwingElevatorTrack>();
			ensure(NewTrack);
			GetSequencer()->NotifyMovieSceneDataChanged(EMovieSceneDataChangeType::MovieSceneStructureItemAdded);
		}))
	);
}
TSharedPtr<SWidget> FSwingElevatorTrackEditor::BuildOutlinerEditWidget(const FGuid& ObjectBinding, UMovieSceneTrack* Track, const FBuildEditWidgetParams& Params)
{
	auto Callback = [=]() -> TSharedRef<SWidget>
	{
		FMenuBuilder MenuBuilder(true, nullptr);

		const FScopedTransaction Transaction(LOCTEXT("AddSwingElevator_Transaction", "Add SwingElevator"));
		Track->Modify();
		FFrameNumber KeyTime = GetSequencer()->GetGlobalTime().Time.FrameNumber;
		auto OneTrack = Cast<USwingElevatorTrack>(Track);
		OneTrack->AddNewEvent(KeyTime);
		GetSequencer()->NotifyMovieSceneDataChanged(EMovieSceneDataChangeType::MovieSceneStructureItemAdded);

		return MenuBuilder.MakeWidget();
	};

	return SNew(SHorizontalBox)
		+ SHorizontalBox::Slot()
		.AutoWidth()
		.VAlign(VAlign_Center)
		[
			FSequencerUtilities::MakeAddButton(LOCTEXT("Rooms", "SwingElevator"), FOnGetContent::CreateLambda(Callback), Params.NodeIsHovered, GetSequencer())
		];
}

void FSwingElevatorTrackEditor::BuildObjectBindingTrackMenu(FMenuBuilder& MenuBuilder, const TArray<FGuid>& ObjectBindings, const UClass* ObjectClass)
{
	//if (ObjectClass->IsChildOf(AActor::StaticClass()) || ObjectClass->IsChildOf(USceneComponent::StaticClass()))
	//{
	//	MenuBuilder.AddMenuEntry(
	//		LOCTEXT("AddSwingElevatorTrack", "SwingElevator"),
	//		LOCTEXT("AddSwingElevatorTrackTooltip", "Adds SwingElevator track."),
	//		FSlateIcon("SwingElevator", "EventIcon"),
	//		FUIAction(FExecuteAction::CreateLambda([=]
	//		{
	//			auto FocusedMovieScene = GetFocusedMovieScene();
	//			if (FocusedMovieScene == nullptr)
	//			{
	//				return;
	//			}
	//			const FScopedTransaction Transaction(LOCTEXT("AddSwingElevatorTrack_Transaction", "Add SwingElevator Track"));
	//			FocusedMovieScene->Modify();

	//			auto NewTrack = FocusedMovieScene->AddTrack<USwingElevatorTrack>(ObjectBinding);
	//			ensure(NewTrack);
	//			GetSequencer()->NotifyMovieSceneDataChanged(EMovieSceneDataChangeType::MovieSceneStructureItemAdded);
	//		}))
	//	);
	//}
}

#undef LOCTEXT_NAMESPACE

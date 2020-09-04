// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class SeqExEditor : ModuleRules
{
    public SeqExEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" });

        PublicIncludePaths.AddRange(new string[] 
        {
            "SeqExEditor/Public"
        });
        PrivateIncludePaths.AddRange(new string[] 
        {
            "SeqExEditor/Private"
        });
        PublicDependencyModuleNames.AddRange(new string[]
        {
            "Core",
            "CoreUObject",
            "Engine",
            "Slate",
            "AnimGraphRuntime",
            "BlueprintGraph",
        });
        if (Target.bBuildEditor == true)
        {
            PrivateDependencyModuleNames.AddRange(new string[]
            {
                "UnrealEd",
                "GraphEditor",
                "SeqEx",
                "Sequencer",
                "MovieScene",
            });
        }
        PrivateIncludePathModuleNames.AddRange(
            new string[] {
                "Persona",
                "SkeletonEditor",
                "AdvancedPreviewScene",
            }
        );
        PrivateDependencyModuleNames.AddRange(new string[]
        {
            "InputCore",
            "SlateCore",
            "UnrealEd",
            "GraphEditor",
            "PropertyEditor",
            "ContentBrowser",
            "AnimGraph",
			"ToolMenus",

            "LevelSequence",
            "ActorSequence",
            "MovieScene",
        });
        if (Target.bBuildEditor == true)
        {
            PrivateDependencyModuleNames.AddRange(new string[]
            {
                //"EditorStyle",
            });
        }
        //PrivateDependencyModuleNames.AddRange(new string[] { "UnrealEd" });
        PrivateIncludePathModuleNames.AddRange(new string[] {  });
        DynamicallyLoadedModuleNames.AddRange(new string[] {  });
    }
}
// Copyright CrystalVapor 2024, All rights reserved.


#include "CRRecoilPatternEditorStyle.h"

#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyle.h"


FCRRecoilEditorStyle::FCRRecoilEditorStyle()
{
}

FCRRecoilEditorStyle::~FCRRecoilEditorStyle()
{
}

const FName FCRRecoilEditorStyle::TypeName(TEXT("FCRRecoilEditorStyle"));

const FCRRecoilEditorStyle& FCRRecoilEditorStyle::GetDefault()
{
	static FCRRecoilEditorStyle Default;
	return Default;
}

TSharedRef<FSlateStyleSet> FCRRecoilEditorStyle::Create()
{
	TSharedRef<FSlateStyleSet> Style = MakeShareable(new FSlateStyleSet("RecoilPatternEditorStyle"));

	Style->SetContentRoot(IPluginManager::Get().FindPlugin("CrystalRecoil")->GetBaseDir() / TEXT("Resources"));
	
	return Style;
}

void FCRRecoilEditorStyle::GetResources(TArray<const FSlateBrush*>& OutBrushes) const
{
	// Add any brush resources here so that Slate can correctly atlas and reference them
}

// Copyright CrystalVapor 2024, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Styling/SlateWidgetStyle.h"
#include "Styling/SlateWidgetStyleContainerBase.h"
#include "CRRecoilPatternEditorStyle.generated.h"

USTRUCT()
struct CRYSTALRECOILEDITOR_API FCRRecoilEditorStyle : public FSlateWidgetStyle
{
	GENERATED_BODY()

	FCRRecoilEditorStyle();

	virtual ~FCRRecoilEditorStyle() override;

	virtual void GetResources(TArray<const FSlateBrush*>& OutBrushes) const override;

	virtual const FName GetTypeName() const override { return TypeName;	};

	static const FCRRecoilEditorStyle& GetDefault();

	static TSharedRef<FSlateStyleSet> Create();

	inline static const FName TypeName = FName("FCRRecoilEditorStyle");
};

UCLASS(MinimalAPI, HideCategories = "Object")
class UCRRecoilEditorStyleWidgetStyle : public USlateWidgetStyleContainerBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Meta = (ShowOnlyInnerProperties), Category = "Appearance")
	FCRRecoilEditorStyle WidgetStyle;

	virtual const FSlateWidgetStyle* const GetStyle() const override
	{
		return &WidgetStyle;
	}
};

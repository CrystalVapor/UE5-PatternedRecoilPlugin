// Copyright CrystalVapor 2024, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Styling/SlateWidgetStyle.h"
#include "Styling/SlateWidgetStyleContainerBase.h"

#include "CRRecoilPatternEditorStyle.generated.h"

/**
 * 
 */
USTRUCT()
struct CRYSTALRECOILEDITOR_API FCRRecoilEditorStyle : public FSlateWidgetStyle
{
	GENERATED_USTRUCT_BODY()

	FCRRecoilEditorStyle();
	virtual ~FCRRecoilEditorStyle();

	// FSlateWidgetStyle
	virtual void GetResources(TArray<const FSlateBrush*>& OutBrushes) const override;
	static const FName TypeName;
	virtual const FName GetTypeName() const override { return TypeName; };
	static const FCRRecoilEditorStyle& GetDefault();
	static TSharedRef<FSlateStyleSet> Create();
};

/**
 */
UCLASS(hidecategories=Object, MinimalAPI)
class UCRRecoilEditorStyleWidgetStyle : public USlateWidgetStyleContainerBase
{
	GENERATED_BODY()

public:
	UPROPERTY(Category=Appearance, EditAnywhere, meta=(ShowOnlyInnerProperties))
	FCRRecoilEditorStyle WidgetStyle;

	virtual const struct FSlateWidgetStyle* const GetStyle() const override
	{
		return static_cast<const struct FSlateWidgetStyle*>(&WidgetStyle);
	}
};

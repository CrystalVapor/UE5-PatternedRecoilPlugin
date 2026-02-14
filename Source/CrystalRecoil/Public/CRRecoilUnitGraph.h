// Copyright CrystalVapor 2024, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "CRRecoilUnitGraph.generated.h"

UENUM()
enum class ECRRecoilUnitGraphRearrangePolicy : uint8
{
	// Sorts units from bottom to top (Standard vertical recoil)
	AscendByY,

	// Sorts units from top to bottom (Inverted/Downward recoil)
	DescendByY,

	// Sorts units from left to right (Horizontal sweep patterns)
	AscendByX,

	// Sorts units from right to left
	DescendByX
};

USTRUCT()
struct FCRRecoilUnit
{
	GENERATED_BODY()
	;

	FCRRecoilUnit()
		: ID(0)
		, Position(FVector2f::ZeroVector)
	{
	}

	FCRRecoilUnit(const int32 InID, const FVector2f& InLocation)
		: ID(InID)
		, Position(InLocation)
	{
	}

	bool operator==(const FCRRecoilUnit& Other) const
	{
		return ID == Other.ID;
	}

	bool operator>(const FCRRecoilUnit& Other) const
	{
		return Position.Y - Other.Position.Y > 0.f;
	}

	UPROPERTY(VisibleAnywhere, Category = "Unit")
	uint32 ID;

	UPROPERTY(EditAnywhere, Category = "Unit")
	FVector2f Position;
};

DECLARE_MULTICAST_DELEGATE_OneParam(FCRRecoilUnitChangedDelegate, int32 /* Index */)

UCLASS()
class CRYSTALRECOIL_API UCRRecoilUnitGraph : public UObject
{
	GENERATED_BODY()

public:
	FCRRecoilUnit CreateNewUnit(const FVector2f& RecoilUnitLocation);

	int32 AddUnit(const FVector2f& RecoilUnitLocation);

	void InsertUnit(const FCRRecoilUnit& RecoilUnit, const int32 Index);

	void RemoveAt(const int32 Index);

	void Empty();

	FVector2f GetUnitLocationAt(const int32 Index);

	FCRRecoilUnit& GetUnitAt(const int32 Index);

	const FCRRecoilUnit& GetUnitAt(const int32 Index) const;

	int32 GetUnitCount() const;

	void RemoveUnitByID(uint32 ID);

	FCRRecoilUnit* GetUnitByID(uint32 ID);

	TArray<FCRRecoilUnit>& GetRawData();

	// Rearrange the ID of the units
	void RearrangeID();

	// Sort the units by the sort policy
	void RearrangeUnits();

	FCRRecoilUnitChangedDelegate OnUnitRemoved;

	FCRRecoilUnitChangedDelegate OnUnitAdded;

	UPROPERTY(EditAnywhere, Transient, NonTransactional, Category = "Editor Graph")
	bool bDrawUnitNumbers = true;

	UPROPERTY(EditAnywhere, Transient, NonTransactional, Meta = (ClampMin = 4.f, ClampMax = 24.f), Category = "Editor Graph")
	float UnitNumbersFontSize = 8.f;

	UPROPERTY(EditAnywhere, Transient, NonTransactional, Meta = (ClampMin = 5.f, ClampMax = 100.f), Category = "Editor Graph")
	float UnitDrawSize = 18.f;

	UPROPERTY(EditAnywhere, Transient, NonTransactional, Meta = (ClampMin = 0.f, ClampMax = 1.f), Category = "Editor Graph")
	float UnitLinesOpacity = 0.3f;

	UPROPERTY(EditAnywhere, Transient, NonTransactional, Meta = (ClampMin = 4.f, ClampMax = 24.f), Category = "Editor Graph")
	float OriginDrawSize = 8.f;

	UPROPERTY(EditAnywhere, Transient, NonTransactional, Meta = (ClampMin = 0.f, ClampMax = 1.f), Category = "Editor Graph")
	float OriginOpacity = 0.5f;

	/**
	* Determines how recoil units are automatically sorted/rearranged in the list
	* AscendByY: Standard behavior (Shot 1 is at bottom, Shot 10 is at top)
	* AscendByX: Useful for horizontal sweeps (Shot 1 is Left, Shot 10 is Right)
	*/
	UPROPERTY(EditAnywhere, NonTransactional, Category = "Recoil Pattern")
	ECRRecoilUnitGraphRearrangePolicy RearrangePolicy = ECRRecoilUnitGraphRearrangePolicy::AscendByY;

	#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	#endif

protected:
	// Array of Units on the Graph
	UPROPERTY(EditAnywhere, Category = "Recoil Pattern")
	TArray<FCRRecoilUnit> RecoilUnits;

	UPROPERTY()
	uint32 NextID = 0;
};

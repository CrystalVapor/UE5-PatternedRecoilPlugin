// Copyright CrystalVapor 2024, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "CRRecoilUnitGraph.generated.h"

UENUM()
enum class ECRRecoilUnitGraphRearrangePolicy
{
	AscendByY,
	DescendByY,
	AscendByX,
	DescendByX
};

USTRUCT()
struct FCRRecoilUnit
{
	GENERATED_BODY();
public:
	FCRRecoilUnit():ID(0),Location(FVector2d(0,0)){}
	FCRRecoilUnit(int32 InID, const FVector2d& InLocation):ID(InID),Location(InLocation){}
	bool operator==(const FCRRecoilUnit& Other) const
	{
		return ID == Other.ID;
	}
	bool operator>(const FCRRecoilUnit& Other) const
	{
		return Location.Y-Other.Location.Y > 0;
	}
	UPROPERTY(VisibleAnywhere)
	uint32 ID;
	UPROPERTY(EditAnywhere)
	FVector2D Location;
};

DECLARE_MULTICAST_DELEGATE_OneParam(FCRRecoilUnitChangedDelegate, int32 /*Index*/)

/**
 * 
 */
UCLASS()
class CRYSTALRECOIL_API UCRRecoilUnitGraph : public UObject
{
	GENERATED_BODY()
public:
	FCRRecoilUnit CreateNewUnit(const FVector2d& RecoilUnitLocation);
	int32 AddUnit(const FVector2d& RecoilUnitLocation);
	void InsertUnit(FCRRecoilUnit& RecoilUnit, int32 Index);
	void RemoveAt(int32 Index);
	void Empty();
	FVector2D GetUnitLocationAt(int32 Index);
	FCRRecoilUnit& GetUnitAt(int32 Index);
	const FCRRecoilUnit& GetUnitAt(int32 Index) const;
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
	
	UPROPERTY(EditAnywhere, Transient, NonTransactional)
	bool bDrawUnitNumbers = true;
	UPROPERTY(EditAnywhere, Transient, NonTransactional)
	bool bDrawUnitLines = true;
	UPROPERTY(EditAnywhere, NonTransactional)
	ECRRecoilUnitGraphRearrangePolicy RearrangePolicy = ECRRecoilUnitGraphRearrangePolicy::AscendByY;
protected:
	UPROPERTY(EditAnywhere)
	TArray<FCRRecoilUnit> RecoilUnits;
	UPROPERTY()
	uint32 NextID = 0;
};

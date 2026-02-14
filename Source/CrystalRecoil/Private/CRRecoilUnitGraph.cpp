// Copyright CrystalVapor 2024, All rights reserved.

#include "CRRecoilUnitGraph.h"

FCRRecoilUnit UCRRecoilUnitGraph::CreateNewUnit(const FVector2f& RecoilUnitLocation)
{
	return FCRRecoilUnit(NextID++, RecoilUnitLocation);
}

int32 UCRRecoilUnitGraph::AddUnit(const FVector2f& RecoilUnitLocation)
{
	RecoilUnits.Add(FCRRecoilUnit(NextID++, RecoilUnitLocation));
	OnUnitAdded.Broadcast(NextID - 1);
	return NextID - 1;
}

void UCRRecoilUnitGraph::InsertUnit(const FCRRecoilUnit& RecoilUnit, const int32 Index)
{
	OnUnitAdded.Broadcast(RecoilUnit.ID);
	RecoilUnits.Insert(RecoilUnit, Index);
}

void UCRRecoilUnitGraph::RemoveAt(const int32 Index)
{
	OnUnitRemoved.Broadcast(RecoilUnits[Index].ID);
	RecoilUnits.RemoveAt(Index);

	if (RecoilUnits.Num() == 0)
	{
		NextID = 0;
	}
}

void UCRRecoilUnitGraph::Empty()
{
	RecoilUnits.Empty();

	if (RecoilUnits.Num() == 0)
	{
		NextID = 0;
	}
}

FVector2f UCRRecoilUnitGraph::GetUnitLocationAt(const int32 Index)
{
	return GetUnitAt(Index).Position;
}

FCRRecoilUnit& UCRRecoilUnitGraph::GetUnitAt(const int32 Index)
{
	return RecoilUnits[Index];
}

const FCRRecoilUnit& UCRRecoilUnitGraph::GetUnitAt(const int32 Index) const
{
	return RecoilUnits[Index];
}

int32 UCRRecoilUnitGraph::GetUnitCount() const
{
	return RecoilUnits.Num();
}

void UCRRecoilUnitGraph::RemoveUnitByID(uint32 ID)
{
	OnUnitRemoved.Broadcast(ID);
	RecoilUnits.RemoveAll([ID](const FCRRecoilUnit& Unit) { return Unit.ID == ID; });

	if (RecoilUnits.Num() == 0)
	{
		NextID = 0;
	}
}

FCRRecoilUnit* UCRRecoilUnitGraph::GetUnitByID(uint32 ID)
{
	return RecoilUnits.FindByPredicate([ID](const FCRRecoilUnit& Unit) { return Unit.ID == ID; });
}

TArray<FCRRecoilUnit>& UCRRecoilUnitGraph::GetRawData()
{
	return RecoilUnits;
}

void UCRRecoilUnitGraph::RearrangeID()
{
	for (const auto& RecoilUnit : RecoilUnits)
	{
		OnUnitRemoved.Broadcast(RecoilUnit.ID);
	}

	NextID = 0;

	for (auto& RecoilUnit : RecoilUnits)
	{
		RecoilUnit.ID = NextID++;
		OnUnitAdded.Broadcast(RecoilUnit.ID);
	}
}

void UCRRecoilUnitGraph::RearrangeUnits()
{
	switch (RearrangePolicy)
	{
		case ECRRecoilUnitGraphRearrangePolicy::AscendByY:
		{
			auto SortLambda = [](const FCRRecoilUnit& A, const FCRRecoilUnit& B)
			{
				if (A.Position.Y != B.Position.Y)
				{
					return A.Position.Y < B.Position.Y;
				}

				return A.ID < B.ID;
			};
			Algo::Sort(RecoilUnits, SortLambda);
		}
		break;
		case ECRRecoilUnitGraphRearrangePolicy::DescendByY:
		{
			auto SortLambda = [](const FCRRecoilUnit& A, const FCRRecoilUnit& B)
			{
				if (A.Position.Y != B.Position.Y)
				{
					return A.Position.Y > B.Position.Y;
				}

				return A.ID < B.ID;
			};
			Algo::Sort(RecoilUnits, SortLambda);
		}
		break;
		case ECRRecoilUnitGraphRearrangePolicy::AscendByX:
		{
			auto SortLambda = [](const FCRRecoilUnit& A, const FCRRecoilUnit& B)
			{
				if (A.Position.X != B.Position.X)
				{
					return A.Position.X < B.Position.X;
				}

				return A.ID < B.ID;
			};
			Algo::Sort(RecoilUnits, SortLambda);
		}
		break;
		case ECRRecoilUnitGraphRearrangePolicy::DescendByX:
		{
			auto SortLambda = [](const FCRRecoilUnit& A, const FCRRecoilUnit& B)
			{
				if (A.Position.X != B.Position.X)
				{
					return A.Position.X > B.Position.X;
				}

				return A.ID < B.ID;
			};
			Algo::Sort(RecoilUnits, SortLambda);
		}
		break;
	}
}

#if WITH_EDITOR
void UCRRecoilUnitGraph::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.GetMemberPropertyName() == GET_MEMBER_NAME_STRING_VIEW_CHECKED(UCRRecoilUnitGraph, RecoilUnits))
	{
		if (RecoilUnits.Num() == 0)
		{
			NextID = 0;
			return;
		}

		// Process in reverse order to fix duplicate IDs
		// Example: [0, 1, 2, 3, 3] becomes [0, 1, 2, 3, 4]
		for (int32 i = RecoilUnits.Num() - 1; i >= 0; --i)
		{
			FCRRecoilUnit& Unit = RecoilUnits[i];

			// Check if this ID is a duplicate (appears earlier in the array)
			bool bIsDuplicate = false;
			for (int32 j = 0; j < i; ++j)
			{
				if (RecoilUnits[j].ID == Unit.ID)
				{
					bIsDuplicate = true;
					break;
				}
			}

			// If duplicate, assign a new unique ID
			if (bIsDuplicate)
			{
				Unit.ID = NextID++;
			}
		}

		// Ensure NextID is higher than all existing IDs
		for (const FCRRecoilUnit& Unit : RecoilUnits)
		{
			if (Unit.ID >= NextID)
			{
				NextID = Unit.ID + 1;
			}
		}
	}
}
#endif

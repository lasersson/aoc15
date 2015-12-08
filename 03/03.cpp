#include <aoc.h>

struct house
{
	int x, y;
};

struct house_array
{
	house *Array;
	int Count;
	int Capacity;
};

static void
ExpandHouseArray(house_array *Houses, int Capacity)
{
	Capacity = Max(Capacity, 1);
	Houses->Array = (house *)realloc(Houses->Array, sizeof(house) * Capacity);
	Assert(Houses->Array);
	Houses->Capacity = Capacity;
}

static void
PushHouse(house_array *Houses, house House)
{
	if (Houses->Count == Houses->Capacity)
	{
		ExpandHouseArray(Houses, Houses->Capacity * 2);
	}
	Houses->Array[Houses->Count++] = House;
}

static bool
HouseIsVisited(house_array *Houses, house House)
{
	bool Result = false;
	for (int It = 0; It < Houses->Count; ++It)
	{
		house *HouseIt = Houses->Array + It;
		if (HouseIt->x == House.x && HouseIt->y == House.y)
		{
			Result = true;
			break;
		}
	}

	return Result;
}

static void
VisitHouse(house_array *Houses, house House)
{
	if (!HouseIsVisited(Houses, House))
	{
		PushHouse(Houses, House);
	}
}

static void
Solve(input_file Input)
{
	house_array VisitedHouses = {};

	house CurrentHouse = { 0, 0 };
	VisitHouse(&VisitedHouses, CurrentHouse);
	for (int It = 0; It < Input.Length; ++It)
	{
		char c = Input.Contents[It];
		switch (c)
		{
			case '^': ++CurrentHouse.y; break;
			case '>': ++CurrentHouse.x; break;
			case 'v': --CurrentHouse.y; break;
			case '<': --CurrentHouse.x; break;
			default: Assert(!"Invalid input");
		}
		VisitHouse(&VisitedHouses, CurrentHouse);
	}

	printf("%d\n", VisitedHouses.Count);
}

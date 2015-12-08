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

#define SANTA_MAX_COUNT 2

static void
Solve(input_file Input)
{
	house_array VisitedHouses = {};

	for (int SantaCount = 0; SantaCount < 2; ++SantaCount)
	{
		VisitedHouses.Count = 0;

		house SantaHouse[SANTA_MAX_COUNT] = {};
		VisitHouse(&VisitedHouses, SantaHouse[0]);
		for (int It = 0; It < Input.Length; ++It)
		{
			house *CurSantaHouse = SantaHouse + (It % (SantaCount + 1));
			char c = Input.Contents[It];
			switch (c)
			{
				case '^': ++CurSantaHouse->y; break;
				case '>': ++CurSantaHouse->x; break;
				case 'v': --CurSantaHouse->y; break;
				case '<': --CurSantaHouse->x; break;
				default: Assert(!"Invalid input");
			}
			VisitHouse(&VisitedHouses, *CurSantaHouse);
		}
		printf("%d\n", VisitedHouses.Count);
	}

	free(VisitedHouses.Array);
}

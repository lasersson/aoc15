#include <aoc.h>

struct house
{
	int x, y;
};

static bool
HouseIsVisited(house *Houses, house House)
{
	bool Result = false;
	for (int It = 0; It < GACount(Houses); ++It)
	{
		house *HouseIt = Houses + It;
		if (HouseIt->x == House.x && HouseIt->y == House.y)
		{
			Result = true;
			break;
		}
	}

	return Result;
}

static house *
VisitHouse(house *Houses, house House)
{
	if (!Houses || !HouseIsVisited(Houses, House))
	{
		GAPush(Houses, House);
	}
	return Houses;
}

#define SANTA_MAX_COUNT 2

static void
Solve(input_file Input)
{
	house *VisitedHouses = nullptr;

	for (int SantaCount = 0; SantaCount < 2; ++SantaCount)
	{
		house SantaHouse[SANTA_MAX_COUNT] = {};
		VisitedHouses = VisitHouse(VisitedHouses, SantaHouse[0]);
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
			VisitedHouses = VisitHouse(VisitedHouses, *CurSantaHouse);
		}
		printf("%d\n", GACount(VisitedHouses));

		GACount(VisitedHouses) = 0;
	}

	GAFree(VisitedHouses);
}

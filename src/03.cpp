#include <aoc.h>

union house
{
	struct
	{
		s16 x, y;
	};
	u32 v;
};

#define SANTA_MAX_COUNT 2

static output
Solve(input Input)
{
	output Output;
	hash_table VisitedHouses = InitHashTable(256);
	for (int SantaCount = 0; SantaCount < 2; ++SantaCount)
	{
		house SantaHouse[SANTA_MAX_COUNT] = {};
		InsertHash(&VisitedHouses, SantaHouse[0].v);
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
			InsertHash(&VisitedHouses, CurSantaHouse->v);
		}

		Output.v[SantaCount] = VisitedHouses.Count;
		Clear(&VisitedHouses);
	}

	FreeHashTable(&VisitedHouses);
	return Output;
}

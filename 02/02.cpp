#include <aoc.h>

static void
Solve(input_file Input)
{
	int TotalArea = 0;

	char *Delim = "x\n";
	char *Token = strtok(Input.Contents, Delim);
	while (Token)
	{
		int Length = atoi(Token);
		int Width = atoi(strtok(0, Delim));
		int Height = atoi(strtok(0, Delim));

		int LW = Length * Width;
		int WH = Width * Height;
		int HL = Height * Length;
		int MinSide = Min(Min(LW, WH), HL);

		int Area = 2 * LW + 2 * WH + 2 * HL + MinSide;
		TotalArea += Area;

		Token = strtok(0, Delim);
	}

	printf("%d\n", TotalArea);
}

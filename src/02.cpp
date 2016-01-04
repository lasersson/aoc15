#include <aoc.h>

static output
Solve(input Input)
{
	int TotalArea = 0;
	int TotalRibbonLength = 0;

	char *Delim = "x\n";
	char *Token = strtok(Input.Contents, Delim);
	while (Token)
	{
		int Length = atoi(Token);
		int Width = atoi(strtok(0, Delim));
		int Height = atoi(strtok(0, Delim));

		int AreaLW = Length * Width;
		int AreaWH = Width * Height;
		int AreaHL = Height * Length;
		int MinFaceArea = Min(Min(AreaLW, AreaWH), AreaHL);
		int Area = 2 * AreaLW + 2 * AreaWH + 2 * AreaHL + MinFaceArea;
		TotalArea += Area;

		int PerimLW = 2 * Length + 2 * Width;
		int PerimWH = 2 * Width + 2 * Height;
		int PerimHL = 2 * Height + 2 * Length;
		int MinFacePerimeter = Min(Min(PerimLW, PerimWH), PerimHL);
		int Volume = Length * Width * Height;
		int RibbonLength = MinFacePerimeter + Volume;
		TotalRibbonLength += RibbonLength;

		Token = strtok(0, Delim);
	}

	output Output = { TotalArea, TotalRibbonLength };
	return Output;
}

#include <aoc.h>

#define GRID_WIDTH 1000
#define GRID_HEIGHT 1000
static bool LightOnGrid[GRID_WIDTH * GRID_HEIGHT];
static int LightBrightnessGrid[GRID_WIDTH * GRID_HEIGHT];

enum action
{
	Action_Unknown,
	Action_TurnOn,
	Action_TurnOff,
	Action_Toggle,
};

struct pos
{
	int x, y;
};

static void
Solve(input_file Input)
{
	char *Delim = "\n, ";
	char *Token = strtok(Input.Contents, Delim);

	int LightsLitCount = 0;
	int TotalBrightness = 0;
	while (Token)
	{
		action Action = Action_Unknown;
		if (strcmp(Token, "toggle") == 0)
		{
			Action = Action_Toggle;
		}
		else
		{
			Token = strtok(nullptr, Delim);
			if (strcmp(Token, "on") == 0)
			{
				Action = Action_TurnOn;
			}
			else if (strcmp(Token, "off") == 0)
			{
				Action = Action_TurnOff;
			}
		}

		pos TopLeftCorner;
		TopLeftCorner.x = atoi(strtok(nullptr, Delim));
		TopLeftCorner.y = atoi(strtok(nullptr, Delim));
		Assert(TopLeftCorner.x >= 0 && TopLeftCorner.x < GRID_WIDTH);
		Assert(TopLeftCorner.y >= 0 && TopLeftCorner.y < GRID_HEIGHT);
		strtok(nullptr, Delim);
		pos BottomRightCorner;
		BottomRightCorner.x = atoi(strtok(nullptr, Delim));
		BottomRightCorner.y = atoi(strtok(nullptr, Delim));
		Assert(BottomRightCorner.x >= 0 && BottomRightCorner.x < GRID_WIDTH);
		Assert(BottomRightCorner.y >= 0 && BottomRightCorner.y < GRID_HEIGHT);

		int RowCount = BottomRightCorner.y - TopLeftCorner.y + 1;
		int ColCount = BottomRightCorner.x - TopLeftCorner.x + 1;
		for (int RowIt = 0; RowIt < RowCount; ++RowIt)
		{
			for (int ColIt = 0; ColIt < ColCount; ++ColIt)
			{
				int LightIndex = (TopLeftCorner.y + RowIt) * GRID_WIDTH + (TopLeftCorner.x + ColIt);
				bool *LightIsOn = LightOnGrid + LightIndex;
				int *LightBrightness = LightBrightnessGrid + LightIndex;
				switch (Action)
				{
					case Action_TurnOn:
					{
						if (!*LightIsOn)
						{
							++LightsLitCount;
							*LightIsOn = true;
						}

						*LightBrightness += 1;
						++TotalBrightness;
						break;
					}
					case Action_TurnOff:
					{
						if (*LightIsOn)
						{
							--LightsLitCount;
							*LightIsOn = false;
						}

						if (*LightBrightness > 0)
						{
							*LightBrightness -= 1;
							--TotalBrightness;
						}
						break;
					}
					case Action_Toggle:
					{
						*LightIsOn = !*LightIsOn;
						if (*LightIsOn)
						{
							++LightsLitCount;
						}
						else
						{
							--LightsLitCount;
						}
						
						*LightBrightness += 2;
						TotalBrightness += 2;
						break;
					}

					default:
					{
						Assert(!"Unknown action");
						break;
					}
				}
			}
		}

		Token = strtok(nullptr, Delim);
	}

	printf("%d\n", LightsLitCount);
	printf("%d\n", TotalBrightness);
}

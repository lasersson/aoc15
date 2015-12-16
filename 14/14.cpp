#include <aoc.h>

struct reindeer
{
	int Speed;
	int FlyTime;
	int RestTime;
};

DeclareArray(reindeer_array, reindeer);

static int
GetDistanceAfter(reindeer Reindeer, int Duration)
{
	int Period = Reindeer.FlyTime + Reindeer.RestTime;
	int PeriodCount = Duration / Period;
	int FlyTime = PeriodCount * Reindeer.FlyTime;

	int RemainingSeconds = Duration - PeriodCount * Period;
	FlyTime += Min(Reindeer.FlyTime, RemainingSeconds);

	int Distance = FlyTime * Reindeer.Speed;
	return Distance;
}

static void
Solve(input_file Input)
{
	reindeer_array Reindeers = {};
	char *Delim = " .\n\r";
	char *Token = strtok(Input.Contents, Delim);
	while (Token)
	{
		reindeer Reindeer = {};

		char *Name = Token;
		Token = strtok(nullptr, Delim); // can
		Token = strtok(nullptr, Delim); // fly
		Token = strtok(nullptr, Delim); // Speed
		Reindeer.Speed = atoi(Token); 
		Token = strtok(nullptr, Delim); // km/s
		Token = strtok(nullptr, Delim); // for
		Token = strtok(nullptr, Delim); // FlyTime
		Reindeer.FlyTime = atoi(Token);
		Token = strtok(nullptr, Delim); // seconds,
		Token = strtok(nullptr, Delim); // but
		Token = strtok(nullptr, Delim); // then
		Token = strtok(nullptr, Delim); // must
		Token = strtok(nullptr, Delim); // rest
		Token = strtok(nullptr, Delim); // for
		Token = strtok(nullptr, Delim); // RestTime
		Reindeer.RestTime = atoi(Token);
		Token = strtok(nullptr, Delim); // seconds

		ArrayAdd(&Reindeers, Reindeer, reindeer);

		Token = strtok(nullptr, Delim); // Name
	}

	int Duration = 2503;
	int *Distances = (int *)malloc(sizeof(int) * Reindeers.Count);
	int *Scores = (int *)malloc(sizeof(int) * Reindeers.Count);
	memset(Scores, 0, sizeof(int) * Reindeers.Count);

	for (int Time = 1; Time <= Duration; ++Time)
	{
		int MaxDistance = 0;
		for (int It = 0; It < Reindeers.Count; ++It)
		{
			Distances[It] = GetDistanceAfter(Reindeers.Array[It], Time);
			MaxDistance = Max(MaxDistance, Distances[It]);
		}
		for (int It = 0; It < Reindeers.Count; ++It)
		{
			if (Distances[It] == MaxDistance)
			{
				Scores[It] += 1;
			}
		}
	}

	int MaxScore = 0;
	int MaxDistance = 0;
	for (int It = 0; It < Reindeers.Count; ++It)
	{
		MaxScore = Max(MaxScore, Scores[It]);
		MaxDistance = Max(MaxDistance, Distances[It]);
	}

	printf("%d\n", MaxDistance);
	printf("%d\n", MaxScore);

	free(Reindeers.Array);
	free(Distances);
	free(Scores);
}

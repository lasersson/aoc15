#include <aoc.h>

#define ArrayLength(x) (sizeof(x)/sizeof((x)[0]))

static void
Solve(input_file Input)
{
	static char *Vowels = "aeiou";
	static int VowelCount = (int)strlen(Vowels);
	static char *DisallowedPairs[] = { "ab", "cd", "pq", "xy" };

	char *Word = strtok(Input.Contents, "\n");
	int GoodWordCount1 = 0;
	int GoodWordCount2 = 0;
	while (Word)
	{
		int WordLength = (int)strlen(Word);

		int WordVowelCount = 0;
		bool WordHasDisallowedPair = false;
		bool WordHasRepeatingLetter1 = false;

		bool WordHasRepeatingLetter2 = false;
		bool WordHasRepeatingPair = false;
		for (int It = 0; It < WordLength; ++It)
		{
			char c = Word[It];
			char pc = It > 0 ? Word[It - 1] : 0;
			char ppc = It > 1 ? Word[It - 2] : 0;
			
			for (int vIt = 0; vIt < VowelCount; ++vIt)
			{
				if (c == Vowels[vIt])
				{
					++WordVowelCount;
				}
			}

			if (c == pc)
			{
				WordHasRepeatingLetter1 = true;
			}

			for (int PairIt = 0; PairIt < ArrayLength(DisallowedPairs); ++PairIt)
			{
				char *Pair = DisallowedPairs[PairIt];
				if (pc == Pair[0] && c == Pair[1])
				{
					WordHasDisallowedPair = true;
				}
			}
			if ((c == 'b' && pc == 'a') ||
				(c == 'd' && pc == 'c') ||
				(c == 'q' && pc == 'p') ||
				(c == 'y' && pc == 'x'))
			{
				WordHasDisallowedPair = true;
			}

			if (c == ppc)
			{
				WordHasRepeatingLetter2 = true;
			}

			for (int PairIt = 0; PairIt < (It - 2); ++PairIt)
			{
				if (pc == Word[PairIt] && c == Word[PairIt + 1])
				{
					WordHasRepeatingPair = true;
				}
			}
		}

		if (WordVowelCount >= 3 && WordHasRepeatingLetter1 && !WordHasDisallowedPair)
		{
			++GoodWordCount1;
		}

		if (WordHasRepeatingLetter2 && WordHasRepeatingPair)
		{
			++GoodWordCount2;
		}

		Word = strtok(nullptr, "\n");
	}

	printf("%d\n", GoodWordCount1);
	printf("%d\n", GoodWordCount2);
}

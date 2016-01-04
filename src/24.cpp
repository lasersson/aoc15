#include <aoc.h>
#include <stdint.h>

#define WEIGHT_COUNT_MAX 32

typedef uint32_t u32;
typedef uint64_t u64;

static int
ComputeWeight(int *Weights, u32 Combination)
{
	int Sum = 0;
	for (int It = 0; It < GACount(Weights); ++It)
	{
		if ((1 << It) & Combination)
		{
			Sum += Weights[It];
		}
	}
	return Sum;
}

static u64
ComputeQuantumEntanglement(int *Weights, u32 Combination)
{
	u64 QuantumEntanglement = 1;
	for (int It = 0; It < GACount(Weights); ++It)
	{
		if ((1 << It) & Combination)
		{
			QuantumEntanglement *= (u64)Weights[It];
		}
	}
	return QuantumEntanglement;
}

static u32
CountBitsSet(u32 x)
{
	x = x - ((x >> 1) & 0x55555555);
	x = (x & 0x33333333) + ((x >> 2) & 0x33333333);
	int Result = (((x + (x >> 4)) & 0x0F0F0F0F) * 0x01010101) >> 24;
	return Result;
}

static bool
HasValidCombinations(u32 **Combinations, u32 CurMask, int CompartmentIndex, int CompartmentCount)
{
	bool Result = false;
	for (int CombIt = 0; CombIt < WEIGHT_COUNT_MAX && !Result; ++CombIt)
	{
		u32 *CombArray = Combinations[CombIt];
		for (int It = 0; It < GACount(CombArray); ++It)
		{
			u32 Comb = CombArray[It];
			if ((Comb & ~CurMask) == Comb)
			{
				if (CompartmentIndex < CompartmentCount - 1)
				{
					Result = HasValidCombinations(Combinations, CurMask | Comb, CompartmentIndex + 1, CompartmentCount);
				}
				else
				{
					Result = true;
					break;
				}
			}
		}
	}
	return Result;
}

static u64
FindMinQuantumEntanglement(int *Weights, int TotalWeight, int CompartmentCount)
{
	u32 WeightCount = GACount(Weights);
	Assert(WeightCount < WEIGHT_COUNT_MAX);
	int TargetWeight = TotalWeight / CompartmentCount;

	u32 *Combinations[WEIGHT_COUNT_MAX] = {};

	struct combination
	{
		u32 Mask;
		u32 MinBit;
	};
	combination *CombStack = nullptr;
	combination CombInit = { 0, WeightCount };
	GAPush(CombStack, CombInit);
	while (GACount(CombStack))
	{
		combination Comb = GATop(CombStack);
		GAPop(CombStack);
		u32 CombCount = CountBitsSet(Comb.Mask);
		int CombWeight = ComputeWeight(Weights, Comb.Mask);
		if (CombWeight == TargetWeight)
		{
			u32 *CombArray = Combinations[CombCount];
			GAPush(CombArray, Comb.Mask);
			Combinations[CombCount] = CombArray;
		}
		else if (Comb.MinBit > 0 && CombWeight < TargetWeight)
		{
			combination NewComb = {};
			NewComb.MinBit = Comb.MinBit - 1;

			NewComb.Mask = Comb.Mask;
			GAPush(CombStack, NewComb);
			NewComb.Mask = Comb.Mask | (1 << NewComb.MinBit);
			GAPush(CombStack, NewComb);
		}
	}
	GAFree(CombStack);

	u64 MinQuantumEntanglement = ~(0ull);
	for (u32 CombIt = 0; CombIt < WeightCount; ++CombIt)
	{
		u32 *CombArray = Combinations[CombIt];
		for (int It = 0; It < GACount(CombArray); ++It)
		{
			u32 Comb = CombArray[It];
			u64 QuantumEntanglement = ComputeQuantumEntanglement(Weights, Comb);
			if (QuantumEntanglement < MinQuantumEntanglement &&
				HasValidCombinations(Combinations, Comb, 1, CompartmentCount))
			{
				MinQuantumEntanglement = QuantumEntanglement;
			}
		}
	}

	for (int It = 0; It < WEIGHT_COUNT_MAX; ++It)
	{
		u32 *CombArray = Combinations[It];
		if (CombArray)
		{
			GAFree(CombArray);
		}
	}

	return MinQuantumEntanglement;
}

static output
Solve(input Input)
{
	int TotalWeight = 0;
	int *Weights = nullptr;
	char *Delim = "\r\n";
	char *Token = strtok(Input.Contents, Delim);
	while (Token)
	{
		int Weight = atoi(Token);
		TotalWeight += Weight;
		GAPush(Weights, Weight);
		Token = strtok(nullptr, Delim);
	}

	output Output = {};
	Output.a = FindMinQuantumEntanglement(Weights, TotalWeight, 3);
	Output.b = FindMinQuantumEntanglement(Weights, TotalWeight, 4);
	return Output;
}

#include <aoc.h>

struct ingredient
{
	int Capacity;
	int Durability;
	int Flavor;
	int Texture;
	int Calories;
};

static ingredient
Mul(ingredient i, int x)
{
	i.Capacity *= x;
	i.Durability *= x;
	i.Flavor *= x;
	i.Texture *= x;
	i.Calories *= x;
	return i;
}

static ingredient
Add(ingredient i, ingredient j)
{
	i.Capacity += j.Capacity;
	i.Durability += j.Durability;
	i.Flavor += j.Flavor;
	i.Texture += j.Texture;
	i.Calories += j.Calories;
	return i;
}

static output
Max(output x, output y)
{
	x.a = Max(x.a, y.a);
	x.b = Max(x.b, y.b);
	return x;
}

static output
CalcMaxScore(int IngredientIndex, ingredient CurrentTotal, ingredient *Ingredients, int xSum)
{
	output MaxScore = {};
	for (int x = 0; x <= (100 - xSum); ++x)
	{
		ingredient NewTotal = Add(Mul(Ingredients[IngredientIndex], x), CurrentTotal);
		output Score = {};
		int xNew = xSum + x;
		if (IngredientIndex == GACount(Ingredients) - 1)
		{
			if (xNew == 100)
			{
				Score.a =
					Max(0, NewTotal.Capacity) *
					Max(0, NewTotal.Durability) *
					Max(0, NewTotal.Flavor) *
					Max(0, NewTotal.Texture);
				if (NewTotal.Calories == 500)
				{
					Score.b = Score.a;
				}
			}
		}
		else
		{
			Score = CalcMaxScore(IngredientIndex+1, NewTotal, Ingredients, xNew);
		}
		MaxScore = Max(MaxScore, Score);
	}

	return MaxScore;
}

static output
CalcMaxScore(ingredient *Ingredients)
{
	ingredient Current = {};
	output MaxScore = CalcMaxScore(0, Current, Ingredients, 0);
	return MaxScore;
}

static output
Solve(input Input)
{
	ingredient *Ingredients = nullptr;

	const char *Delim = " ,:\n\r";
	char *Token = strtok(Input.Contents, Delim);
	while (Token)
	{
		ingredient Ingredient;
		char *Property = Token;
		Token = strtok(nullptr, Delim);
		Assert(strcmp(Token, "capacity") == 0);
		Ingredient.Capacity = atoi(strtok(nullptr, Delim));

		Token = strtok(nullptr, Delim);
		Assert(strcmp(Token, "durability") == 0);
		Ingredient.Durability = atoi(strtok(nullptr, Delim));

		Token = strtok(nullptr, Delim);
		Assert(strcmp(Token, "flavor") == 0);
		Ingredient.Flavor = atoi(strtok(nullptr, Delim));

		Token = strtok(nullptr, Delim);
		Assert(strcmp(Token, "texture") == 0);
		Ingredient.Texture = atoi(strtok(nullptr, Delim));

		Token = strtok(nullptr, Delim);
		Assert(strcmp(Token, "calories") == 0);
		Ingredient.Calories = atoi(strtok(nullptr, Delim));

		GAPush(Ingredients, Ingredient);

		Token = strtok(nullptr, Delim);
	}

	output Output = CalcMaxScore(Ingredients);
	GAFree(Ingredients);

	return Output;
}

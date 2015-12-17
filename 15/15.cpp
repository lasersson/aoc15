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

DeclareArray(ingredient_array, ingredient);

DeclareArray(int_array, int);

struct score
{
	int Regular;
	int Diet;
};

static score
Max(score a, score b)
{
	a.Regular = Max(a.Regular, b.Regular);
	a.Diet = Max(a.Diet, b.Diet);
	return a;
}

static score
CalcMaxScore(int IngredientIndex, ingredient CurrentTotal, ingredient_array *Ingredients, int xSum)
{
	score MaxScore = {};
	for (int x = 0; x <= (100 - xSum); ++x)
	{
		ingredient NewTotal = Add(Mul(Ingredients->Array[IngredientIndex], x), CurrentTotal);
		score Score = {};
		int xNew = xSum + x;
		if (IngredientIndex == Ingredients->Count-1)
		{
			if (xNew == 100)
			{
				Score.Regular =
					Max(0, NewTotal.Capacity) *
					Max(0, NewTotal.Durability) *
					Max(0, NewTotal.Flavor) *
					Max(0, NewTotal.Texture);
				if (NewTotal.Calories == 500)
				{
					Score.Diet = Score.Regular;
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

static score
CalcMaxScore(ingredient_array *Ingredients)
{
	ingredient Current = {};
	score MaxScore = CalcMaxScore(0, Current, Ingredients, 0);
	return MaxScore;
}

static void
Solve(input_file Input)
{
	ingredient_array Ingredients = {};
	int_array IngredientFactors = {};

	char *Delim = " ,:\n\r";
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

		ArrayAdd(&Ingredients, Ingredient, ingredient);

		int IngredientFactor = Ingredient.Capacity + Ingredient.Durability + Ingredient.Flavor + Ingredient.Texture;
		ArrayAdd(&IngredientFactors, IngredientFactor, int);

		Token = strtok(nullptr, Delim);
	}

	score MaxScore = CalcMaxScore(&Ingredients);
	printf("%d\n", MaxScore.Regular);
	printf("%d\n", MaxScore.Diet);
}

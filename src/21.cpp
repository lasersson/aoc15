#include <aoc.h>

struct actor
{
	int HitPoints;
	int Damage;
	int Armor;
};

struct item
{
	int Cost;
	int Damage;
	int Armor;
};

item Weapons[] = { {8, 4, 0}, {10, 5, 0}, {25, 6, 0}, {40, 7, 0}, {74, 8, 0} };
item Armor[] = { {0, 0, 0}, {13, 0, 1}, {31, 0, 2}, {53, 0, 3}, {75, 0, 4}, {102, 0, 5} };
item Rings[] = { {0, 0, 0}, {25, 1, 0}, {50, 2, 0}, {100, 3, 0}, {20, 0, 1}, {40, 0, 2}, {80, 0, 3} };

static int
PredictWinner(actor Player0, actor Player1)
{
	actor *Attacker = &Player0;
	actor *Defender = &Player1;
	int Winner = 1;
	while (Attacker->HitPoints >= 0 && Defender->HitPoints >= 0)
	{
		int Damage = Max(1, Attacker->Damage - Defender->Armor);
		Defender->HitPoints -= Damage;
		actor *Tmp = Attacker;
		Attacker = Defender;
		Defender = Tmp;
		Winner = 1 - Winner;
	}
	return Winner;
}

static output
Solve(input Input)
{
	actor Boss = {};

	const char *Delim = "\r\n:";
	strtok(Input.Contents, Delim);
	Boss.HitPoints = atoi(strtok(nullptr, Delim));
	strtok(nullptr, Delim);
	Boss.Damage = atoi(strtok(nullptr, Delim));
	strtok(nullptr, Delim);
	Boss.Armor = atoi(strtok(nullptr, Delim));

	int CostMin = 999999;
	int CostMax = 0;
	for (int WeaponIt = 0; WeaponIt < ArrayCount(Weapons); ++WeaponIt)
	{
		item PlayerWeapon = Weapons[WeaponIt];
		for (int ArmorIt = 0; ArmorIt < ArrayCount(Armor); ++ArmorIt)
		{
			item PlayerArmor = Armor[ArmorIt];
			for (int Ring1It = 0; Ring1It < ArrayCount(Rings); ++Ring1It)
			{
				item PlayerRing1 = Rings[Ring1It];
				for (int Ring2It = 0; Ring2It < ArrayCount(Rings); ++Ring2It)
				{
					if (Ring1It != Ring2It || Ring1It == 0)
					{
						item PlayerRing2 = Rings[Ring2It];
						int TotalCost = PlayerWeapon.Cost + PlayerArmor.Cost + PlayerRing1.Cost + PlayerRing2.Cost;
						actor Player;
						Player.HitPoints = 100;
						Player.Damage = PlayerWeapon.Damage + PlayerRing1.Damage + PlayerRing2.Damage;
						Player.Armor = PlayerArmor.Armor + PlayerRing1.Armor + PlayerRing2.Armor;
						int Winner = PredictWinner(Player, Boss);
						if (Winner == 0)
						{
							CostMin = Min(CostMin, TotalCost);
						}
						else
						{
							CostMax = Max(CostMax, TotalCost);
						}
					}
				}
			}
		}
	}
	output Output = { CostMin, CostMax };
	return Output;
}

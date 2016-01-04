#include <aoc.h>

struct actor
{
	int HitPoints;
	int Damage;
	int Armor;
	int Mana;
	int ManaSpent;
};

enum spell
{
	Spell_MagicMissile,
	Spell_Drain,
	Spell_Shield,
	Spell_Poison,
	Spell_Recharge,

	Spell_Count,
};

int SpellCosts[Spell_Count] = { 53, 73, 113, 173, 229 };

enum effect
{
	Effect_Shield,
	Effect_Poison,
	Effect_Recharge,

	Effect_Count
};

enum turn
{
	Turn_Player,
	Turn_Boss,
};

enum difficulty
{
	Difficulty_Normal,
	Difficulty_Hard,

	Difficulty_Count
};

struct game_state
{
	actor Player;
	actor Boss;
	int EffectTimes[Effect_Count];
	difficulty Difficulty;
	turn Turn;
};

static game_state
NextTurn(game_state State)
{
	State.Turn = (turn)(1 - State.Turn);
	return State;
}

static actor
DealDamage(actor Actor, int Damage)
{
	Damage = Max(1, Damage - Actor.Armor);
	Actor.HitPoints = Max(0, Actor.HitPoints - Damage);
	return Actor;
}

static bool
IsAlive(actor Actor)
{
	bool Result = Actor.HitPoints > 0;
	return Result;
}

static game_state
ApplyEffects(game_state State)
{
	if (State.Difficulty == Difficulty_Hard && State.Turn == Turn_Player)
	{
		State.Player = DealDamage(State.Player, 1);
	}
	for (int EffectIt = 0; EffectIt < Effect_Count; ++EffectIt)
	{
		int EffectTime = State.EffectTimes[EffectIt];
		if (EffectTime > 0)
		{
			switch (EffectIt)
			{
				case Effect_Shield:
					State.Player.Armor = 7;
					break;
				case Effect_Poison:
					State.Boss = DealDamage(State.Boss, 3);
					break;
				case Effect_Recharge:
					State.Player.Mana += 101;
					break;
			}
			State.EffectTimes[EffectIt] = EffectTime - 1;
		}
		else
		{
			if (EffectIt == Effect_Shield)
			{
				State.Player.Armor = 0;
			}
		}
	}
	return State;
}

static bool
CanCastSpell(game_state State, spell Spell)
{
	bool Result = false;
	int SpellCost = SpellCosts[Spell];
	if (SpellCost <= State.Player.Mana)
	{
		switch (Spell)
		{
			case Spell_Shield:
				Result = State.EffectTimes[Effect_Shield] == 0;
				break;
			case Spell_Poison:
				Result = State.EffectTimes[Effect_Poison] == 0;
				break;
			case Spell_Recharge:
				Result = State.EffectTimes[Effect_Recharge] == 0;
				break;
			default:
				Result = true;
				break;
		}
	}
	return Result;
}

static game_state
CastSpell(game_state State, spell Spell)
{
	int SpellCost = SpellCosts[Spell];
	Assert(IsAlive(State.Player));
	Assert(State.Player.Mana >= SpellCost);
	State.Player.Mana -= SpellCost;
	State.Player.ManaSpent += SpellCost;
	switch (Spell)
	{
		case Spell_MagicMissile:
			State.Boss = DealDamage(State.Boss, 4);
			break;
		case Spell_Drain:
			State.Boss = DealDamage(State.Boss, 2);
			State.Player.HitPoints += 2;
			break;
		case Spell_Shield:
			Assert(State.EffectTimes[Effect_Shield] == 0);
			State.EffectTimes[Effect_Shield] = 6;
			break;
		case Spell_Poison:
			Assert(State.EffectTimes[Effect_Poison] == 0);
			State.EffectTimes[Effect_Poison] = 6;
			break;
		case Spell_Recharge:
			Assert(State.EffectTimes[Effect_Recharge] == 0);
			State.EffectTimes[Effect_Recharge] = 5;
			break;
		default:
			Assert(!"Invalid spell");
			break;
	}
	return State;
}

static output
Solve(input Input)
{
	game_state InitState = {};
	InitState.Player = { 50, 0, 0, 500, 0 };

	char *Delim = "\r\n:";
	strtok(Input.Contents, Delim);
	InitState.Boss.HitPoints = atoi(strtok(nullptr, Delim));
	strtok(nullptr, Delim);
	InitState.Boss.Damage = atoi(strtok(nullptr, Delim));
	
	output Output = {};
	for (int DifficultyIt = 0; DifficultyIt < Difficulty_Count; ++DifficultyIt)
	{
		game_state PlaythroughState = InitState;
		PlaythroughState.Difficulty = (difficulty)DifficultyIt;
		game_state *StateStack = nullptr;
		GAPush(StateStack, PlaythroughState);
		int ManaSpentMin = 999999;
		int It = 0;
		while (GACount(StateStack))
		{
			game_state State = GATop(StateStack);
			GAPop(StateStack);

			Assert(IsAlive(State.Player));
			Assert(IsAlive(State.Boss));

			State = ApplyEffects(State);
			if (IsAlive(State.Player))
			{
				if (IsAlive(State.Boss))
				{
					Assert(IsAlive(State.Player));
					Assert(IsAlive(State.Boss));

					if (State.Turn == Turn_Player && State.Player.ManaSpent < ManaSpentMin)
					{
						for (int SpellIt = 0; SpellIt < Spell_Count; ++SpellIt)
						{
							if (CanCastSpell(State, (spell)SpellIt))
							{
								game_state NewState = CastSpell(State, (spell)SpellIt);
								if (!IsAlive(NewState.Boss))
								{
									ManaSpentMin = Min(ManaSpentMin, NewState.Player.ManaSpent);
								}
								else
								{
									NewState = NextTurn(NewState);
									GAPush(StateStack, NewState);
								}
							}
						}
					}
					else
					{
						game_state NewState = State;
						NewState.Player = DealDamage(NewState.Player, NewState.Boss.Damage);
						if (IsAlive(NewState.Player))
						{
							NewState = NextTurn(NewState);
							GAPush(StateStack, NewState);
						}
					}
				}
				else
				{
					ManaSpentMin = Min(ManaSpentMin, State.Player.ManaSpent);
				}
			}

			++It;
		}
		Output.v[DifficultyIt] = ManaSpentMin;
	}
	return Output;
}

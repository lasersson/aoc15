#include <aoc.h>

union atom
{
	int Id;
	char Name[4];
};

struct replacement
{
	atom *Src;
	atom *Dst;
};

static bool
Equal(atom *a, atom *b)
{
	bool Result = true;
	if (GACount(a) == GACount(b))
	{
		for (int It = 0; It < GACount(a); ++It)
		{
			if (a[It].Id != b[It].Id)
			{
				Result = false;
				break;
			}
		}
	}
	else
	{
		Result = false;
	}
	return Result;
}


static atom **
Replace(atom *Molecule, atom *Src, atom *Dst, atom **Results)
{
	for (int MolIt = 0; MolIt < GACount(Molecule) ; ++MolIt)
	{
		atom *Mol = Molecule + MolIt;
		bool IsMatch = true;
		for (int CmpIt = 0; CmpIt < GACount(Src); ++CmpIt)
		{
			if (Src[CmpIt].Id != Mol[CmpIt].Id)
			{
				IsMatch = false;
				break;
			}
		}

		if (IsMatch)
		{
			atom *Result = nullptr;
			GAInit(Result, GACount(Molecule) - GACount(Src) + GACount(Dst));
			for (int It = 0; It < MolIt; ++It)
			{
				GAPush(Result, Molecule[It]);
			}
			for (int It = 0; It < GACount(Dst); ++It)
			{
				GAPush(Result, Dst[It]);
			}
			for (int It = MolIt + GACount(Src); It < GACount(Molecule); ++It)
			{
				GAPush(Result, Molecule[It]);
			}

			bool ShouldPush = true;
			for (int ResIt = 0; ResIt < GACount(Results); ++ResIt)
			{
				if (Equal(Result, Results[ResIt]))
				{
					ShouldPush = false;
					break;
				}
			}
			if (ShouldPush)
			{
				GAPush(Results, Result);
			}
			else
			{
				GAFree(Result);
			}
		}
	}
	return Results;
}

static int
ParseAtom(char *Str, atom *OutAtom)
{
	int Advance = 1;
	OutAtom->Name[0] = Str[0];
	if (!(Str[1] >= 'A' && Str[1] <= 'Z'))
	{
		OutAtom->Name[1] = Str[1];
		Advance = 2;
	}
	return Advance;
}

static atom *
ParseMolecule(char *Str)
{
	atom *Molecule = nullptr;
	int StrLen = (int)strlen(Str);
	for (int It = 0; It < StrLen;)
	{
		atom Atom;
		Atom.Id = 0;
		It += ParseAtom(Str + It, &Atom);
		GAPush(Molecule, Atom);
	}
	return Molecule;
}

static int
CompareReplacements(const void *a, const void *b)
{
	const replacement *ReplA = (const replacement *)a;
	const replacement *ReplB = (const replacement *)b;
	int Result = GACount(ReplA->Dst) - GACount(ReplB->Dst);;
	return Result;
}

static output
Solve(input Input)
{
	atom *MedicineMolecule = nullptr;
	replacement *Replacements = nullptr;
	const char *Delim = "\r\n =>";
	char *Token = strtok(Input.Contents, Delim);
	while (Token)
	{
		replacement Replacement = {};
		Replacement.Src = ParseMolecule(Token);
		Token = strtok(nullptr, Delim);
		Replacement.Dst = ParseMolecule(Token);
		GAPush(Replacements, Replacement);

		Token = strtok(nullptr, Delim);
		int Len = (int)strlen(Token);
		if (Len > 2)
		{
			MedicineMolecule = ParseMolecule(Token);
			Token = strtok(nullptr, Delim);
		}
	}

	qsort(Replacements, GACount(Replacements), sizeof(replacement), CompareReplacements);

	output Output = {};
	atom **ResultSet = nullptr;
	for (int ReplIt = 0; ReplIt < GACount(Replacements); ++ReplIt)
	{
		replacement Repl = Replacements[ReplIt];
		ResultSet = Replace(MedicineMolecule, Repl.Src, Repl.Dst, ResultSet);
	}

	Output.a = GACount(ResultSet);
	for (int ResIt = 0; ResIt < GACount(ResultSet); ++ResIt)
	{
		GAFree(ResultSet[ResIt]);
	}
	GAClear(ResultSet);

	struct replace_step
	{
		atom *Molecule;
		int Count;
	};

	atom **TestedMolecules = nullptr;
	replace_step *StepStack = nullptr;
	replace_step InitStep = { MedicineMolecule, 0 };
	GAPush(StepStack, InitStep);
	GAPush(TestedMolecules, MedicineMolecule);
	int StepCount = 0;
	while (GACount(StepStack))
	{
		replace_step Step = GATop(StepStack);
		GAPop(StepStack);

		if (StepCount == 0)
		{
			for (int ReplIt = 0; ReplIt < GACount(Replacements); ++ReplIt)
			{
				replacement Repl = Replacements[ReplIt];
				ResultSet = Replace(Step.Molecule, Repl.Dst, Repl.Src, ResultSet);
			}

			for (int ResIt = 0; ResIt < GACount(ResultSet); ++ResIt)
			{
				atom *Res = ResultSet[ResIt];
				if (GACount(Res) == 1 && Res[0].Id == 'e')
				{
					StepCount = Step.Count + 1;
					GAFree(Res);
				}
				else if (StepCount == 0)
				{
					bool AlreadyTested = false;
					for (int TestIt = 0; TestIt < GACount(TestedMolecules); ++TestIt)
					{
						if (Equal(TestedMolecules[TestIt], Res))
						{
							AlreadyTested = true;
							break;
						}
					}

					if (!AlreadyTested)
					{
						replace_step NewStep = { Res, Step.Count + 1 };
						GAPush(StepStack, NewStep);
						GAPush(TestedMolecules, Res);
					}
					else
					{
						GAFree(Res);
					}
				}
				else
				{
					GAFree(Res);
				}
			}
		}

		GAClear(ResultSet);
	}

	Output.b = StepCount;

	for (int It = 0; It < GACount(TestedMolecules); ++It)
	{
		GAFree(TestedMolecules[It]);
	}
	GAFree(TestedMolecules);

	GAFree(ResultSet);
	GAFree(StepStack);
	for (int ReplIt = 0; ReplIt < GACount(Replacements); ++ReplIt)
	{
		GAFree(Replacements[ReplIt].Src);
		GAFree(Replacements[ReplIt].Dst);
	}
	GAFree(Replacements);

	return Output;
}

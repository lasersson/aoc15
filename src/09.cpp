#include <aoc.h>

typedef u16 node;

static node
MakeNodeId(const char *Str)
{
	node Node = Fletcher16(Str, (u32)strlen(Str));
	return Node;
}

static node
HasNode(node *Nodes, node Node)
{
	bool Exists = false;
	for (int It = 0; It < GACount(Nodes); ++It)
	{
		if (Nodes[It] == Node)
		{
			Exists = true;
			break;
		}
	}
	return Exists;
}

static node *
AddNodeToSet(node *Nodes, node Node)
{
	if (!Nodes || !HasNode(Nodes, Node))
	{
		GAPush(Nodes, Node);
	}
	return Nodes;
}

struct edge
{
	node Node1;
	node Node2;
	int Dist;
};

struct path_result
{
	int Shortest;
	int Longest;
};

static path_result
PathMinMax(path_result A, path_result B)
{
	A.Shortest = Min(A.Shortest, B.Shortest);
	A.Longest = Max(A.Longest, B.Longest);
	return A;
}

static path_result
FindPaths(node Node, edge *Edges, node *Visited)
{
	Assert(GACount(Visited) < GACapacity(Visited));
	GAPush(Visited, Node);
	path_result Result = { 9999999, 0 };
	for (int It = 0; It < GACount(Edges); ++It)
	{
		edge *Edge = Edges + It;
		if (Edge->Node1 == Node || Edge->Node2 == Node)
		{
			node OtherNode = Edge->Node1 == Node ? Edge->Node2 : Edge->Node1;
			if (!HasNode(Visited, OtherNode))
			{
				path_result EdgeRes = FindPaths(OtherNode, Edges, Visited);
				EdgeRes.Shortest += Edge->Dist;
				EdgeRes.Longest += Edge->Dist;
				Result = PathMinMax(Result, EdgeRes);
			}
		}
	}
	GAPop(Visited);

	if (Result.Shortest == 9999999 && Result.Longest == 0)
	{
		Result = {};
	}

	return Result;
}

static output
Solve(input Input)
{
	node *AllNodes = nullptr;
	edge *Edges = nullptr;

	const char *Delim = "\n ";
	char *Token = strtok(Input.Contents, Delim);
	while (Token)
	{
		node Node1 = MakeNodeId(Token);
		AllNodes = AddNodeToSet(AllNodes, Node1);
		Token = strtok(nullptr, Delim);
		Assert(strcmp(Token, "to") == 0);
		Token = strtok(nullptr, Delim);
		node Node2 = MakeNodeId(Token);
		AllNodes = AddNodeToSet(AllNodes, Node2);
		Token = strtok(nullptr, Delim);
		Assert(Token[0] == '=');
		Token = strtok(nullptr, Delim);
		int Dist = atoi(Token);

		edge Edge = { Node1, Node2, Dist };
		GAPush(Edges, Edge);

		Token = strtok(nullptr, Delim);
	}

	node Source = MakeNodeId("source");
	for (int It = 0; It < GACount(AllNodes); ++It)
	{
		edge Edge = { Source, AllNodes[It], 0 };
		GAPush(Edges, Edge);
	}
	AllNodes = AddNodeToSet(AllNodes, Source);

	node *Visited = nullptr;
	Visited = GAInit(Visited, GACount(AllNodes));
	path_result Result = FindPaths(Source, Edges, Visited);

	GAFree(AllNodes);
	GAFree(Edges);
	GAFree(Visited);

	output Output = { Result.Shortest, Result.Longest };
	return Output;
}

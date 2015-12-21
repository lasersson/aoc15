#include <aoc.h>
#include <stdint.h>

typedef uint16_t vertex_id;

static vertex_id
MakeVertexId(char *Str)
{
	int Len = (int)strlen(Str);
	vertex_id Sum1 = 0;
	vertex_id Sum2 = 0;
	for (int It = 0; It < Len; ++It)
	{
		Sum1 = (Sum1 + Str[It]) % 255;
		Sum2 = (Sum2 + Sum1) % 255;
	}
	
	vertex_id VertexId = (Sum2 << 8) | (Sum1);
	return VertexId;
}

static int
FindVertexIndex(vertex_id *Vertices, vertex_id Vertex)
{
	int Result = -1;
	if (Vertices)
	{
		for (int It = 0; It < GACount(Vertices); ++It)
		{
			if (Vertices[It] == Vertex)
			{
				Result = It;
				break;
			}
		}
	}

	return Result;
}

static vertex_id *
AddVertexToSet(vertex_id *Vertices, char *VertexName, int *OutVertexIndex)
{
	vertex_id VertexId = MakeVertexId(VertexName);
	int VertexIndex = FindVertexIndex(Vertices, VertexId);
	if (VertexIndex < 0)
	{
		GAPush(Vertices, VertexId);
		VertexIndex = GACount(Vertices) - 1;
	}
	*OutVertexIndex = VertexIndex;
	return Vertices;
}

struct edge
{
	int Src;
	int Dst;
	int Weight;
};

static edge
FindEdge(edge *Edges, int Src, int Dst)
{
	edge Result = { -1, -1, 0 };
	for (int EdgeIt = 0; EdgeIt < GACount(Edges); ++EdgeIt)
	{
		edge Edge = Edges[EdgeIt];
		if (Edge.Src == Src && Edge.Dst == Dst)
		{
			Result = Edge;
			break;
		}
	}

	return Result;
}

static int
FindMaxHappiness(int VertexIndex, vertex_id *Vertices, edge *Edges, vertex_id *Visited)
{
	int MaxHappiness = -999999;
	GAPush(Visited, Vertices[VertexIndex]);

	if (GACount(Visited) < GACount(Vertices))
	{
		for (int EdgeIt = 0; EdgeIt < GACount(Edges); ++EdgeIt)
		{
			edge Edge = Edges[EdgeIt];
			if (Edge.Src == VertexIndex)
			{
				if (FindVertexIndex(Visited, Vertices[Edge.Dst]) < 0)
				{
					edge OtherEdge = FindEdge(Edges, Edge.Dst, Edge.Src);
					int Happiness = FindMaxHappiness(Edge.Dst, Vertices, Edges, Visited) + Edge.Weight + OtherEdge.Weight;
					MaxHappiness = Max(MaxHappiness, Happiness);
				}
			}
		}
	}
	else
	{
		int FirstVertex = FindVertexIndex(Vertices, Visited[0]);
		edge Edge0 = FindEdge(Edges, VertexIndex, FirstVertex);
		edge Edge1 = FindEdge(Edges, FirstVertex, VertexIndex);
		MaxHappiness = Edge0.Weight + Edge1.Weight;
	}
	GAPop(Visited);
	return MaxHappiness;
}

static void Solve(input_file Input)
{
	vertex_id *Vertices = nullptr;
	edge *Edges = nullptr;

	char *Delim = " \r\n.";
	char *Token = strtok(Input.Contents, Delim);
	while (Token)
	{
		int Src;
		Vertices = AddVertexToSet(Vertices, Token, &Src);
		Token = strtok(nullptr, Delim); // would
		Token = strtok(nullptr, Delim); // gain/lose
		int Sign = 0;
		if (strcmp(Token, "lose") == 0)
		{
			Sign = -1;
		}
		else if (strcmp(Token, "gain") == 0)
		{
			Sign = 1;
		}
		Assert(Sign != 0);
		Token = strtok(nullptr, Delim); // n
		int Weight = atoi(Token) * Sign;
		Token = strtok(nullptr, Delim); // happiness
		Token = strtok(nullptr, Delim); // units
		Token = strtok(nullptr, Delim); // by
		Token = strtok(nullptr, Delim); // sitting
		Token = strtok(nullptr, Delim); // next
		Token = strtok(nullptr, Delim); // to
		Token = strtok(nullptr, Delim); // V2

		int Dst;
		Vertices = AddVertexToSet(Vertices, Token, &Dst);
		edge Edge = { Src, Dst, Weight };
		GAPush(Edges, Edge);

		Token = strtok(nullptr, Delim); // V1
	}

	vertex_id *Visited = nullptr;
	Visited = GAInit(Visited, GACapacity(Vertices) + 1);

	int MaxHappiness = FindMaxHappiness(0, Vertices, Edges, Visited);
	printf("%d\n", MaxHappiness);

	int Me;
	Vertices = AddVertexToSet(Vertices, "Me", &Me);
	for (int VertexIt = 0; VertexIt < GACount(Vertices); ++VertexIt)
	{
		if (VertexIt != Me)
		{
			edge Edge0 = { VertexIt, Me, 0 };
			GAPush(Edges, Edge0);
			edge Edge1 = { Me, VertexIt, 0 };
			GAPush(Edges, Edge1);
		}
	}

	MaxHappiness = FindMaxHappiness(0, Vertices, Edges, Visited);
	printf("%d\n", MaxHappiness);

	GAFree(Vertices);
	GAFree(Visited);
	GAFree(Edges);
}

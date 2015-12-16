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

DeclareArray(vertex_array, vertex_id);

static int
FindVertexIndex(vertex_array *Vertices, vertex_id Vertex)
{
	int Result = -1;
	for (int It = 0; It < Vertices->Count; ++It)
	{
		if (Vertices->Array[It] == Vertex)
		{
			Result = It;
			break;
		}
	}

	return Result;
}

static int
AddVertexToSet(vertex_array *Vertices, char *VertexName)
{
	vertex_id VertexId = MakeVertexId(VertexName);
	int VertexIndex = FindVertexIndex(Vertices, VertexId);
	if (VertexIndex < 0)
	{
		ArrayAdd(Vertices, VertexId, vertex_id);
		VertexIndex = Vertices->Count - 1;
	}
	return VertexIndex;
}

struct edge
{
	int Src;
	int Dst;
	int Weight;
};

DeclareArray(edge_array, edge);

static edge
FindEdge(edge_array *Edges, int Src, int Dst)
{
	edge Result = { -1, -1, 0 };
	for (int EdgeIt = 0; EdgeIt < Edges->Count; ++EdgeIt)
	{
		edge Edge = Edges->Array[EdgeIt];
		if (Edge.Src == Src && Edge.Dst == Dst)
		{
			Result = Edge;
			break;
		}
	}

	return Result;
}

static int
FindMaxHappiness(int VertexIndex, vertex_array *Vertices, edge_array *Edges, vertex_array *Visited)
{
	int MaxHappiness = -999999;
	ArrayAdd(Visited, Vertices->Array[VertexIndex], vertex_id);

	if (Visited->Count < Vertices->Count)
	{
		for (int EdgeIt = 0; EdgeIt < Edges->Count; ++EdgeIt)
		{
			edge Edge = Edges->Array[EdgeIt];
			if (Edge.Src == VertexIndex)
			{
				if (FindVertexIndex(Visited, Vertices->Array[Edge.Dst]) < 0)
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
		int FirstVertex = FindVertexIndex(Vertices, Visited->Array[0]);
		edge Edge0 = FindEdge(Edges, VertexIndex, FirstVertex);
		edge Edge1 = FindEdge(Edges, FirstVertex, VertexIndex);
		MaxHappiness = Edge0.Weight + Edge1.Weight;
	}
	ArrayPop(Visited);
	return MaxHappiness;
}

static void Solve(input_file Input)
{
	vertex_array Vertices = {};
	edge_array Edges = {};

	char *Delim = " \r\n.";
	char *Token = strtok(Input.Contents, Delim);
	while (Token)
	{
		int Src = AddVertexToSet(&Vertices, Token);
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

		int Dst = AddVertexToSet(&Vertices, Token);
		edge Edge = { Src, Dst, Weight };
		ArrayAdd(&Edges, Edge, edge);

		Token = strtok(nullptr, Delim); // V1
	}

	vertex_array Visited = {};
	Visited.Cap = Vertices.Cap + 1;
	Visited.Array = (vertex_id *)malloc(sizeof(vertex_id) * Visited.Cap);

	int MaxHappiness = FindMaxHappiness(0, &Vertices, &Edges, &Visited);
	printf("%d\n", MaxHappiness);


	int Me = AddVertexToSet(&Vertices, "Me");
	for (int VertexIt = 0; VertexIt < Vertices.Count; ++VertexIt)
	{
		if (VertexIt != Me)
		{
			edge Edge0 = { VertexIt, Me, 0 };
			ArrayAdd(&Edges, Edge0, edge);
			edge Edge1 = { Me, VertexIt, 0 };
			ArrayAdd(&Edges, Edge1, edge);
		}
	}

	MaxHappiness = FindMaxHappiness(0, &Vertices, &Edges, &Visited);
	printf("%d\n", MaxHappiness);

	free(Vertices.Array);
	free(Visited.Array);
	free(Edges.Array);
}

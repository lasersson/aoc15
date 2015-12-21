#include <aoc.h>
#include <stdint.h>

typedef uint32_t u32;
typedef uint64_t u64;

static u32 Shift[64] = {
	7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,
	5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,
	4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,
	6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21 };

static u32 K[64] =
{
	0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
	0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
	0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
	0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
	0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
	0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
	0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
	0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
	0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
	0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
	0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
	0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
	0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
	0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
	0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
	0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391,
};

inline u32
RotateLeft(u32 x, u32 s)
{
	u32 Result = (x << s) | (x >> (32 - s));
	return Result;
}

union digest
{
	struct
	{
		u32 A, B, C, D;
	};
	unsigned char b[16];
	u32 w[4];
};

static void
Solve(input_file Input)
{
	char *Key = strtok(Input.Contents, "\n\r");

	bool FoundA = false;
	bool FoundB = false;
	for (u32 Number = 0; Number >= 0; ++Number)
	{
		unsigned char Msg[64];
		u32 *Msg4 = (u32 *)Msg;
		int MsgByteCount = sprintf((char *)Msg, "%s%u", Key, Number);
		if (MsgByteCount > 55)
		{
			printf("Key too long\n");
			break;
		}

		Msg[MsgByteCount] = 0x80;
		for (u32 It = MsgByteCount + 1; It < 56; ++It)
		{
			Msg[It] = 0;
		}
		*((u64 *)(Msg + 56)) = MsgByteCount * 8;

		digest Digest;
		Digest.w[0] = 0x67452301;
		Digest.w[1] = 0xefcdab89;
		Digest.w[2] = 0x98badcfe;
		Digest.w[3] = 0x10325476;

		digest Cur = Digest;
		for (u32 It = 0; It < 64; ++It)
		{
			u32 F;
			u32 g;
			if (It < 16)
			{
				F = (Cur.B & Cur.C) | ((~Cur.B) & Cur.D);
				g = It;
			}	
			else if (It < 32)
			{
				F = (Cur.D & Cur.B) | ((~Cur.D) & Cur.C);
				g = (5 * It + 1) % 16;
			}
			else if (It < 48)
			{
				F = Cur.B ^ Cur.C ^ Cur.D;
				g = (3 * It + 5) % 16;
			}
			else
			{
				F = Cur.C ^ (Cur.B | (~Cur.D));
				g = (7 * It) % 16;
			}

			u32 NewA = Cur.D;
			Cur.D = Cur.C;
			Cur.C = Cur.B;
			Cur.B = Cur.B + RotateLeft((Cur.A + F + K[It] + Msg4[g]), Shift[It]);
			Cur.A = NewA;

		}

		Digest.A += Cur.A;
		Digest.B += Cur.B;
		Digest.C += Cur.C;
		Digest.D += Cur.D;

		if (Digest.b[0] == 0 && Digest.b[1] == 0 && (Digest.b[2] & 0xf0) == 0)
		{
			if (!FoundA)
			{
				printf("%d\n", Number);
				FoundA = true;
			}
			if (Digest.b[2] == 0 && !FoundB)
			{
				printf("%d\n", Number);
				FoundB = true;
				break;
			}
		}
	}

}

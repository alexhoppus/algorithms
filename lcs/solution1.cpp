#include <iostream>
#include <cstdio>
#include <algorithm>
#include <string.h>
#include <map>
#include <string>

using namespace std;
#define N 1024
char str1[N];
char str2[N];

/*2D case*/
int hash_tbl[N][N];

void remember(int res, char *end1, char *end2)
{
	hash_tbl[end1-str1][end2-str2] = res;
}

int query(char *end1, char *end2)
{
	return hash_tbl[end1-str1][end2-str2];
}

int lcs(char *end1, char *end2)
{
	int res = 0;
	if ((end1 - str1 < 0) || (end2 - str2 < 0))
		return 0;
	res = query(end1, end2);
	if (res == -1) {
		if (*end1 == *end2)
			res = 1 + lcs(end1 - 1, end2 - 1);
		else
			res = max(lcs(end1 - 1, end2), lcs(end1, end2 - 1));
		remember(res, end1, end2);
	}
	return res;
}

int main()
{
	int ret = 0;
	int len1, len2, n_lcs;
	while(gets(str1) && gets(str2)) {
        	len1 = strlen(str1);
        	len2 = strlen(str2);
		for (int i = 0; i < N; i++)
			for (int j = 0; j < N; j++)
				hash_tbl[i][j] = -1;
		n_lcs = lcs(&str1[len1 - 1], &str2[len2 - 1]);
		printf("%d\n", n_lcs);
	}
}

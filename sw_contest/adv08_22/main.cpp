#include <iostream>

#define N_ADVS 3
#define MAX_VISITORS 50
#define INT_MAX 
int AV[MAX_VISITORS];
int DV[MAX_VISITORS];

int T, N;
int LA[N_ADVS];
int PA[N_ADVS];
int AA[N_ADVS];

using namespace std;

static bool inside_range(int a1, int a2, int v1, int v2)
{
	return (a1 >= v1) && (a2 <= v2);
}

static inline int max(int a, int b)
{
	return a > b ? a : b;
}

static bool is_intersect(int x1, int x2, int x3, int x4)
{
	return (x3 < x2) && (x4 > x1);
}

int count_points()
{
	int points = 0;
	for (int i = 0; i < N; i++) {
		int p[N_ADVS];
		for (int p_i = 0; p_i < N_ADVS; p_i++) {
			p[p_i] = inside_range(AA[p_i], AA[p_i] + LA[p_i], AV[i], AV[i] + DV[i]) ? PA[p_i] : 0;
		}
		points += max(p[0], max(p[1], p[2]));
	}
	return points;
}

int solve_testcase()
{
	int max_points = 0;
	for (int aa_1 = 1; aa_1 <= 50; aa_1++) {
		AA[0] = aa_1;
		for (int aa_2 = 1; aa_2 <= 50; aa_2++) {
			AA[1] = aa_2;
			if (is_intersect(AA[0], AA[0] + LA[0], AA[1], AA[1] + LA[1]))
				continue;
			for (int aa_3 = 1; aa_3 <= 50; aa_3++) {
				int current_points = 0;
				AA[2] = aa_3;
				if (is_intersect(AA[0], AA[0] + LA[0], AA[2], AA[2] + LA[2]) ||
					is_intersect(AA[1], AA[1] + LA[1], AA[2], AA[2] + LA[2]))
					continue;
				current_points = count_points();
				max_points = max(current_points, max_points);
			}
		}
	}
	return max_points;
}

int main()
{
	cin >> T;
	for (int i_t = 0;  i_t < T; i_t++) {
		int max_points = 0;
		cin >> N >> LA[0] >> LA[1] >> LA[2] >> PA[0] >> PA[1] >> PA[2];
		for (int i_n = 0; i_n < N; i_n++) {
			cin >> AV[i_n];
			cin >> DV[i_n];
		}
		max_points = solve_testcase();
		cout << "#" << i_t + 1 << " " << max_points << endl;
	}
	return 0;
}

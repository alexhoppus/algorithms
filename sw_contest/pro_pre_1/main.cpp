#include <iostream>
#include <stdlib.h>
#include <string.h>

#define MAX_N 2000

using namespace std;
struct edje {
	short v1;
	short v2;
	short cost;
	void set(short vtx1, short vtx2, short c)
	{
		v1 = vtx1;
		v2 = vtx2;
		cost = c;
	}
};

struct node {
	short adj[MAX_N];
	short adj_cost[MAX_N];
	short n_adj;

	void add_neighbour(short v, short cost) {
		adj_cost[n_adj] = cost;
		adj[n_adj++] = v;
	}

	bool is_leaf() {
		return n_adj ? true : false;
	}
};

struct u_tree {
	struct node nodes[MAX_N];

	struct edje e[MAX_N];
	short n_edjes;
	struct edje forbidden_edje;

	u_tree() {
		memset(nodes, 0x0, MAX_N * sizeof(struct node));
	};

	void add_edje(short v1, short v2, short cost) {
		nodes[v1].add_neighbour(v2, cost);
		nodes[v2].add_neighbour(v1, cost);
		e[n_edjes++].set(v1, v2, cost);
	}

	void set_forbidden_edje(struct edje e) {
		forbidden_edje = e;
	}
};

int solve_testcase()
{
	int max_points = 0;
	return max_points;
}

u_tree t;

int main()
{
	int T, N;
	cin >> T;
	for (int i_t = 0;  i_t < T; i_t++) {
		int max_points;
		cin >> N;
		for (int i_n = 0; i_n < N - 1; i_n++) {
			short v1, v2, cost;
			cin >> cost >> v1 >> v2;
			t.add_edje(v1, v2, cost);
		}

		for (int i_n = 0; i_n < N - 1; i_n++) {
			t.set_forbidden_edje(t.e[i_n]);
		}
		max_points = solve_testcase();
		cout << "#" << i_t + 1 << " " << max_points << endl;
	}
	return 0;
}

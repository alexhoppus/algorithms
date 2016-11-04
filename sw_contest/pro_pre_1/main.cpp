#include <iostream>

#define MAX_N 2001
//#define DEBUG 1
using namespace std;

int adj_list[MAX_N][MAX_N];
int adj_list_cost[MAX_N][MAX_N];
int n_adj_list[MAX_N];
struct edje {
	int v1;
	int v2;
	int cost;
};

static inline int edge_cost(struct edje *e)
{
	return e->cost;
}

int visited[MAX_N];

static inline int get_cost(int v1, int v2)
{
	return adj_list_cost[v1][v2];
}

static inline void set_visited(int v1, int dist)
{
	visited[v1] = dist;
}

static inline bool is_visited(int v1)
{
	return visited[v1] == -1 ? false : true;
}
struct edje edjes[MAX_N];
static int n_edjes = 0;

struct edje forbidden_edje;

void set_forbidden_edje(struct edje e)
{
	forbidden_edje.v1 = e.v1;
	forbidden_edje.v2 = e.v2;
}

static inline bool is_edje_forbidden(int v1, int v2)
{
	if ((forbidden_edje.v1 == v1 && forbidden_edje.v2 == v2)
		|| (forbidden_edje.v1 == v2 && forbidden_edje.v2 == v1))
		return true;
	return false;
}

void put_to_edje_arr(int v1, int v2, int cost)
{
	edjes[n_edjes].v1 = v1;
	edjes[n_edjes].v2 = v2;
	edjes[n_edjes].cost = cost;
	n_edjes++;
}

void memset(void *ptr, int val, unsigned long nr)
{
	for (unsigned int i = 0; i < nr; i++)
		*((int *) ptr + i) = val;
}

void _add_edje(int v1, int v2, int cost)
{
	adj_list[v1][n_adj_list[v1]] = v2;
	adj_list_cost[v1][v2] = cost;
	adj_list_cost[v2][v1] = cost;
	n_adj_list[v1]++;
}

void add_edje(int v1, int v2, int cost)
{
	_add_edje(v1, v2, cost);
	_add_edje(v2, v1, cost);
	put_to_edje_arr(v1, v2, cost);
}

#define N_MAX_QUEUE 5*1024
namespace queue {
	int _store[N_MAX_QUEUE];
	int qbegin, qend;
	static inline void push(int e) {
		_store[qend] = e;
		qend++;
	}

	static inline int pop() {
		int elem = _store[qbegin];
		qbegin++;
		return elem;
	}

	static inline int size() {
		return (int) (qend - qbegin);
	}

	static inline void flush() {
		qbegin = 0;
		qend = 0;
	}
};

int bfs(int vertex)
{
	int max_v = vertex;
	memset(visited, -1, MAX_N);

	queue::push(vertex);
	set_visited(vertex, 0);

	while(queue::size()) {
		int v = queue::pop();
#if DEBUG
	cout << "BFS pop vertex " << *v << endl;
#endif
		if (visited[max_v] < visited[v]) {
#if DEBUG
			cout << "max_v set to " << v << endl;
#endif
			max_v = v;
		}
		for (int i = 0; i < n_adj_list[v]; i++) {
			int neighbour = adj_list[v][i];
			if (!is_visited(neighbour) && !is_edje_forbidden(v, neighbour)) {
#if DEBUG
				cout << "neighbour " << neighbour << endl;
				cout << "visited["<< neighbour << "] = " << visited[v] + get_cost(neighbour, v) << endl;
#endif
				queue::push(neighbour);
				set_visited(neighbour, visited[v] + get_cost(neighbour, v));
			}
		}
	}
	return max_v;
}

int solve_for_vertex(int v)
{
	queue::flush();
	int new_v = bfs(v);
	int max_v = bfs(new_v);
	return visited[max_v];
}

int solve_testcase()
{
	int max_dist = 0;
#if DEBUG
	cout << "nedjes " << n_edjes << endl;
#endif
	for (int i = 0; i < n_edjes; i++) {
		int cur_max_dist = 0;
		struct edje e = edjes[i];
		set_forbidden_edje(e);
#if DEBUG
		cout << "removing edge " << e.v1 << " " << e.v2 << endl;
#endif
		int d1 = solve_for_vertex(e.v1);
		int d2 = solve_for_vertex(e.v2);
#if DEBUG
		cout << "d1 " << d1 << " d2 " << d2 << endl;
#endif
		cur_max_dist = d1 + d2 + edge_cost(&e);
		if (max_dist < cur_max_dist)
			max_dist = cur_max_dist;
	}
	return max_dist;
}

int main()
{
	int T, N;
	std::ios::sync_with_stdio(false);
	cin >> T;
	for (int i_t = 0;  i_t < T; i_t++) {
		int max_dist;
		memset(n_adj_list, 0x0, MAX_N);
		n_edjes = 0;
		queue::flush();
		cin >> N;
		for (int i_n = 0; i_n < N - 1; i_n++) {
			short v1, v2, cost;
			cin >> cost >> v1 >> v2;
			add_edje(v1, v2, cost);
		}

		max_dist = solve_testcase();
		cout << "#" << i_t + 1 << " " << max_dist << endl;
	}
	return 0;
}

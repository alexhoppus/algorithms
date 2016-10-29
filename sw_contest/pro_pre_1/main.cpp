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

static inline void set_visited(int v1)
{
	visited[v1] = 1;
}

static inline int is_visited(int v1)
{
	return visited[v1];
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

#define N_MAX_QUEUE 2*1024*1024
namespace queue {
	void *_store[N_MAX_QUEUE];
	void **qbegin;
	void **qend;
	static inline void push(void* e) {
		*qend = e;
		qend++;
	}

	static inline void *pop() {
		void *elem = *qbegin;
		qbegin++;
		return elem;
	}

	static inline int size() {
		return (int) (qend - qbegin);
	}
	static inline int vsize() {
		return (int) (qend - &_store[0]);
	}

	static inline void flush() {
		qbegin = _store;
		qend =_store;
	}
};

struct vertex_dist {
	int v;
	int d;
};

#define MAX_VD_CACHE 2*1024*1024
struct vertex_dist vd[MAX_VD_CACHE];
static int n_vd = 0;

struct vertex_dist *alloc_vd()
{
	return &vd[n_vd++];
}

struct vertex_dist *bfs(struct vertex_dist *vd)
{
	int max_d = 0;
	struct vertex_dist *max_vd = vd;
	memset(visited, 0x0, MAX_N);

	queue::push(vd);
	set_visited(vd->v);
#if DEBUG
	cout << "BFS started from vertex " << vd->v << endl;
#endif

	while(queue::size()) {
		struct vertex_dist *vd = (struct vertex_dist *)queue::pop();
		if (vd->d > max_vd->d) {
#if DEBUG
			cout << "max_vd set to " << vd->d << endl;
#endif
			max_vd = vd;
		}
		int *v = &vd->v;
#if DEBUG
		cout << "qsize " << queue::size() <<  " v " << vd->v << " dist " << vd->d << " n_adj_list[*v] " << n_adj_list[*v] << endl;
#endif
		for (int i = 0; i < n_adj_list[*v]; i++) {
			int *neighbour = &adj_list[*v][i];
			if (!is_edje_forbidden(*v, *neighbour) && !is_visited(*neighbour)) {
				struct vertex_dist *neighbour_vd = alloc_vd();
				neighbour_vd->v = *neighbour;
				neighbour_vd->d = vd->d + get_cost(*neighbour, *v);
				queue::push(neighbour_vd);
				set_visited(*neighbour);
			}
		}
	}
	return max_vd;
}

int solve_for_vertex(int v)
{
	n_vd = 0;
	queue::flush();

	struct vertex_dist *vd = alloc_vd();
	vd->v = v;
	vd->d = 0;
	struct vertex_dist *new_vd = bfs(vd);
	new_vd->d = 0;
	struct vertex_dist *max_vd = bfs(new_vd);
	return max_vd->d;
}

int solve_testcase()
{
	int max_dist = solve_for_vertex(1);
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
	cin >> T;
	for (int i_t = 0;  i_t < T; i_t++) {
		int max_dist;
		memset(adj_list, 0x0, MAX_N * MAX_N);
		memset(adj_list_cost, 0x0, MAX_N * MAX_N);
		memset(n_adj_list, 0x0, MAX_N);
		memset(edjes, 0x0, MAX_N);
		n_edjes = 0;
		n_vd = 0;
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

#include <iostream>

int T;
#define MAX_N 20
#define MAX_ELEM 4
int N, C;
int c_w[MAX_ELEM];
int c_h[MAX_ELEM];
int c_dist[MAX_ELEM];

int map[MAX_N][MAX_N] = {0};
int visited[MAX_N][MAX_N] = {0};

#define NO_ELEMENTS_HERE -1

using namespace std;

void memset(void *ptr, int val, unsigned long nr)
{
	for (unsigned int i = 0; i < nr; i++)
		*((int *) ptr + i) = val;
}

static inline void mark_visited(int w, int h)
{
	visited[h][w] = 1;
}

static inline bool is_visited(int w, int h)
{
	return visited[h][w];
}

static inline bool is_road(int w, int h)
{
	return map[h][w];
}

static inline bool in_bounds(int w, int h)
{
	return (w < N) && (w > -1) && (h > -1) && (h < N);
}

static inline bool can_move_here(int w, int h)
{
	return in_bounds(w, h) && is_road(w, h) && (!is_visited(w, h));
}

static inline int min(int a, int b)
{
	return a < b ? a : b;
}

static inline int max(int a, int b)
{
	return a > b ? a : b;
}

static inline int element_located_here(int w, int h)
{
	for (int i = 0; i < C; i++) {
		if ((w == (c_w[i] - 1)) && (h == (c_h[i] - 1)))
			return i;
	}
	return NO_ELEMENTS_HERE;
}

#define N_MAX_QUEUE 4096
template <class T>
class queue {
	T _store[N_MAX_QUEUE];
	T *qbegin;
	T *qend;
public:
	void push(T e) {
		*qend = e;
		qend++;
	}

	T pop() {
		T elem = *qbegin;
		qbegin++;

		return elem;
	}

	int size() {
		return (int) (qend - qbegin);
	}
	queue() : qbegin(_store), qend(_store) {};
};

struct pos {
	int _w;
	int _h;
	int _d;
	pos(int w, int h, int d) : _w(w), _h(h), _d(d) {};
	pos() : _w(0), _h(0), _d(0) {};
};

void bfs(int w, int h)
{
	int local_element_idx;
	queue<struct pos> my_q;
	my_q.push(pos(w, h, 0));
	mark_visited(w, h);
	while(my_q.size()) {
		struct pos e = my_q.pop();
		local_element_idx = element_located_here(e._w, e._h);
		if (local_element_idx != NO_ELEMENTS_HERE)
			c_dist[local_element_idx] = e._d;

		if (can_move_here(e._w + 1, e._h)) {
			mark_visited(e._w + 1, e._h);
			my_q.push(pos(e._w + 1, e._h, e._d + 1));
		}
		if (can_move_here(e._w, e._h + 1)) {
			mark_visited(e._w, e._h + 1);
			my_q.push(pos(e._w, e._h + 1, e._d + 1));
		}
		if (can_move_here(e._w - 1, e._h)) {
			mark_visited(e._w - 1, e._h);
			my_q.push(pos(e._w - 1, e._h, e._d + 1));
		}
		if (can_move_here(e._w, e._h - 1)) {
			mark_visited(e._w, e._h - 1);
			my_q.push(pos(e._w, e._h - 1, e._d + 1));
		}
	}
}

int place_research_center(int w, int h)
{
	int long_dist = 0;
	memset(visited, 0, MAX_N * MAX_N);
	memset(c_dist, 0, MAX_ELEM);
	bfs(w, h);
	for (int i = 0; i < C; i++)
		long_dist = max(long_dist, c_dist[i]);
	return long_dist;
}

static bool research_center_could_be_placed(int w, int h)
{
	if (!is_road(w, h))
		return false;
	if (element_located_here(w, h) != NO_ELEMENTS_HERE)
		return false;
	return true;
}

int solve_test_case()
{
	int min_long_dist = N * N;
	for (int i_h = 0; i_h < N; i_h++)
		for (int i_w = 0; i_w < N; i_w++) {
			if (research_center_could_be_placed(i_w, i_h)) {
				int long_dist = place_research_center(i_w, i_h);
				min_long_dist = min(min_long_dist, long_dist);
			}
		}
	return min_long_dist;
}

int main()
{
	cin >> T;
	for (int i_t = 0;  i_t < T; i_t++) {
		int min_long_dist = 0;
		cin >> N >> C;
		for (int i_c = 0; i_c < C; i_c++) {
			cin >> c_h[i_c] >> c_w[i_c];
		}
		for (int i_h = 0; i_h < N; i_h++) {
			for (int i_w = 0; i_w < N; i_w++) {
				cin >> map[i_h][i_w];
			}
		}
		min_long_dist = solve_test_case();
		cout << "#" << i_t + 1 << " " << min_long_dist << endl;
	}
	return 0;
}

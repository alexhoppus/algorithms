#include <iostream>
#include <assert.h>

using namespace std;

int T;
int N;
#define N_TANKS 2
#define N_MAX 25

int g_r[N_TANKS], g_c[N_TANKS];
int map[N_MAX][N_MAX] = {0};
int beenthere[N_MAX][N_MAX][N_MAX][N_MAX] = {0};

void memset(void *ptr, int val, unsigned long nr)
{
	for (unsigned int i = 0; i < nr; i++)
		*((int *) ptr + i) = val;
}

#define N_MAX_QUEUE 1024*1024
template <class T>
class queue {
	T *_store[N_MAX_QUEUE];
	T **qbegin;
	T **qend;
public:
	void push(T* e) {
		*qend = e;
		qend++;
	}

	T *pop() {
		T *elem = *qbegin;
		qbegin++;
		return elem;
	}

	int size() {
		return (int) (qend - qbegin);
	}
	int vsize() {
		return (int) (qend - &_store[0]);
	}

	void flush() {
		qbegin = _store;
		qend =_store;
	}
	queue() : qbegin(_store), qend(_store) {
	};
};

enum DIRECTION {
	UP_LEFT,
	UP,
	UP_RIGHT,
	CE_LEFT,
	CE,
	CE_RIGHT,
	DOWN_LEFT,
	DOWN,
	DOWN_RIGHT,
	DIRECTION_END
};

bool inside_map(int r, int c)
{
	if (r > -1 && c > -1 && r < N && c < N)
		return true;
	return false;
}

struct position {
	int r;
	int c;
	position(int _r, int _c) : r(_r), c(_c) {};
	position(const struct position& _p) : r(_p.r), c(_p.c) {};
	position() : r(0), c(0) {};

};

#define POS_CACHE_MAX 10*1024*1024
struct position pos_cache[POS_CACHE_MAX];
static int pos_cache_idx = 0;
static inline struct position *alloc_pos()
{
	return &pos_cache[pos_cache_idx++];
}

struct position *get_new_pos_check(int r, int c) {
	if (inside_map(r, c) && !map[r][c]) {
		struct position *p = alloc_pos();
		p->r = r; p->c = c;
		return p;
	} else {
		return NULL;
	}
}
struct position *get_new_pos(DIRECTION d, int r, int c) {
	switch(d) {
		case UP_LEFT:
			return get_new_pos_check(r - 1, c - 1);
		case UP:
			return get_new_pos_check(r - 1, c);
		case UP_RIGHT:
			return get_new_pos_check(r - 1, c + 1);
		case CE_LEFT:
			return get_new_pos_check(r, c - 1);
		case CE:
			return get_new_pos_check(r, c);
		case CE_RIGHT:
			return get_new_pos_check(r, c + 1);
		case DOWN_LEFT:
			return get_new_pos_check(r + 1, c - 1);
		case DOWN:
			return get_new_pos_check(r + 1, c);
		case DOWN_RIGHT:
			return get_new_pos_check(r + 1, c + 1);
	};
}

struct both_position {
	int _level;
	struct position *pos[N_TANKS];
	both_position(struct position *p1, struct position *p2, int level) { pos[0] = p1; pos[1] = p2;_level = level;};
	both_position() {pos[0] = NULL; pos[1] = NULL; _level = 0;};
	inline bool operator==(const struct both_position& rhs)
	{
		if (pos[0]->r == rhs.pos[0]->r && pos[0]->c == rhs.pos[0]->c &&
			pos[1]->r == rhs.pos[1]->r && pos[1]->c == rhs.pos[1]->c) {
			return true;
		}
		return false;
	}
	friend ostream& operator<<(ostream& os, const both_position& dt);
};

#define BPOS_CACHE_MAX 1024*1024
struct both_position bpos_cache[BPOS_CACHE_MAX];
static int bpos_cache_idx = 0;
static inline struct both_position *alloc_bpos()
{
	return &bpos_cache[bpos_cache_idx++];
}

ostream& operator<<(ostream& os, const both_position& bp)
{
	for (int i = 0; i < N_TANKS; i++) {
		os << "r[" << i << "]" << " = " << bp.pos[i]->r << " ";
		os << "c[" << i << "]" << " = " << bp.pos[i]->c << " ";
	}
	os << endl;
	return os;
};

queue<struct both_position> q;

struct position *available_positions[N_TANKS][DIRECTION_END];
int n_available_positions[N_TANKS] = {0};

bool both_position_allowed(struct position *p1, struct position *p2)
{
	if (p1->r <= p2->r + 1 && p1->r >= p2->r - 1 && p1->c <= p2->c + 1 && p1->c >= p2->c - 1)
		return false;
	if (p2->r <= p1->r + 1 && p2->r >= p1->r - 1 && p2->c <= p1->c + 1 && p2->c >= p1->c - 1)
		return false;
	return true;
}

int solve(struct both_position *bp)
{
	q.push(bp);
	int step = 0;
	while(q.size() != 0) {
		struct both_position *bp = q.pop();
		for (int i_t = 0; i_t < N_TANKS; i_t++) {
			n_available_positions[i_t] = 0;
			struct position *p = bp->pos[i_t];
			for (int direction = 0; direction < DIRECTION_END; direction++) {
				struct position *new_pos = get_new_pos((DIRECTION)direction, p->r, p->c);
				if (new_pos) {
					available_positions[i_t][n_available_positions[i_t]] = new_pos;
					n_available_positions[i_t]++;
				}
			}
		}
		for (int i_pos1 = 0; i_pos1 < n_available_positions[0]; i_pos1++) {
			struct position *p1 = available_positions[0][i_pos1];
			for (int i_pos2 = 0; i_pos2 < n_available_positions[1]; i_pos2++) {
				struct position *p2 = available_positions[1][i_pos2];
				if (both_position_allowed(p1, p2)) {
					if (!beenthere[p1->r][p1->c][p2->r][p2->c]) {
						struct both_position *new_bp = alloc_bpos();
						new_bp->pos[0] = p1; new_bp->pos[1] = p2; new_bp->_level = bp->_level + 1;
						if (new_bp->pos[0]->r == g_r[0] && new_bp->pos[0]->c == g_c[0] &&
								new_bp->pos[1]->r == g_r[1] && new_bp->pos[1]->c == g_c[1])
							return bp->_level + 1;
						q.push(new_bp);
						beenthere[p1->r][p1->c][p2->r][p2->c] = 1;
					}
				}
			}
		}
		step++;
	}
	return -1;
}

int main()
{
	cin >> T;
	for (int i_t = 0; i_t < T; i_t++) {
		pos_cache_idx = 0; bpos_cache_idx = 0;
		struct both_position *bp = alloc_bpos();
		q.flush();
		memset(map, 0x0, N_MAX);
		memset(beenthere, 0x0, N_MAX * N_MAX * N_MAX * N_MAX);
		cin >> N;
		for (int i_c = 0; i_c < N_TANKS; i_c++) {
			int r, c;
			struct position *p = alloc_pos();
			cin >> r >> c;
			p->r = r - 1; p->c = c - 1;
			bp->pos[i_c] = p;
			cin >> g_r[i_c] >> g_c[i_c];
			g_r[i_c]--; g_c[i_c]--;
		}
		for (int i_r = 0; i_r < N; i_r++) {
			for (int i_c = 0; i_c < N; i_c++) {
				cin >> map[i_r][i_c];
			}
		}
		int ans = solve(bp);
		cout << "#" << i_t + 1 << " " << ans << endl;
	}
	return 0;
}

#include <iostream>
#include <assert.h>

using namespace std;

int T;
int N;
#define N_TANKS 2
#define N_MAX 25

int g_r[N_TANKS], g_c[N_TANKS];

int map[N_MAX][N_MAX] = {0};

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

	void print_all()
	{
		int it = 0;
		cout << "====================================" << endl;
		for (T** i = qbegin; i != qend; i++) {
			cout << "q["<<it++<<"] = " << **i << endl;
		}
		cout << "====================================" << endl;
	}

	bool in_queue(T *e) {
		//cout << "===========Searching for============" << endl;
		//cout << *e << endl;
		//cout << "====================================" << endl;
		for (T** i = qbegin; i != qend; i++) {
			if (*e == **i)
				return true;
		}
		//cout << "====================================" << endl;
		return false;
	}

	int size() {
		return (int) (qend - qbegin);
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
	struct position *get_new_pos_check(int r, int c) {
		//cout << "r = " << r << " c= " << c << endl;
		if (inside_map(r, c) && !map[r][c]) {
			return new position(r, c);
		} else {
			return NULL;
		}
	}
	struct position *get_new_pos(DIRECTION d) {
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
};

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

int solve(struct both_position bp)
{
	q.push(&bp);
	int step = 0;
	while(q.size() != 0) {
		//cout << "iteration " << step << " qsize " << q.size() << endl;
		//q.print_all();
		struct both_position *bp = q.pop();
		if (!(step % 10000)) {
			cout << "lvl " << bp->_level << endl;
			cout << "qsize " << q.size() << endl;
		}
		if (bp->_level > 200) {
			return -1;
		}
		//cout << "Considering bp" << endl;
		//cout << *bp << endl;
		for (int i_t = 0; i_t < N_TANKS; i_t++) {
			n_available_positions[i_t] = 0;
			struct position *p = bp->pos[i_t];
			//cout << "Get position " << "[" << p->r << " , " << p->c << "]" << endl;
			for (int direction = 0; direction < DIRECTION_END; direction++) {
				//cout << "direction " << direction << endl;
				struct position *new_pos = p->get_new_pos((DIRECTION)direction);
				//cout << "new_pos is " << new_pos << endl;
				if (new_pos) {
					available_positions[i_t][n_available_positions[i_t]] = new_pos;
					n_available_positions[i_t]++;
				}
			}
			//cout << "tank " << i_t << " , " << n_available_positions[i_t] << " elements added" << endl;
		}
		for (int i_pos1 = 0; i_pos1 < n_available_positions[0]; i_pos1++) {
			struct position *p1 = available_positions[0][i_pos1];
			for (int i_pos2 = 0; i_pos2 < n_available_positions[1]; i_pos2++) {
				struct position *p2 = available_positions[1][i_pos2];
				if (both_position_allowed(p1, p2)) {
					bool in_place = true;
					struct both_position *new_bp = new both_position(p1, p2, bp->_level + 1);
					for (int i = 0; i < N_TANKS; i++) {
						if (new_bp->pos[i]->r == g_r[i] && new_bp->pos[i]->c == g_c[i]) {
						}
						else
							in_place = false;
					}
					if (in_place)
						return bp->_level + 1;
					//cout << "Adding bp" << endl;
					if (!q.in_queue(new_bp)) {
						//cout << "Adding elem" << endl;
						//cout << *new_bp << endl;
						q.push(new_bp);
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
		struct both_position bp;
		cin >> N;
		for (int i_c = 0; i_c < N_TANKS; i_c++) {
			int r, c;
			cin >> r >> c;
			bp.pos[i_c] = new position(r - 1, c - 1);
			cin >> g_r[i_c] >> g_c[i_c];
			g_r[i_c]--; g_c[i_c]--;
		}
		for (int i_r = 0; i_r < N; i_r++) {
			for (int i_c = 0; i_c < N; i_c++) {
				cin >> map[i_r][i_c];
			}
		}
		q.flush();
		int ans = solve(bp);
		cout << "#" << i_t << " " << ans << endl;
	}
	return 0;
}

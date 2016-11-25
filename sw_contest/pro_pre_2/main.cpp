#include <iostream>

#define MAX_N 101

using namespace std;

int T;
int N;

int b[MAX_N] = {0};

int ans;
//#define DEBUG 1
void memset(void *ptr, unsigned long long val, unsigned long nr)
{
	for (unsigned int i = 0; i < nr; i++)
		*((unsigned long long *) ptr + i) = val;
}

static inline void set_bit(unsigned long long *arg, int bit)
{
	arg += bit / (sizeof(unsigned long long) * 8);
	*arg |= (1 << (bit % (sizeof(unsigned long long) * 8)));
}

static inline int get_bit(unsigned long long *arg, int bit)
{
	arg += bit / (sizeof(unsigned long long) * 8);
	return *arg & (1 << (bit % (sizeof(unsigned long long) * 8)));
}

static inline void b_set_state(unsigned long long *mask, int b_num, int state)
{
	if (state == 0x1) {
//		cout << "set state 0x1 bnum - " << b_num << endl;
		set_bit(mask, b_num * 2 + 1);
	} else if (state == 0x2) {
//		cout << "set state 0x2 bnum - " << b_num << endl;
		set_bit(mask, b_num * 2);
	} else {
//		cout << "set state 0x0 bnum - " << b_num << endl;
	}
}

static inline int b_get_state(unsigned long long *mask, int b_num)
{
	if (get_bit(mask, b_num * 2 + 1))
		return 0x1;
	if (get_bit(mask, b_num * 2))
		return 0x2;
	return 0x0;
}

struct mask {
	unsigned long long m1;
	unsigned long long m2;
	unsigned long long m3;
	unsigned long long m4;
};

//#if DEBUG
void print_mask(struct mask *m)
{
	for (int i = 0; i < N; i++) {
		cout << b_get_state((unsigned long long *)m, i) << " ";
	}
	cout << endl;
}
//#endif

void compute_height(struct mask *m)
{
	int t1 = 0, t2 = 0;
	for (int i = 0; i < N; i++) {
		int state = b_get_state((unsigned long long *)m, i);
		if (state == 0x1)
			t1 += b[i];
		else if (state == 0x2)
			t2 += b[i];
	}
#if DEBUG
	cout << "t1 = " << t1 << " t2 = " << t2 << endl;
#endif
	if (t1 == t2) {
		if (ans < t1)
			ans = t1;
	}
}

void solve(int blck_idx, struct mask *m)
{
	if (blck_idx >= N)
		return;

	blck_idx++;
	compute_height(m);
//#if DEBUG
	cout << "blck idx " << blck_idx << endl;
	print_mask(m);
//#endif

	for (int i_s = 0; i_s < 3; i_s++) {
		struct mask new_m = *m;
		b_set_state((unsigned long long *)&new_m, blck_idx, i_s);
		solve(blck_idx, &new_m);
	}
	return;
}

int main()
{
	std::ios::sync_with_stdio(false);
	cin >> T;
	for (int i_t = 0; i_t < T; i_t++) {
		cin >> N;
		for (int i_n = 0; i_n < N; i_n++) {
			cin >> b[i_n];
		}
		ans = 0;
		struct mask m;
		memset(&m, 0x0, 4);
		solve(-1, &m);
		cout << "#" << i_t + 1 << " " << ans << endl;
	}

	return 0;
}

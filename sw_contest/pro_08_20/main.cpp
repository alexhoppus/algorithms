#include <iostream>
#include <stdio.h>

using namespace std;
#define MAX_N 1000
#define MAX_CPU 5
#define MAX_LIMIT 10

int T[MAX_N];
int L[MAX_N];

int N;

unsigned long long cpu_queue;
int cpus = 5;
int ans;
int cpu_used[MAX_CPU] = {0};

char beenthere[MAX_N][MAX_LIMIT][MAX_LIMIT][MAX_LIMIT][MAX_LIMIT][MAX_LIMIT] = {0};

#define DEBUG 1

void memset(void *ptr, char val, unsigned long nr)
{
	for (unsigned int i = 0; i < nr; i++)
		*((char *) ptr + i) = val;
}

static inline int cpu_used_to_ans(void)
{
	int ret = 0;
	for (int i = 0; i < MAX_CPU; i++)
		if (cpu_used[i])
			ret++;
	return ret;
}

static inline int cpu_queue_get(int idx, unsigned long long *cpu_queue)
{
	char *ptr = (char *)cpu_queue;
	ptr += idx;
	return (int) *ptr;
}
static inline void set_beenthere(int pack_idx, unsigned long long *cpu_queue)
{
	int q[MAX_CPU];
	for (int i = 0; i < MAX_CPU; i++) {
		q[i] = cpu_queue_get(i, cpu_queue);
	}
#if DEBUG
	printf("%s %d %d %d %d %d %d\n", __func__, pack_idx, q[0], q[1], q[2], q[3], q[4]);
#endif
	beenthere[pack_idx][q[0]][q[1]][q[2]][q[3]][q[4]] = '1';
}

static inline bool get_beenthere(int pack_idx, unsigned long long *cpu_queue)
{
	int q[MAX_CPU];
	for (int i = 0; i < MAX_CPU; i++) {
		q[i] = cpu_queue_get(i, cpu_queue);
	}
#if DEBUG
	printf("%s %d %d %d %d %d %d\n", __func__, pack_idx, q[0], q[1], q[2], q[3], q[4]);
#endif
	if (beenthere[pack_idx][q[0]][q[1]][q[2]][q[3]][q[4]] == '1')
		return true;
	else
		return false;
}
static inline int cpu_queue_change(int idx, unsigned long long *cpu_queue, int val)
{
	char *ptr = (char *)cpu_queue;
#if DEBUG
//	printf("cpu_queue_change idx %d val %d cur value %d\n", idx, val, cpu_queue_get(idx, cpu_queue));
#endif
	ptr += idx;
	val = val < -10 ? -10 : val;
	*ptr += (char) val;
	if ((int) *ptr > MAX_LIMIT)
		return 1;
	if ((int)*ptr < 0)
		*ptr = (char )0x0;
#if DEBUG
//	printf("changed value %d\n", cpu_queue_get(idx, cpu_queue));
#endif
	return 0;
}

static inline void get_workload(int pack_idx, unsigned long long *cpu_queue)
{
	if (!pack_idx)
		return;
	int diff = T[pack_idx] - T[pack_idx - 1];
	for (int i_cpu = 0; i_cpu < cpus; i_cpu++)
		cpu_queue_change(i_cpu, cpu_queue, -diff);
}

int solve(int cpu_idx, int pack_idx, unsigned long long cpu_queue)
{
	int ret = 0;
	cpu_used[cpu_idx] = 1;
#if DEBUG
	//printf("solve: cpu_idx %d pack_idx %d ", cpu_idx, pack_idx);
#endif
	if (get_beenthere(pack_idx, &cpu_queue))
		return 0;
	set_beenthere(pack_idx, &cpu_queue);
	if (ans != -1)
		return 1;
	if (pack_idx >= N) {
		ans = cpu_used_to_ans();
/*#if DEBUG
		printf("ans = %d\n", ans);
#endif*/
		return 1;
	}
	get_workload(pack_idx, &cpu_queue);

	if (cpu_queue_change(cpu_idx, &cpu_queue, L[pack_idx]))
		return 0;
#if DEBUG
	printf("solve: cpu_idx %d pack_idx %d ", cpu_idx, pack_idx);
	for (int i = 0; i < cpus; i++) {
		int cpu_i_q = cpu_queue_get(i, &cpu_queue);
		printf(" cpu[%d] = %d ", i, cpu_i_q);
	}
	printf("\n");
#endif
	pack_idx++;
	for (int i = 0; i < cpus; i++)
		ret |= solve(i, pack_idx, cpu_queue);
	return ret;
}

int main()
{
	int t;
	std::ios::sync_with_stdio(false);
	cin >> t;
	for (int i_t = 0; i_t < t; i_t++) {
		cin >> N;
		cpu_queue = 0;
		ans = -1;
		memset(beenthere, '0', MAX_N * MAX_LIMIT * MAX_LIMIT * MAX_LIMIT * MAX_LIMIT * MAX_LIMIT);
		for (int i = 0; i < MAX_CPU; i++)
			cpu_used[i] = 0;
		for (int i_n = 0; i_n < N; i_n++) {
			cin >> T[i_n];
			cin >> L[i_n];
		}
		solve(0, 0, cpu_queue);
		printf("# %d %d", i_t + 1, ans);
	}
}

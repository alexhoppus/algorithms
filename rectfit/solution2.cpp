#include <stdio.h>
#include <iostream>
#include <vector>
#include <assert.h>
#include <algorithm>

#define MAX_H 25
#define MAX_W 10
using namespace std;

int w = 0, h = 0;
int matrix[MAX_H + 1];
int row = 0;
int n_squares = 0;
static inline int max(int a, int b)
{
        return a > b ? a : b;
}

static inline void set_bit(int *arg, int bit)
{
	*arg |= (1 << bit);
}

static inline void clear_bit(int *arg, int bit)
{
	*arg &= (~(1 << bit));
}

static inline int test_bit(int *arg, int bit)
{
	return (*arg & (1 << bit));
}
#ifdef DEBUG
void print_profile(pair<int, int> profile)
{
        for (int i = 0; i < w; i++) {
                if (test_bit(&profile.first, i))
                        cout << "1";
                else
                        cout << "0";
        }
	cout << " " << profile.second;
        cout << endl;
}
#endif

int _generate_profiles(int profile, int next_profile, int step, vector<pair<int, int> > &profiles, int value)
{
	int i = step;
	assert(!(row >= (h - 1)));
	for (; i < w - 1; i++) {
		if ((!test_bit(&profile, i)) && (!test_bit(&profile, i + 1)) &&
			(!test_bit(&next_profile, i)) && (!test_bit(&next_profile, i + 1))) {
			int changed_next_profile = next_profile;
			int changed_profile = profile;
			set_bit(&changed_next_profile, i);
			set_bit(&changed_next_profile, i + 1);
			set_bit(&changed_profile, i);
			set_bit(&changed_profile, i + 1);
			n_squares = max(n_squares, value + 1);
			profiles.push_back(*(new pair<int, int>(changed_next_profile, value + 1)));
			if (i + 2 < w - 1) {
				_generate_profiles(changed_profile, changed_next_profile,
					i + 2, profiles, value + 1);
				_generate_profiles(profile, next_profile,  i + 2, profiles, value);
			}
			
		}	
	}		
}

int generate_profiles(pair<int, int> profile, vector<pair<int, int> > &profiles)
{
	int next_profile = matrix[row + 1];
	int step = 0;
	profiles.push_back(*(new pair<int, int>(next_profile, profile.second)));
	_generate_profiles(profile.first, next_profile, step, profiles, profile.second);
}

int main()
{
	int n_tests, i, sum = 0;
	cin >> n_tests;
	for (i = 0; i < n_tests; i++) {
		cin >> h;
                cin >> w;
                int rows, cols;
		n_squares = 0;
		cout << "Test case " << i << " " << "w=" << w << " " << "h=" << h << endl;
                for (rows = 0; rows < h; rows++) {
                        for (cols = 0; cols < w; cols++) {
				int val;
                                cin >> val;
				if (val)
					set_bit(&matrix[rows], cols);
				else
					clear_bit(&matrix[rows], cols);
                        }
                }
		row = -1;
		vector< pair<int, int> > profiles;
		profiles.push_back(*(new pair<int, int>(0xffffffff, 0)));
		while (row < h - 1) {
			vector< pair<int, int> > new_profiles;
			
			for (int i = 0; i < profiles.size(); i++)
				generate_profiles(profiles[i], new_profiles);

			sort(new_profiles.begin(), new_profiles.end(), [](pair<int, int> a, pair<int, int> b) {
       				return b.first < a.first;   
			});
			assert(!new_profiles.empty());
			int j = 0;
			profiles.clear();
			profiles.push_back(new_profiles[0]);
			for (int i = 1; i < new_profiles.size(); i++) {
				if (profiles[j].first == new_profiles[i].first) {
					profiles[j].second = max(profiles[j].second, new_profiles[i].second);
				} else {
					profiles.push_back(new_profiles[i]);
					j++;
				} 
			}
#ifdef DEBUG
			cout << "=====Begin=====" << endl;
			for (int p = 0; p < profiles.size(); p++) {
				print_profile(profiles[p]);
			}
			cout << "=====End=====" << endl;
#endif
			row++;
		}
		cout << "Answer: " << n_squares << endl;
        }
        return 0;
}

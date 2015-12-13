#include <stdio.h>
#include <iostream>

#define MAX_H 25
#define MAX_W 25
int w = 0, h = 0;
int matrix[MAX_H][MAX_W];

#define NO_END  0
#define END_LINE 1
#define END_MAT  2
#define SHAPE_SIZE 2

static inline int max(int a, int b)
{
	return a > b ? a : b;
}

using namespace std;

void set_bit(unsigned long long *tmp_mat, int row, int col)
{
	int nbit = row * w + col;	
	*tmp_mat = *tmp_mat | (1 << nbit);
}

int test_bit(unsigned long long *tmp_mat, int row, int col)
{
	int nbit = row * w + col;
	return (*tmp_mat >> nbit) & (1);
}

unsigned long long update_matrix(int start_row, unsigned long long old_partmatrix)
{
	int col, row;
	unsigned long long tmp_matrix = (old_partmatrix >> w);
	for (col = 0; col < w; col++)
		if (matrix[start_row + 1][col])
			set_bit(&tmp_matrix, 1, col);
	return tmp_matrix;
}

int we_can_place(int col, unsigned long long part_matrix)
{
	if (!test_bit(&part_matrix, 0, col) && !test_bit(&part_matrix, 0, col + 1) &&
		!test_bit(&part_matrix, 1, col) && !test_bit(&part_matrix, 1, col + 1))
		return 1;
	return 0;
}
int decide_place_or_not(int row, int col, unsigned long long part_matrix);

void place_figure(int row, int col, unsigned long long *part_matrix)
{
	int r = 0, c = 0;
	for (r = 0; r < SHAPE_SIZE; r++)
		for (c = 0; c < SHAPE_SIZE; c++)
			set_bit(part_matrix, r, col + c);
}

int determine_end_cond(int row, int col)
{
	if (col + SHAPE_SIZE == w) {
		if (row + SHAPE_SIZE == h)
			return END_MAT;
		return END_LINE;
	}
	return NO_END;
}

/*Updates row col and part matrix*/
int get_next_state(int *row, int *col, unsigned long long *part_matrix)
{
       if (determine_end_cond(*row, *col) == END_LINE) {
                /* We are going to the next line*/
                *row += 1;
                *col = 0;
		/*Update second line of part matrix*/
                *part_matrix = update_matrix(*row, *part_matrix);
        } else if (determine_end_cond(*row, *col) == NO_END) {
                /* We are somewhere inside line */
                *col += 1;
        } else {
                /*End of matrix*/
                return END_MAT;
        }
	return 0;
}

int place(int row, int col, unsigned long long part_matrix)
{
	place_figure(row, col, &part_matrix);
	if (END_MAT == get_next_state(&row, &col, &part_matrix))
		return 1;
        return 1 + decide_place_or_not(row, col, part_matrix);
}

int dont_place(int row, int col, unsigned long long part_matrix)
{
        if (END_MAT == get_next_state(&row, &col, &part_matrix))
                return 0;
	return 0 + decide_place_or_not(row, col, part_matrix);
}

int decide_place_or_not(int row, int col, unsigned long long part_matrix)
{
	int sum;
	if (we_can_place(col, part_matrix)) {
		/*If we can place we have choice*/
		int place_sum = place(row, col, part_matrix);
		int not_place_sum = dont_place(row, col, part_matrix);
		sum = max(place_sum, not_place_sum);
	} else {
		/*We have no choice*/
		sum = dont_place(row, col, part_matrix);
	}
	return sum;
}

int main()
{
	int n_tests, i;
	cin >> n_tests;
	for (i = 0; i < n_tests; i++) {
		cin >> h;
		cin >> w;
		int rows, cols;
		for (rows = 0; rows < h; rows++) {
			for (cols = 0; cols < w; cols++) {
				cin >> matrix[rows][cols];
			}
		}

                /*Part - matrix:
                        bitarray which encodes current and following row.
                */
		unsigned long long part_matrix = 0;
		for (rows = 0; rows < SHAPE_SIZE; rows++)
			for (cols = 0; cols < w; cols++) {
		                if (matrix[rows][cols])
					set_bit(&part_matrix, rows, cols);
			}
		cout << h << " " << w << endl;
		int sum = decide_place_or_not(0, 0, part_matrix);
		cout << sum << endl;
		
	}
	return 0;
} 

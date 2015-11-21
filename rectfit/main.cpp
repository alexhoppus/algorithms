#include <stdio.h>
#include <iostream>

#define MAX_H 25
#define MAX_W 10
int matrix[MAX_H][MAX_W];

using namespace std;

void print_matrix(int w, int h)
{
	int rows, cols;
	cout << endl;
	for (rows = 0; rows < h; rows++) {
		for (cols = 0; cols < w; cols++) {
			cout << matrix[rows][cols];
		}
		cout << endl;
	}
	cout << endl;
}

int main()
{
	int n_tests, i;
	cin >> n_tests;
	for (i = 0; i < n_tests; i++) {
		int w, h;
		cin >> h;
		cin >> w;
		int rows, cols;
		for (rows = 0; rows < h; rows++) {
			for (cols = 0; cols < w; cols++) {
				cin >> matrix[rows][cols];
			}
		}
		print_matrix(w, h);
		
	}
	
	return 0;
}

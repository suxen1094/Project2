#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class MAP {
public:
	MAP(int m, int n): row(m), col(n) {
		map = new char*[m];
		for (int i = 0; i < m; i++) {
			map[i] = new char[n];
		}
	}
	void READ(fstream & fin) {
		for (int i = 0; i < this->row; i++) {
			for (int j = 0; j < this->col; j++) {
				char c;
				fin >> c;
				(this->map)[i][j] = c;
				cout << map[i][j]; // debug
			}
			cout << endl; // debug
		}
	}
	int row;
	int col;
	char **map;
};

int main() {
	fstream fin, fout;
	fin.open("floor1.data", ios::in);
	fout.open("final.path", ios::out);
	int m, n, B;
	fin >> m >> n >> B;
	cout << "m = " << m << ", n = " << n << ", B = " << B << endl; // debug
	MAP mm(m, n);
	mm.READ(fin);
}
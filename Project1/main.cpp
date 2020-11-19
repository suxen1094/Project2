#include <iostream>
#include <fstream>
#include <string>
#include <queue>
#include <stack>

using namespace std;
int chargeRow, chargeCol;
queue<int> ROW;
queue<int> COL;
queue<int> DIST;

class MAP {
public:

	MAP(int m, int n): row(m), col(n) {
		map = new int*[m];
		dist = new int*[m];
		for (int i = 0; i < m; i++) {
			map[i] = new int[n];
			dist[i] = new int[n];
			fill(dist[i], dist[i] + n, 0);
		}
	}

	void read(fstream & fin) {
		for (int i = 0; i < this->row; i++) {
			for (int j = 0; j < this->col; j++) {
				char c;
				fin >> c;
				if (c == 'R') {
					(this->map)[i][j] = 5;
					chargeRow = i;
					chargeCol = j;
				}
				else {
					(this->map)[i][j] = c - '0';
					if (c == '0') {
						need_clean++;
					}
				}
			}
		}
	}

	// used to debug
	void showmap() {
		cout << "map:\n\n";
		for (int i = 0; i < this->row; i++) {
			for (int j = 0; j < this->col; j++) {
				cout << this->map[i][j];
			}
			cout << endl;
		}
		cout << "\n----------------------\n\n";
	}

	void showdist() {
		cout << "dist:\n\n";
		for (int i = 0; i < row; i++) {
			for (int j = 0; j < col; j++) {
				cout << this->dist[i][j];
			}
			cout << endl;
		}
		cout << "\n----------------------\n\n";
	}

	int row;
	int col;
	int **map;
	int need_clean = 0;
	int **dist;
};

class robot {
public:
	robot(int B) : battery(B) {}

	void BFS(MAP & mm) {
		ROW.push(chargeRow);
		COL.push(chargeCol);

		while (!ROW.empty()) {
			int curRow = ROW.front();
			int curCol = COL.front();
			int curDist = mm.dist[curRow][curCol];
			ROW.pop();
			COL.pop();
			mm.map[curRow][curCol] = 3;

			// find lefthand side
			if (curCol > 0) {
				if (mm.map[curRow][curCol - 1] == 0) {
					ROW.push(curRow);
					COL.push(curCol - 1);
					mm.dist[curRow][curCol - 1] = curDist + 1;
				}
			}
			// find top side
			if (curRow > 0) {
				if (mm.map[curRow - 1][curCol] == 0) {
					ROW.push(curRow - 1);
					COL.push(curCol);
					mm.dist[curRow - 1][curCol] = curDist + 1;
				}
			}
			// find righthand side
			if (curCol < mm.col - 1) {
				if (mm.map[curRow][curCol + 1] == 0) {
					ROW.push(curRow);
					COL.push(curCol + 1);
					mm.dist[curRow][curCol + 1] = curDist + 1;
				}
			}
			// find down side
			if (curRow < mm.row - 1) {
				if (mm.map[curRow + 1][curCol] == 0) {
					ROW.push(curRow + 1);
					COL.push(curCol);
					mm.dist[curRow + 1][curCol] = curDist + 1;
				}
			}
		}
	}

	void record(fstream & fout) {
		fout << step << endl;
	}

	int battery;
	int step = 0;
	int robotRow, robotCol;
};

int main() {
	// file operation
	fstream fin, fout;
	fin.open("floor2.data", ios::in);
	fout.open("final.path", ios::out);
	int m, n, B;
	fin >> m >> n >> B;
	cout << "m = " << m << ", n = " << n << ", B = " << B << endl; // debug

	// my objects
	MAP mm(m, n);
	robot r(B);
	mm.read(fin);
	mm.showmap();
	mm.showdist();
	r.BFS(mm);
	mm.showmap();
	mm.showdist();
	r.record(fout);
}
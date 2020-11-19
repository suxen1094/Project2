#include <iostream>
#include <fstream>
#include <string>
#include <queue>
#include <stack>

enum direction
{
	LEFT = 1, RIGHT, TOP, DOWN, INITIAL
};

using namespace std;
int chargeRow, chargeCol;
queue<int> qrow;
queue<int> qcol;
queue<int> qdist;

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
		qrow.push(chargeRow);
		qcol.push(chargeCol);

		while (!qrow.empty()) {
			int curRow = qrow.front();
			int curCol = qcol.front();
			int curDist = mm.dist[curRow][curCol];
			qrow.pop();
			qcol.pop();
			mm.map[curRow][curCol] = 3;

			// find lefthand side
			if (curCol > 0) {
				if (mm.map[curRow][curCol - 1] == 0) {
					qrow.push(curRow);
					qcol.push(curCol - 1);
					mm.dist[curRow][curCol - 1] = curDist + 1;
				}
			}
			// find top side
			if (curRow > 0) {
				if (mm.map[curRow - 1][curCol] == 0) {
					qrow.push(curRow - 1);
					qcol.push(curCol);
					mm.dist[curRow - 1][curCol] = curDist + 1;
				}
			}
			// find righthand side
			if (curCol < mm.col - 1) {
				if (mm.map[curRow][curCol + 1] == 0) {
					qrow.push(curRow);
					qcol.push(curCol + 1);
					mm.dist[curRow][curCol + 1] = curDist + 1;
				}
			}
			// find down side
			if (curRow < mm.row - 1) {
				if (mm.map[curRow + 1][curCol] == 0) {
					qrow.push(curRow + 1);
					qcol.push(curCol);
					mm.dist[curRow + 1][curCol] = curDist + 1;
				}
			}
		}
	}

	void DFS(MAP & mm, fstream & fout) {
		while (mm.need_clean) {
			stack<int> srow;
			stack<int> scol;
			stack<int> back_srow;
			stack<int> back_scol;
			stack<int> state;
			int curStep = 0;
			state.push(INITIAL);
			srow.push(chargeRow);
			scol.push(chargeCol);
			while (!srow.empty()) {
				if (!mm.need_clean) break;
				int curRow = srow.top();
				int curCol = scol.top();
				int lastState = state.top();
				bool go = false;

				srow.pop();
				scol.pop();
				back_srow.push(curRow);
				back_scol.push(curCol);
				step++;
				curStep++;

				cout << curRow << ' ' << curCol << endl;
				if (mm.map[curRow][curCol] == 0) {
					mm.map[curRow][curCol] = 3;
					mm.need_clean--;
				}
				mm.showmap();
				if (curStep * 2 > battery) break;

				// find lefthand side(1)
				if (curCol > 0) {
					if (mm.map[curRow][curCol - 1] == 0) {
						srow.push(curRow);
						scol.push(curCol - 1);
						state.push(LEFT);
						go = true;
					}
				}
				// find top side
				if (curRow > 0 && !go) {
					if (mm.map[curRow - 1][curCol] == 0) {
						srow.push(curRow - 1);
						scol.push(curCol);
						state.push(TOP);
						go = true;
					}
				}
				// find righthand side
				if (curCol < mm.col - 1 && !go) {
					if (mm.map[curRow][curCol + 1] == 0) {
						srow.push(curRow);
						scol.push(curCol + 1);
						state.push(RIGHT);
						go = true;
					}
				}
				// find down side
				if (curRow < mm.row - 1 && !go) {
					if (mm.map[curRow + 1][curCol] == 0) {
						srow.push(curRow + 1);
						scol.push(curCol);
						state.push(DOWN);
						go = true;
					}
				}

				if (!go) {
					if (lastState == LEFT) {
						srow.push(curRow);
						scol.push(curCol + 1);
					}
					else if (lastState == TOP) {
						srow.push(curRow + 1);
						scol.push(curCol);
					}
					else if (lastState == RIGHT) {
						srow.push(curRow);
						scol.push(curCol - 1);
					}
					else if (lastState == DOWN) {
						srow.push(curRow - 1);
						scol.push(curCol);
					}
					else if (lastState == INITIAL) {
						bool ini_go = false;
						// find down side
						if (curRow < mm.row - 1) {
							if (mm.map[curRow + 1][curCol] == 3) {
								srow.push(curRow + 1);
								scol.push(curCol);
								state.push(INITIAL);
								ini_go = true;
							}
						}
						// find righthand side
						if (curCol < mm.col - 1 && !ini_go) {
							if (mm.map[curRow][curCol + 1] == 3) {
								srow.push(curRow);
								scol.push(curCol + 1);
								state.push(INITIAL);
								ini_go = true;
							}
						}
						// find top side
						if (curRow > 0 && !ini_go) {
							if (mm.map[curRow - 1][curCol] == 3) {
								srow.push(curRow - 1);
								scol.push(curCol);
								state.push(INITIAL);
								ini_go = true;
							}
						}
						// find lefthand side(1)
						if (curCol > 0 && !ini_go) {
							if (mm.map[curRow][curCol - 1] == 3) {
								srow.push(curRow);
								scol.push(curCol - 1);
								state.push(INITIAL);
								go = true;
							}
						}
					}
				}
			}

			// go back to charge point
			// pop±¼­«½ÆªºÂI
			back_srow.pop();
			back_scol.pop();
			while (!back_srow.empty()) {
				step++;
				cout << back_srow.top() << ' ' << back_scol.top() << endl;
				back_srow.pop();
				back_scol.pop();
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
	fin.open("108062226.data", ios::in);
	fout.open("final.path", ios::out);
	int m, n, B;
	fin >> m >> n >> B;
	cout << "m = " << m << ", n = " << n << ", B = " << B << endl; // debug

	// my objects
	MAP mm(m, n);
	robot r(B);
	mm.read(fin);
	r.DFS(mm, fout);
	mm.showmap();
}
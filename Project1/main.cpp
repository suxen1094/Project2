#include <iostream>
#include <fstream>
#include <string>
#include <queue>
#include <stack>
#include <vector>

using namespace std;
queue<int> qrow;
queue<int> qcol;
queue<int> qdist;
vector<int> v;

class MAP {
public:

	MAP(int m, int n): row(m), col(n) {
		map = new int*[m];
		dist = new int*[m];
		visited = new int*[m];
		dfs_visited = new int*[m];
		for (int i = 0; i < m; i++) {
			map[i] = new int[n];
			dist[i] = new int[n];
			visited[i] = new int[n];
			dfs_visited[i] = new int[n];
			fill(dist[i], dist[i] + n, 0);
			fill(visited[i], visited[i] + n, 1);
			fill(dfs_visited[i], dfs_visited[i] + n, 1);
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
						visited[i][j] = 0;
						dfs_visited[i][j] = 0;
					}
				}
			}
		}
	}

	// used to debug
	void showmap(int ** m) {
		cout << "\n\n";
		for (int i = 0; i < this->row; i++) {
			for (int j = 0; j < this->col; j++) {
				cout << m[i][j];
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

	void showdistline(int num) {
		for (int i = 0; i < col; i++)
			cout << dist[num][i];
		cout << "\n----------------------\n\n";
	}


	int row;
	int col;
	int **map;
	int need_clean = 0;
	int **dist;
	int **visited;
	int **dfs_visited;
	int chargeRow, chargeCol;
};

class robot {
public:
	robot(int B) : battery(B) {}

	void BFS(MAP & mm) {
		qrow.push(mm.chargeRow);
		qcol.push(mm.chargeCol);
		while (!qrow.empty()) {
			int curRow = qrow.front();
			int curCol = qcol.front();
			int curDist = mm.dist[curRow][curCol];
			qrow.pop();
			qcol.pop();
			//mm.showmap();
			// find lefthand side
			if (curCol > 0) {
				if (mm.map[curRow][curCol - 1] == 0) {
					qrow.push(curRow);
					qcol.push(curCol - 1);
					mm.map[curRow][curCol - 1] = 3;
					mm.dist[curRow][curCol - 1] = curDist + 1;
				}
			}
			// find top side
			if (curRow > 0) {
				if (mm.map[curRow - 1][curCol] == 0) {
					qrow.push(curRow - 1);
					qcol.push(curCol);
					mm.map[curRow - 1][curCol] = 3;
					mm.dist[curRow - 1][curCol] = curDist + 1;
				}
			}
			// find righthand side
			if (curCol < mm.col - 1) {
				if (mm.map[curRow][curCol + 1] == 0) {
					qrow.push(curRow);
					qcol.push(curCol + 1);
					mm.map[curRow][curCol + 1] = 3;
					mm.dist[curRow][curCol + 1] = curDist + 1;
				}
			}
			// find down side
			if (curRow < mm.row - 1) {
				if (mm.map[curRow + 1][curCol] == 0) {
					qrow.push(curRow + 1);
					qcol.push(curCol);
					mm.map[curRow + 1][curCol] = 3;
					mm.dist[curRow + 1][curCol] = curDist + 1;
				}
			}
		}
	}

	void DFS(MAP & mm, fstream & fout) {

		while (mm.need_clean) {
			find = false;
			int MAX = 0;
			int ** k = new int *[mm.row];

			// find the point with biggest distance
			for (int i = 0; i < mm.row; i++) {
				k[i] = new int[mm.col];
				for (int j = 0; j < mm.col; j++) {
					k[i][j] = mm.dfs_visited[i][j];
					if (mm.visited[i][j] == 0) {
						if (MAX < mm.dist[i][j]) {
							far_row = i;
							far_col = j;
							MAX = mm.dist[i][j];
						}
					}
				}
			}
			
			// find the path from charge point to far point
			findpath_dfs(mm, mm.chargeRow, mm.chargeCol, k, 0);

			record(mm);
		}

		orderc.push(mm.chargeCol);
		orderr.push(mm.chargeRow);
	}

	void findpath_dfs(MAP &mm, int curRow, int curCol, int ** dfs_visit, int curstep) {
		sc.push(curCol);
		sr.push(curRow);
		if (curstep > mm.dist[far_row][far_col]) return;
		if (curRow == far_row && curCol == far_col) {
			find = true;
			return;
		}

		// left
		if (curCol > 0 && find == false) {
			if (dfs_visit[curRow][curCol - 1] == 0) {
				dfs_visit[curRow][curCol - 1] = 1;
				findpath_dfs(mm, curRow, curCol - 1, dfs_visit, curstep + 1);
			}
		}

		// right
		if (curCol < mm.col - 1 && find == false) {
			if (dfs_visit[curRow][curCol + 1] == 0) {
				dfs_visit[curRow][curCol + 1] = 1;
				findpath_dfs(mm, curRow, curCol + 1, dfs_visit, curstep + 1);
			}
		}

		// top
		if (curRow > 0 && find == false) {
			if (dfs_visit[curRow - 1][curCol] == 0) {
				dfs_visit[curRow - 1][curCol] = 1;
				findpath_dfs(mm, curRow - 1, curCol, dfs_visit, curstep + 1);
			}
		}

		// down
		if (curRow < mm.row - 1 && find == false) {
			if (dfs_visit[curRow + 1][curCol] == 0) {
				dfs_visit[curRow + 1][curCol] = 1; 
				findpath_dfs(mm, curRow + 1, curCol, dfs_visit, curstep + 1);
			}
		}
		if (find) {
			sr.push(curRow);
			sc.push(curCol);
		}
		else {
			sr.pop();
			sc.pop();
		}
	}

	void record(MAP &mm) {
		while (!sr.empty()) {
			step++;
			int r = sr.top();
			int c = sc.top();
			sr.pop();
			sc.pop();
			orderr.push(r);
			orderc.push(c);
			if (mm.visited[r][c] == 0) {
				mm.need_clean--;
			}
			mm.visited[r][c] = 1;
		}
	}

	bool find = false;
	int battery;
	int step = 0; 
	stack<int> sr, sc;
	queue<int> orderr, orderc;
	int far_row = 0, far_col = 0;
};

int main() {
	// file operation
	fstream fin, fout;
	fin.open("floor.data", ios::in);
	fout.open("3.path", ios::out);
	int m, n, B;
	fin >> m >> n >> B;
	cout << "m = " << m << ", n = " << n << ", B = " << B << endl; // debug

	// my objects
	MAP mm(m, n);
	robot r(B);
	mm.read(fin);
	mm.showmap(mm.dfs_visited);
	mm.showmap(mm.visited);
	r.BFS(mm);
	r.DFS(mm, fout);

	fout << r.step << endl;
	while (!r.orderr.empty()) {
		fout << r.orderr.front() << " " << r.orderc.front() << endl;
		r.orderc.pop();
		r.orderr.pop();
	}
}
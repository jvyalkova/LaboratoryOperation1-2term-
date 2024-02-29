#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>

using namespace std;

int N;
vector<int> critWay, reserve, works, sorted;
vector<vector<int>> main_matrix;

void matrix_read()
{
	ifstream file;

	vector<vector<int>> matrix;

	file.open("matrix.txt");
	if (file.is_open())
	{
		vector<int> row;
		string s1, s2;

		while (getline(file, s1, '\n'))
		{
			stringstream line(s1);

			while (getline(line, s2, ' '))
				row.push_back(stoi(s2));

			matrix.push_back(row);
			row.clear();
		}
	}

	N = matrix[0].size();
	works = matrix[N];
	matrix.erase(matrix.begin() + N);
	main_matrix = matrix;
}

void topsort(int m, vector<int>& visited)
{
	visited[m] = 1;
	for (int i = 0; i < N; i++) {
		if (main_matrix[m][i] == 1 && visited[i] != 1)
			topsort(i, visited);
	}

	sorted.push_back(m);
}

bool cycle_check()
{
	bool cycles;
	vector<vector<int>> M1 = main_matrix, M2(N, vector<int>(N));

	for (int l = 1; l < N; l++)
	{
		for (int i = 0; i < N; i++)
		{
			for (int j = 0; j < N; j++)
			{
				M2[i][j] = 0;
				for (int k = 0; k < N; k++)
					M2[i][j] = (int)(M2[i][j] || M1[i][k] && main_matrix[k][i]);
			}
			if (M2[i][i] != 0) return false;
		}
		M1 = M2;
	}
	return true;
}

int source_find()
{
	int i, j, k = 0, result;
	bool flag;
	vector<int> source(N);

	for (i = 0; i < N; i++)
	{
		flag = true;
		for (j = 0; j < N; j++)
			if (main_matrix[j][i] == 1) flag = false;
		if (flag) { source[i] = 1; k++; }
	}
	i = 0;

	if (k == 1)
	{
		while (source[i] == 0) i++;
		result = i;
	}
	else if (k == 0) result = -1;
	else
	{
		vector<vector<int>> new_matrix(N + 1, vector<int>(N + 1));

		for (i = 0; i < N; i++)
		{
			main_matrix[0][i + 1] = source[i];
			for (j = 0; j < N; j++)
				new_matrix[i + 1, j + 1] = main_matrix[i, j];
		}

		main_matrix = new_matrix;
		N++;

		result = 0;
	}


	if (result < 0)
	{
		cout << "No source.";
		exit(-2);
	}
	else cout << "Source: " << result << "\n\n";

	return result;
}

void transpos()
{
	vector<int> new_work_length(N);
	vector<vector<int>> new_matrix(N, vector<int>(N));

	for (int i = 0; i < N; i++)
	{
		int l = 0;

		while (sorted[l] != i)
			l++;

		new_work_length[l] = works[i];
		for (int j = 0; j < N; j++)
			if (main_matrix[i][j] == 1)
			{
				int k = 0;
				while (sorted[k] != j) k++;
				new_matrix[l][k] = 1;
			}
	}

	main_matrix = new_matrix;
	works = new_work_length;
}

void crit_way_find()
{
	int crit_way_forward, crit_way_back;
	vector<int> T(N), T1(N);

	for (int i = 0; i < N; i++)
		for (int j = i + 1; j < N; j++)
			if (main_matrix[i][j] == 1 && T[j] < T[i] + works[i])
				T[j] = T[i] + works[i];
	crit_way_forward = T[N - 1] + works[N - 1];

	for (int i = N - 1; i >= 0; i--)
		for (int j = i - 1; j >= 0; j--)
			if (main_matrix[j][i] == 1 && T1[j] < T1[i] + works[i])
				T1[j] = T1[i] + works[i];
	crit_way_back = T1[0] + works[0];

	cout << "Critical way (forward): " << crit_way_forward << "\n";
	cout << "Critical way (back): " << crit_way_back << "\n";

	cout << "Vertexes: ";
	for (int i = 0; i < N; i++)
	{
		int buff = works[i] + T[i] + T1[i];
		critWay.push_back(buff);
		if (buff == crit_way_forward)
			cout << i << " ";
		reserve.push_back(crit_way_forward - buff);
	}
	cout << "\n";

	cout << "Rest of time: ";
	for (int i = 0; i < N; i++)
		cout << reserve[i] << " ";
	cout << "\n";
}

int main()
{
	matrix_read();

	cout << "Input matrix:\n";
	for (int i = 0; i < N; i++) 
	{
		for (int j = 0; j < N; j++)
			cout << main_matrix[i][j] << " ";
		cout << "\n";
	}

	cout << "\nLength of work:\n";
	for (int i = 0; i < N; i++)
		cout << works[i] << " ";
	cout << "\n\n";


	bool cycles = cycle_check();
	if (!cycles)
	{
		cout << "Cycle found.\n";
		exit(-1);
	}
	else cout << "No cycles.\n";

	int source = source_find();
	vector<int> visited_vertex(N);

	topsort(source, visited_vertex);
	reverse(sorted.begin(), sorted.end());

	cout << "Old numbering:\n";
	for (int i = 0; i < N; i++)
		cout << sorted[i] << " ";

	cout << "\nNew numbering:\n";
	for (int i = 0; i < N; i++)
		cout << i << " ";
	cout << "\n";

	transpos();

	cout << "\nNew matrix:\n";
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
			cout << main_matrix[i][j] << " ";
		cout << "\n";
	}

	cout << "\nNew length:\n";
	for (int i = 0; i < N; i++)
		cout << works[i] << " ";
	cout << "\n\n";

	crit_way_find();
}

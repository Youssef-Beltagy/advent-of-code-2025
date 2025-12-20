#include "problem_solutions.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <utility>

using namespace std;

vector<string> d4parse_input()
{
    ifstream input("input/d4.txt");
    vector<string> matrix;
    string line;

    while (input >> line)
    {
        matrix.push_back(line);
    }

    return matrix;
}

int d4helper(vector<string>& matrix)
{
    vector<pair<int, int>> directions = {
        {0, 1},
        {0, -1},

        {1, 0},
        {1, 1},
        {1, -1},

        {-1, 0},
        {-1, 1},
        {-1, -1}};

    int ans = 0;

    for (int r = 0, R = matrix.size(); r < R; r++)
    {
        for (int c = 0, C = matrix[0].size(); c < C; c++)
        {

            if (matrix[r][c] != '@')
                continue;

            int surrounding_rolls = 0;

            for (pair<int, int> direction : directions)
            {
                int cur_r = r + direction.first, cur_c = c + direction.second;
                if (cur_r < 0 || cur_r >= R || cur_c < 0 || cur_c >= C)
                    continue;
                if (matrix[cur_r][cur_c] == '@' || matrix[cur_r][cur_c] == 'p')
                    surrounding_rolls++;
            }

            if (surrounding_rolls < 4)
            {
                matrix[r][c] = 'p'; // A placeholder to represent a roll that we can remove but hasn't been removed yet.
                ans++;
            }
        }
    }

    for (int r = 0, R = matrix.size(); r < R; r++)
    {
        for (int c = 0, C = matrix[0].size(); c < C; c++)
        {
            if (matrix[r][c] == 'p') matrix[r][c] = 'X'; // mark the roll as removed
        }
    }

    return ans;
}

void d4p1()
{
    vector<string> matrix = d4parse_input();

    int ans = d4helper(matrix);

    cout << "Part 1: " << ans << endl;
}

void d4p2()
{
    // Either do it iteratively
    // Or, try being smart...
    // I care about my time. So the easiest way it is.

    vector<string> matrix = d4parse_input();

    int ans = 0, cur_ans = 0;

    do {
        cur_ans = d4helper(matrix);

        ans += cur_ans;

    } while (cur_ans > 0);

    cout << "Part 2: " << ans << endl;
}
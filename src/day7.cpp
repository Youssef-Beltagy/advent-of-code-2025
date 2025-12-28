#include "problem_solutions.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <numeric>

using namespace std;

void d7p1()
{
    ifstream input("input/d7.txt");

    string line;
    vector<string> matrix;

    while (getline(input, line))
    {
        matrix.push_back(line);
    }

    unsigned long long ans = 0;

    for (int r = 1, R = matrix.size(); r < R; r++)
    {
        for (int c = 0, C = matrix[r].size(); c < C; c++)
        {
            if (matrix[r - 1][c] != 'S')
                continue;

            if (matrix[r][c] != '^')
            {
                // '.' or 'S'
                // Fall through
                matrix[r][c] = 'S';
                continue;
            }

            // '^'

            // not completely correct
            // Doesn't consider cases where splitters are adjacent or at the edte
            // S..
            // ^^.

            ans++;

            if (c > 0 && matrix[r][c - 1] != '^')
                matrix[r][c - 1] = 'S';
            if (c < C - 1 && matrix[r][c + 1] != '^')
                matrix[r][c + 1] = 'S';
        }
    }

    cout << "Part 1: " << ans << endl;
}

void add_timelines(string &cur, const string &add)
{
    unsigned long long curNum = cur == "." ? 0 : stoull(cur);
    cur = to_string(curNum + stoull(add));
}

void d7p2()
{
    ifstream input("input/d7.txt");
    string line;

    vector<vector<string>> matrix;

    while (getline(input, line))
    {
        matrix.push_back({});
        for (const char c : line)
        {
            string s = c == 'S' ? "1" : string{c};
            matrix.back().push_back(s);
        }
    }

    for (int r = 1, R = matrix.size(); r < R; r++)
    {
        for (int c = 0, C = matrix[r].size(); c < C; c++)
        {
            // If row above doesn't have a particle, skip
            if (matrix[r - 1][c] == "." || matrix[r - 1][c] == "^")
                continue;

            // If this is not a splitter (meaning it is num or "."), add the timelines that fall through
            if (matrix[r][c] != "^")
            {
                // Falling through doesn't affect the number of possible timelines that can reach this coordinate
                add_timelines(matrix[r][c], matrix[r - 1][c]);
                continue;
            }

            // '^'

            // not completely correct
            // Doesn't consider cases where splitters are adjacent or at the edte
            // S..
            // ^^.

            if (c > 0 && matrix[r][c - 1] != "^")
                add_timelines(matrix[r][c - 1], matrix[r - 1][c]);
            if (c < C - 1 && matrix[r][c + 1] != "^")
                add_timelines(matrix[r][c + 1], matrix[r - 1][c]);
        }
    }


    // Each numeric entry represent all the possible ways to reach current coordinate.
    // Sum of all entries represents all the possible ways to reach the bottom.
    // Sadists: https://stackoverflow.com/questions/46691506/summation-of-vector-containing-long-long-int-using-accumulation
    unsigned long long ans = accumulate(matrix.back().begin(), matrix.back().end(), 0ULL, [](unsigned long long acc, const string& cur){
        if (cur != "^" && cur != ".") acc += stoull(cur);
        return acc;
    });

    // unsigned long long ans2 = 0;
    // // for (const vector<string>& vec : matrix) 
    // for (const string& cur: matrix.back()) if (cur != "^" && cur != ".") ans2 += stoull(cur);

    cout << "Part 2: " << ans << endl;
}

#include "problem_solutions.h"
#include <fstream>
#include <iostream>
#include <cmath>
#include <vector>
#include <sstream>

using namespace std;

void d6p1()
{

    vector<string> lines;

    {
        ifstream input("input/d6.txt");
        string line;
        while (getline(input, line))
        {
            lines.push_back(line);
        }
    }

    vector<vector<unsigned long long>> matrix;

    for (int r = 0, R = lines.size() - 1; r < R; r++)
    {
        matrix.push_back({});

        stringstream ss(lines[r]);
        unsigned long long cur_num;
        while (ss >> cur_num)
            matrix.back().push_back(cur_num);
    }

    vector<char> operations;

    {
        stringstream ss(lines.back());
        char operation;
        while (ss >> operation)
            operations.push_back(operation);
    }

    unsigned long long ans = 0;

    for (int c = 0, C = operations.size(); c < C; c++)
    {

        const char operation = operations[c];

        // Either * or +
        unsigned long long cur_acc = operation == '*' ? 1 : 0;

        for (int r = 0, R = matrix.size(); r < R; r++)
        {
            if (operation == '*')
                cur_acc *= matrix[r][c];
            else
                cur_acc += matrix[r][c];
        }

        ans += cur_acc;
    }

    cout << "Part 1: " << ans << endl;
}

void d6p2()
{
    vector<string> lines;

    {
        ifstream input("input/d6.txt");
        string line;
        while (getline(input, line))
        {
            lines.push_back(line);
        }
    }
    
    unsigned long long ans = 0;
    size_t st = 0;
    while(st < lines.back().size()){

        const char operation = lines.back()[st];
        unsigned long long cur_acc = operation == '*' ? 1 : 0;

        size_t end = lines.back().find_first_not_of(' ', st + 1); // fix for end
        end = min(end, lines.back().size() + 1);

        for(int c = st, C = end - 1; c < C; c++){
            unsigned long long cur_num = 0;
            
            for (int r = 0, R = lines.size() - 1; r < R; r++){
                if (lines[r][c] != ' ') cur_num = cur_num * 10 + lines[r][c] - '0';
            }

            if (operation == '*')
                cur_acc *= cur_num;
            else
                cur_acc += cur_num;
        }

        st = end;
        ans += cur_acc;

    }

        cout << "Part 2: " << ans << endl;


}
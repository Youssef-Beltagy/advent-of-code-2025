#include "problem_solutions.h"
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

unsigned long long d3helper(const int num_batteries_to_enable)
{
    ifstream input("input/d3.txt");
    string line;

    unsigned long long ans = 0;

    while (input >> line)
    {
        vector<int> st = {line[0] - '0'};

        for (int i = 1; i < line.size(); i++)
        {
            int cur = line[i] - '0';

            // If the current jolt is bigger than the previously chosen jolt
            // and there are enough batteries to replace the previous ones,
            // remove the old batteries
            while (cur > st.back() && !st.empty() && line.size() - i > num_batteries_to_enable - st.size())
            {
                st.pop_back();
            }

            if (st.size() < num_batteries_to_enable)
                st.push_back(cur);
        }

        unsigned long long cur_val = 0;

        for (int i : st)
        {
            cur_val *= 10;
            cur_val += i;
        }

        ans += cur_val;
    }

    return ans;
}

void d3p1()
{
    cout << "Part 1: " << d3helper(2) << endl;
}

void d3p2()
{
    cout << "Part 2: " << d3helper(12) << endl;
}
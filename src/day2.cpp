#include "problem_solutions.h"
#include <fstream>
#include <iostream>
#include <cmath>

using namespace std;

void d2p1()
{
    ifstream input("input/d2.txt");
    string s;

    unsigned long long ans = 0;

    while (getline(input, s, ','))
    {
        string num1 = s.substr(0, s.find('-'));
        string num2 = s.substr(s.find('-') + 1, s.size());

        for (unsigned long long cur_num = stoull(num1), num2_int = stoull(num2); cur_num <= num2_int; cur_num++)
        {
            string cur_num_string = to_string(cur_num);
            if (
                cur_num_string.size() % 2 == 0 && cur_num_string.substr(0, cur_num_string.size() / 2) == cur_num_string.substr(cur_num_string.size() / 2, cur_num_string.size()))
                ans += cur_num;
        }
    }

    cout << "Part 1: " << ans << endl;
}

unsigned long long d2p2_eval_num(unsigned long long cur_num)
{
    string cur_num_string = to_string(cur_num);

    for (int cur_size = 1; cur_size <= cur_num_string.size() / 2; cur_size++)
    {
        if (cur_num_string.size() % cur_size != 0)
            continue;
        string prefix = cur_num_string.substr(0, cur_size);

        for (int pos = cur_size; pos + cur_size <= cur_num_string.size(); pos += cur_size)
        {
            if (cur_num_string.substr(pos, cur_size) != prefix)
                break;

            if ((pos + cur_size) == cur_num_string.size())
            {
                return cur_num;
            }
        }
    }

    return 0;
}

void d2p2()
{

    ifstream input("input/d2.txt");
    string s;

    unsigned long long ans = 0;

    while (getline(input, s, ','))
    {
        string num1 = s.substr(0, s.find('-'));
        string num2 = s.substr(s.find('-') + 1, s.size());

        for (unsigned long long cur_num = stoull(num1), num2_int = stoull(num2); cur_num <= num2_int; cur_num++)
        {
            ans += d2p2_eval_num(cur_num);
        }
    }

    cout << "Part 2: " << ans << endl;
}
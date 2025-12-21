#include "problem_solutions.h"
#include <fstream>
#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <limits>

using namespace std;

struct D5Range
{
    unsigned long long st;
    unsigned long long end;
    D5Range(const string &line)
    {
        st = stoull(line.substr(0, line.find('-')));
        end = stoull(line.substr(line.find('-') + 1, line.size()));

    }

    friend bool operator==(const D5Range &r1, const D5Range &r2);
    friend bool operator<(const D5Range &r1, const D5Range &r2);

    bool contains(unsigned long long id) const
    {
        return st <= id && id <= end;
    }

    // use size_t in the future to avoid silly misses like what happened with part 2.
    unsigned long long size() const
    {
        return end - st + 1;
    }
};

bool operator==(const D5Range &r1, const D5Range &r2)
{
    return r1.st == r2.st && r1.end == r2.end;
}

bool operator<(const D5Range &r1, const D5Range &r2)
{
    if (r1.st != r2.st)
        return r1.st < r2.st;

    return r1.end < r2.end;
}

vector<D5Range> d5helper(ifstream &input)
{
    // Read the ranges
    vector<D5Range> fresh_ranges{};
    string line;
    while (getline(input, line))
    {
        if (line == "")
            break;

        fresh_ranges.push_back(D5Range(line));
    }

    sort(fresh_ranges.begin(), fresh_ranges.end());

    // Merge the ranges
    for (int i = 0; i + 1 < fresh_ranges.size();)
    {
        D5Range &curRange = fresh_ranges[i], &nextRange = fresh_ranges[i + 1];

        // Can't merge this.
        if (curRange.end + 1 < nextRange.st) // +1 because the ranges are inclusive
        {
            i++;
            continue;
        }

        curRange.end = max(curRange.end, nextRange.end);

        fresh_ranges.erase(fresh_ranges.begin() + i + 1);
    }

    return fresh_ranges;
}

void d5p1()
{
    ifstream input("input/d5.txt");
    vector<D5Range> fresh_ranges = d5helper(input);

    // Two options,
    // Binary search
    // O(N) search and save time to live my life... I value my life.
    // ... On that note, why did I bother merging the ranges? It does make this section easier.
    // ... Fine, did both. Does that mean I don't value my life?
    int ans = 0;
    string line;

    while (input >> line)
    {
        unsigned long long id = stoull(line);
        auto it = lower_bound(
            fresh_ranges.begin(),
            fresh_ranges.end(),
            id,
            [](const D5Range &lhs, const unsigned long long id) -> bool
            { return lhs.end < id; });

        if (it != fresh_ranges.end() && it->contains(id))
            ans++;

        // for (const D5Range& range : fresh_ranges){
        //     if (range.contains(id)){
        //         ans++;
        //     }
        // }
    }

    cout << "Part 1: " << ans << endl;
}

void d5p2()
{
    ifstream input("input/d5.txt");
    vector<D5Range> fresh_ranges = d5helper(input);

    unsigned long long ans = 0;

    for (const D5Range &range : fresh_ranges)
    {

        ans += range.size();
    }

    cout << "Part 2: " << ans << endl;
}

#include "problem_solutions.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <numeric>
#include <queue>
#include <unordered_map>

using namespace std;

/**
 * Part 1:
 * For merging the lights, union-find is appropriate.
 *
 * For finding the distances, I'm not sure there is an "elegant" solution.
 * I'll start with bruteforcing (while keeping a maxheap).
 * I opimitize, find, or come up with a better algorithm if needed.
 *
 *
 *
 * Part 2:
 * Approach 1: resuse implementation from part 1.
 * Modify so it uses minheap of distances.
 * Update unite so it returns true if it merged two sets.
 * Keep merging stuff (and ignoring failed merged) till one set has all the coordinates
 * This works, is a bit inefficient but lets me get this over with and live my life.
 * 
 * Ahhh, nothing beats getting stuff done with already existing code!
 * I don't think this is significantly different from Prim's MST anyway.
 * The only relative inefficiecy is that the minHeap of distances starts completely full.
 * Otherwise, almost everything is identical.
 * But that's just O(Log(number of edges)) so it isn't the end of the world
 * 
 * And near the end of Prim's MST, the minHeap woudl be full anyway.
 *
 * Approach 2:
 * Prim's Minimum spanning tree: https://www.geeksforgeeks.org/dsa/prims-minimum-spanning-tree-mst-greedy-algo-5/
 * 
 *
 */

struct D8Coordinate
{
    int64_t x, y, z, id;
    D8Coordinate(int64_t x, int64_t y, int64_t z, int64_t id) : x(x), y(y), z(z), id(id) {}
};

int64_t euclidean_squared(const D8Coordinate &from, const D8Coordinate &to)
{
    int64_t abs_x = abs(to.x - from.x), abs_y = abs(to.y - from.y), abs_z = abs(to.z - from.z);
    return abs_x * abs_x + abs_y * abs_y + abs_z * abs_z;
}

struct D8Distance
{

    int64_t from, to;
    int64_t val;

    D8Distance(const D8Coordinate &from, const D8Coordinate &to) : from(from.id), to(to.id), val(euclidean_squared(from, to)) {};
};

bool operator<(const D8Distance &a, const D8Distance &b)
{
    return a.val < b.val;
}

bool operator>(const D8Distance &a, const D8Distance &b)
{
    return a.val > b.val;
}

bool operator==(const D8Distance &a, const D8Distance &b)
{
    return a.val == b.val;
}

vector<D8Coordinate> parse_input(const string &file_name)
{
    // Parse input
    ifstream input(file_name);
    string line;

    int64_t x, y, z, id = 0;

    vector<D8Coordinate> coordinates;

    while (getline(input, line))
    {
        stringstream ss(line);
        string s;

        getline(ss, s, ',');
        x = stoull(s);
        getline(ss, s, ',');
        y = stoull(s);
        getline(ss, s, ',');
        z = stoull(s);

        coordinates.push_back({x, y, z, id});
        id++;
    }
    return coordinates;
}

priority_queue<D8Distance, vector<D8Distance>> find_min_distances(const vector<D8Coordinate> &coordinates, size_t size)
{
    // Find smallest distances

    // maxHeap of the distances;
    priority_queue<D8Distance, vector<D8Distance>> pq_distances;

    for (const D8Coordinate &from : coordinates)
    {
        for (const D8Coordinate &to : coordinates)
        {
            if (&from == &to)
                break;

            pq_distances.push({from, to});

            if (pq_distances.size() > size)
                pq_distances.pop();
        }
    }

    return pq_distances;
}

// UnionFind
// With Path Compression
// Join using size
struct D8UnionFind
{
    vector<int64_t> roots;
    vector<int64_t> sizes;

    D8UnionFind(const vector<D8Coordinate> &coordinates)
    {
        roots.resize(coordinates.size());
        sizes.resize(coordinates.size());

        for (const D8Coordinate &coor : coordinates)
        {
            roots[coor.id] = coor.id;
            sizes[coor.id] = 1;
        }
    }

    int64_t find(int64_t id)
    {
        vector<int64_t> st;

        while (id != roots[id])
        {
            st.push_back(id);
            id = roots[id];
        }

        while (!st.empty())
        {
            roots[st.back()] = id;
            st.pop_back();
        }

        return id;
    }

    bool unite(int64_t id1, int64_t id2)
    {
        id1 = find(id1);
        id2 = find(id2);

        // Already the same set, skip
        if (id1 == id2)
            return false;

        // Unite the sets
        if (sizes[id1] <= sizes[id2])
        {
            roots[id1] = id2;
            sizes[id2] += sizes[id1];
            sizes[id1] = 0;
        }
        else
        {
            roots[id2] = id1;
            sizes[id1] += sizes[id2];
            sizes[id2] = 0;
        }

        return true;
    }
};

void d8p1()
{

    vector<D8Coordinate> coordinates = parse_input("input/d8.txt");

    // maxHeap of the distances;
    priority_queue<D8Distance, vector<D8Distance>> pq_distances = find_min_distances(coordinates, 1000);

    D8UnionFind sets(coordinates);

    // Union Set
    while (!pq_distances.empty())
    {
        D8Distance dist = pq_distances.top();
        pq_distances.pop();

        // cout << "UNITE: " << dist.from << " -> " << dist.to << " -- ";

        // cout << "SUM (before): " << accumulate(sets.sizes.begin(), sets.sizes.end(), 0LL);

        sets.unite(dist.from, dist.to);

        // cout << " SUM (after): " << accumulate(sets.sizes.begin(), sets.sizes.end(), 0LL) << endl;
    }

    priority_queue<int64_t, vector<int64_t>> pq_sizes(sets.sizes.begin(), sets.sizes.end());

    uint64_t ans = pq_sizes.top();
    pq_sizes.pop();
    ans *= pq_sizes.top();
    pq_sizes.pop();
    ans *= pq_sizes.top();

    cout << "Part 1: " << ans << endl;
}

void d8p2()
{

    vector<D8Coordinate> coordinates = parse_input("input/d8.txt");

    // minHeap of the distances;
    priority_queue<D8Distance, vector<D8Distance>, greater<D8Distance>> pq_distances;

    for (const D8Coordinate &from : coordinates)
    {
        for (const D8Coordinate &to : coordinates)
        {
            if (&from == &to)
                break;

            pq_distances.push({from, to});
        }
    }

    D8UnionFind sets(coordinates);

    unsigned long long ans = 0;

    // Union Set
    while (ans == 0)
    {
        // Get the smallest distance
        D8Distance dist = pq_distances.top();
        pq_distances.pop();

        bool union_succeeded = sets.unite(dist.from, dist.to);

        if(union_succeeded && sets.sizes[sets.find(dist.from)] == coordinates.size()){
            ans = coordinates[dist.from].x * coordinates[dist.to].x;
        }

    }

    cout << "Part 2: " << ans << endl;
}
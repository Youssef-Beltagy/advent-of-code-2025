#include "problem_solutions.h"
#include <fstream>
#include <iostream>
#include <cmath>
#include <vector>
#include <queue>
#include <limits>
#include <list>
#include <utility>

using namespace std;

/**
 * Part 1:
 * Approach 1: Brute force.
 * Find all possible rectangles and get the biggest with heap.
 *
 * Approach 2:
 * Be smart about it and only pair coordinates that are far from each other.
 *
 *
 *
 *
 * Part 2:
 * Approach 1:
 * I actually recreate the matrix and solve the problem with it.
 * The input doesn't look too big.
 * This may work.
 * In fact, I can make a smaller rectangle by utilizing how the minimum is not 0.
 *
 * To find what is inside/vs outside, I can use
 * 1. a recursive map search that
 * 2. Or an iterative thing where I mark non-allowed coordinates starting with the edge.
 * 3. If I assume there is always a circle, I choose a left-most coordinate.
 *  Then anything to its left (or connected to it) is bad. Then recursively connect the rest.
 * 4. Or choose a point that I suspect and draw a line to the edge. See how often I cross an R/G line.
 *  If even, ouside. Else, inside the circle.
 *
 *
 * Approach 2:
 * TBH, now that I made a matrix (D9Matrix), I can probably use it along with Dynamic Programming to build up the biggest rectangle.
 * For example,
 * I can start iterating from the top left to the bottom right.
 * For every row,
 *   For every col,
 *      If current is R/G, current score = left cell score + 1.
 *
 * For every row
 *   For every col
 *      If coordinate is not red, skip.
 *      For every row above,
 *          Cal calculate area by track heigh/width
 *   Record area, keep track of best area
 *
 * I can probably improve this approach and do it.
 * I can use it as a fallback if approach 1 doesn't work.
 *
 * Ohh, cool, chatgpt has the same idea
 *
 *
 * Matrix is 100_000 by 100_000
 * There are 500 coordinates
 *
 * Approach 3:
 * Making the matrix is too slow.
 * Especially filling in the greens.
 * I need to optimize this.
 * Doing it with the approrach of pencil crossing from left to right is probably good enough
 *
 *
 *
 *
 *
 *
 * I can make a sparse matrix if I need to optimize space/time more.
 * I'm thinking of doing it by creating a list of ranges. That will at least compact the columns.
 * If I need even more optimization, then a list of list of ranges to compact both rows and columns.
 *
 * ---
 * Some references: // https://stackoverflow.com/questions/69275956/efficient-algorithm-to-find-the-largest-rectangle-from-a-set-of-points
 */

struct D9Coordinate
{
    uint64_t r, c;
    D9Coordinate(uint64_t r, uint64_t c) : r(r), c(c) {}
};

void d9p1()
{
    ifstream input("input/d9.txt");
    string line;

    vector<D9Coordinate> coordinates;

    while (getline(input, line))
    {
        string col = line.substr(0, line.find(','));
        string row = line.substr(line.find(',') + 1, line.size());

        coordinates.push_back({stoull(row), stoull(col)});
    }

    // priority_queue<uint64_t, vector<uint64_t>> pq_rect;

    uint64_t max_area = 0;

    for (const D9Coordinate &t : coordinates)
        for (const D9Coordinate &b : coordinates)
        {
            // Avoid considering the same coordinates twice
            if (t.r > b.r || (t.r == b.r && t.c > b.c))
                continue;

            uint64_t height = b.r - t.r + 1;
            uint64_t width = 1 + (b.c > t.c ? b.c - t.c : t.c - b.c);

            // pq_rect.push(height * width);

            max_area = max(max_area, height * width);
        }

    cout << "Part 1: " << max_area << endl;
}

struct D9Rect
{
    uint64_t area;
    D9Coordinate t;
    D9Coordinate b;

    D9Rect(const D9Coordinate &t, const D9Coordinate &b) : t(t), b(b)
    {
        uint64_t height = b.r - t.r + 1;
        uint64_t width = 1 + (b.c > t.c ? b.c - t.c : t.c - b.c);
        area = height * width;
    }
};

bool operator<(const D9Rect &a, const D9Rect &b)
{
    return a.area < b.area;
}

struct D9Matrix
{

    D9Coordinate min_coor, max_coor;
    vector<vector<char>> matrix;

    D9Matrix(const D9Coordinate &min_coor, const D9Coordinate &max_coor, const vector<D9Coordinate> &coordinates) : min_coor(min_coor), max_coor(max_coor)
    {

        matrix = vector<vector<char>>(max_coor.r - min_coor.r + 1, vector<char>(max_coor.c - min_coor.c + 1, '.'));

        // cout << coordinates.size() << endl; // 496
        // cout << matrix.size() << ", " << matrix.front().size() << endl; // 96428, 96754

        // 500 * 100_000 * 100_000 = 5 trillion.
        for (size_t i = 0; i < coordinates.size(); i++)
        {
            // Get the previous coordinate assuming the coordinates are in a circle
            D9Coordinate prev = coordinates[(i - 1 + coordinates.size()) % coordinates.size()];
            prev = {prev.r - min_coor.r, prev.c - min_coor.c};

            D9Coordinate cur = coordinates[i];
            cur = {cur.r - min_coor.r, cur.c - min_coor.c};

            size_t r = min(prev.r, cur.r), R = max(prev.r, cur.r);
            size_t c = min(prev.c, cur.c), C = max(prev.c, cur.c);
            if (r != R)
            { // Same column different rows
                for (; r <= R; r++)
                {
                    matrix[r][c] = 'X';
                }
            }
            else
            { // Same row different columns

                // Avoid spamming X when filling columns because Xs are used to later fill in the greens
                matrix[r][c] = 'X';
                c++;
                for (; c <= C; c++)
                {
                    matrix[r][c] = 'G';
                }
            }
        }

        // print();

        cout << "Filling in all the greens" << endl;

        for (size_t r = 0, R = matrix.size(); r < R; r++)
        {

            /*
             * Since this is a closed loop, if you trace a line from one side of the matrix to the other,
             * You will notice that "inside" parts of the line cross an odd number of edges.
             * "Outside" parts of the line cross an even number of edges.
             */
            char prev = '.';
            for (size_t c = 0, C = matrix.front().size(); c < C; c++)
            {

                if (prev == '.' && matrix[r][c] == 'X')
                { // Crossing an edge. start coloring in green.
                    prev = 'X';
                }
                else if (prev == 'X' && matrix[r][c] == '.')
                { // Color in green
                    matrix[r][c] = 'X';
                }
                else if (prev == 'X' && matrix[r][c] == 'X')
                { // Crossing an edge.  Stop coloring in green.
                    prev = '.';
                } // Remaining case: prev == '.' && matrix[r][c] == '.' --> just don't color in green

                if (matrix[r][c] == 'G')
                {
                    // Now undo the edge condition added before.
                    matrix[r][c] = 'X';
                }
            }
        }

        // print();
    }

    void print()
    {
        for (const vector<char> &row : matrix)
        {
            for (const char c : row)
            {
                cout << c;
            }
            cout << endl;
        }
    }

    bool is_valid_rect(const D9Rect rect) const
    {
        D9Coordinate t = {rect.t.r - min_coor.r, rect.t.c - min_coor.c};
        D9Coordinate b = {rect.b.r - min_coor.r, rect.b.c - min_coor.c};

        for (size_t r = t.r, R = b.r; r <= R; r++)
            for (size_t c = min(t.c, b.c), C = max(t.c, b.c); c <= C; c++)
            {
                if (matrix[r][c] != 'X')
                    return false;
            }

        return true;
    }
};

void d9p2()
{
    ifstream input("input/d9.txt");
    string line;

    vector<D9Coordinate> coordinates;
    D9Coordinate min_coor = {numeric_limits<uint64_t>::max(), numeric_limits<uint64_t>::max()}, max_coor = {0, 0};

    while (getline(input, line))
    {
        uint64_t col = stoull(line.substr(0, line.find(',')));
        uint64_t row = stoull(line.substr(line.find(',') + 1, line.size()));

        min_coor = {min(min_coor.r, row), min(min_coor.c, col)};
        max_coor = {max(max_coor.r, row), max(max_coor.c, col)};

        coordinates.push_back({row, col});
    }

    D9Matrix matrix(min_coor, max_coor, coordinates);

    cout << "Building Rects" << endl;
    priority_queue<D9Rect, vector<D9Rect>> pq_rect;

    for (const D9Coordinate &t : coordinates)
        for (const D9Coordinate &b : coordinates)
        {
            // Avoid considering the same coordinates twice
            if (t.r > b.r || (t.r == b.r && t.c > b.c))
                continue;

            pq_rect.push(D9Rect(t, b));
        }

    cout << "Finding an answer" << endl;
    D9Rect rect = pq_rect.top();
    while (!pq_rect.empty())
    {
        if (matrix.is_valid_rect(rect))
            break;

        pq_rect.pop();
        rect = pq_rect.top();
    }

    cout << "Part 2: " << rect.area << endl;
}

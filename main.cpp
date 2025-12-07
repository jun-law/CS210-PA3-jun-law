#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

// Directions for DFS (students must use these)
int dr[4] = {-1, 0, 1, 0};
int dc[4] = {0, 1, 0, -1};

// ----------------------------------------------------------
// DO NOT MODIFY: Maze generation
// ----------------------------------------------------------
void generateMaze(vector<vector<int>>& maze, int N, int M) {
    srand(time(0));

    for (int r = 0; r < N; r++) {
        for (int c = 0; c < M; c++) {
            int roll = rand() % 100;
            maze[r][c] = (roll < 70) ? 0 : 1;   // 0 = open, 1 = wall
        }
    }
}

// ----------------------------------------------------------
// DO NOT MODIFY: Choose a random boundary cell that is open
// ----------------------------------------------------------
pair<int,int> chooseBoundaryCell(const vector<vector<int>>& maze) {
    int N = maze.size();
    int M = maze[0].size();

    while (true) {
        int side = rand() % 4;
        int r, c;

        if (side == 0) {          // top row
            r = 0;
            c = rand() % M;
        } else if (side == 1) {   // bottom row
            r = N - 1;
            c = rand() % M;
        } else if (side == 2) {   // left column
            r = rand() % N;
            c = 0;
        } else {                  // right column
            r = rand() % N;
            c = M - 1;
        }

        if (maze[r][c] == 0) {
            return {r, c};
        }
    }
}

// ----------------------------------------------------------
// DO NOT MODIFY: Print the maze with S and E markers
// ----------------------------------------------------------
void printMaze(const vector<vector<int>>& maze,
               int ent_r, int ent_c,
               int exit_r, int exit_c)
{
    int N = maze.size();
    int M = maze[0].size();

    cout << "\nMaze:\n";
    for (int r = 0; r < N; r++) {
        for (int c = 0; c < M; c++) {
            if (r == ent_r && c == ent_c) {
                cout << "S ";
            } else if (r == exit_r && c == exit_c) {
                cout << "E ";
            } else {
                cout << maze[r][c] << " ";
            }
        }
        cout << "\n";
    }
}

// ----------------------------------------------------------
// DO NOT MODIFY: Print the reconstructed path
// Students must fill parent[][] correctly during DFS
// ----------------------------------------------------------
void printPath(pair<int,int> exitcell,
               const vector<vector<int>>& parent_r,
               const vector<vector<int>>& parent_c,
               int ent_r, int ent_c)
{
    int r = exitcell.first;
    int c = exitcell.second;

    vector<pair<int,int>> path;

    // Walk backward from exit to entrance
    while (!(r == ent_r && c == ent_c)) {
        path.push_back({r, c});
        int pr = parent_r[r][c];
        int pc = parent_c[r][c];
        r = pr;
        c = pc;
    }
    path.push_back({ent_r, ent_c});

    cout << "\nPath from entrance to exit:\n";
    for (int i = path.size() - 1; i >= 0; i--) {
        cout << "(" << path[i].first << ", " << path[i].second << ")\n";
    }
}

// ----------------------------------------------------------
// STUDENTS IMPLEMENT DFS HERE
// Add arguments, return type, and logic
// ----------------------------------------------------------
/*
 * This function performs a depth first search on the maze for a path from the entrance to the exit
 * @param r - the row index of the starting coordinates
 * @param c - the column index of the starting coordinates
 * @param maze - the maze containing 0's and 1's for open cells and walls
 * @param visited - a 2D vector to track which cell coordinates have been visited
 * @param parent_r - a 2D vector to track the row index of each parent coordinate
 * @param parent_c - a 2D vector to track the column index of each parent coordinate
 * @param exit_r - the row index of the exit coordinate
 * @param exit_c - the column index of the exit coordinate
 * @return true if a path from the entrance to the exit exists
 */
bool dfs(int r, int c,
         const vector<vector<int>>& maze,
         vector<vector<bool>>& visited,
         vector<vector<int>>& parent_r,
         vector<vector<int>>& parent_c,
         int exit_r, int exit_c) {

    visited[r][c] = true;

    // move in all 4 directions
    for (int i = 0; i < 4; i++) {
        int temp_r = r + dr[i];
        int temp_c = c + dc[i];

        // check boundaries
        if (temp_r < 0 || temp_c < 0 || temp_r > maze.size()-1 || temp_c > maze[0].size()-1) {
            // cout << "(" << temp_r << ", " << temp_c << ") is OUT OF BOUNDS" << endl;
        }
        else {
            // base case
            if (temp_r == exit_r && temp_c == exit_c) {
                visited[temp_r][temp_c] = true;

                // update parent coordinates
                parent_r[temp_r][temp_c] = r;
                parent_c[temp_r][temp_c] = c;

                return true;
            }
            else {
                // available cell
                if (maze[temp_r][temp_c] == 0 && visited[temp_r][temp_c] == false) {
                    visited[temp_r][temp_c] = true;
                    cout << "(" << temp_r << ", " << temp_c << ")\n";

                    // update parent coordinates
                    parent_r[temp_r][temp_c] = r;
                    parent_c[temp_r][temp_c] = c;

                    // recurse
                    if (dfs(temp_r, temp_c, maze, visited, parent_r, parent_c, exit_r, exit_c)) {
                        return true;
                    }
                }
            }
        }
    }
    // no other directions available
    cout << "dead end found" << endl;
    return false;
}

// ----------------------------------------------------------
// MAIN PROGRAM (students add DFS calls and logic)
// ----------------------------------------------------------
int main() {
    int N, M;

    cout << "Enter maze dimensions N M: ";
    cin >> N >> M;

    vector<vector<int>> maze(N, vector<int>(M));
    generateMaze(maze, N, M);

    // Pick entrance and exit
    pair<int,int> entrance = chooseBoundaryCell(maze);
    pair<int,int> exitcell = chooseBoundaryCell(maze);

    while (exitcell == entrance) {
        exitcell = chooseBoundaryCell(maze);
    }

    int ent_r = entrance.first;
    int ent_c = entrance.second;
    int exit_r = exitcell.first;
    int exit_c = exitcell.second;

    // Display the maze
    printMaze(maze, ent_r, ent_c, exit_r, exit_c);

    // Students must use these
    vector<vector<bool>> visited(N, vector<bool>(M, false));
    vector<vector<int>> parent_r(N, vector<int>(M, -1));
    vector<vector<int>> parent_c(N, vector<int>(M, -1));

    // ------------------------------------------------------
    // STUDENT WORK:
    // Call your DFS, track visited, and fill parent_r and parent_c
    // ------------------------------------------------------
    bool found = dfs(ent_r, ent_c, maze, visited, parent_r, parent_c, exit_r, exit_c);

    // ------------------------------------------------------
    // STUDENT WORK:
    // If found, print the path
    // ------------------------------------------------------
      if (found) {
          printPath(exitcell, parent_r, parent_c, ent_r, ent_c);
      } else {
         cout << "\nNo path exists.\n";
      }

     return 0;
}
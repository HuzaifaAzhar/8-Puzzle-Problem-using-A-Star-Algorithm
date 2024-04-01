#include <iostream>
#include <queue>
#include <map>
#include <cmath>

using namespace std;

struct PuzzleState {
    int board[3][3];
    int cost;
    int heuristic;
    int emptyRow;
    int emptyCol;
};

int calculateHeuristic(const int board[3][3]) {
    int heuristic = 0;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            int value = board[i][j];
            if (value != 0) {
                int goalRow = (value - 1) / 3;
                int goalCol = (value - 1) % 3;
                heuristic += abs(i - goalRow) + abs(j - goalCol);
            }
        }
    }
    return heuristic;
}

bool isGoalState(const PuzzleState& state) {
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (state.board[i][j] != i * 3 + j + 1 && (i != 2 || j != 2)) {
                return false;
            }
        }
    }
    return true;
}

void printPuzzleState(const PuzzleState& state) {
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            cout << state.board[i][j] << " ";
        }
        cout << endl;
    }
    cout << "Cost: " << state.cost << ", Heuristic: " << state.heuristic << endl;
}

struct ComparePuzzleState {
    bool operator()(const PuzzleState& a, const PuzzleState& b) const {
        return (a.cost + a.heuristic) > (b.cost + b.heuristic);
    }
};

void solvePuzzleAStar(const int initialBoard[3][3]) {
    priority_queue<PuzzleState, vector<PuzzleState>, ComparePuzzleState> open;

    map<string, int> explored;

    PuzzleState initialState;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            initialState.board[i][j] = initialBoard[i][j];
            if (initialBoard[i][j] == 0) {
                initialState.emptyRow = i;
                initialState.emptyCol = j;
            }
        }
    }
    initialState.cost = 0;
    initialState.heuristic = calculateHeuristic(initialState.board);

    open.push(initialState);

    while (!open.empty()) {
        PuzzleState current = open.top();
        open.pop();

        if (isGoalState(current)) {
            cout << "Goal state found!" << endl;
            printPuzzleState(current);
            return;
        }

        cout << "Current State:" << endl;
        printPuzzleState(current);
        cout << endl;


        int moves[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
        for (const auto& move : moves) {
            int newRow = current.emptyRow + move[0];
            int newCol = current.emptyCol + move[1];

            if (newRow >= 0 && newRow < 3 && newCol >= 0 && newCol < 3) {
                PuzzleState newState = current;
                swap(newState.board[current.emptyRow][current.emptyCol], newState.board[newRow][newCol]);
                newState.emptyRow = newRow;
                newState.emptyCol = newCol;
                newState.cost++;

                newState.heuristic = calculateHeuristic(newState.board);

                string newStateStr;
                for (int i = 0; i < 3; ++i) {
                    for (int j = 0; j < 3; ++j) {
                        newStateStr += to_string(newState.board[i][j]);
                    }
                }

                if (explored.find(newStateStr) == explored.end() || explored[newStateStr] > newState.cost) {
                    open.push(newState);
                    explored[newStateStr] = newState.cost;
                }
            }
        }
    }

    cout << "No solution found." << endl;
}

int main() {
    const int initialBoard[3][3] = {
        {1, 2, 3},
        {4, 0, 6},
        {7, 5, 8}
    };

    solvePuzzleAStar(initialBoard);

    return 0;
}

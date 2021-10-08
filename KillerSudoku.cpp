#include <iostream>
#include "iterator"
#include "thread"
#include <utility>
#include <vector>
#include <chrono>

using namespace std::chrono;
using namespace std;

class KillerSudoku{
public:
    /**
     * @brief Class constructor
     * @param areas
     * @param areas_sum
     */

    KillerSudoku(vector<vector<int>> areas, vector<int> areas_sum){
        this->areas = move(areas);
        this->areas_sum = move(areas_sum);
    }

    void printSudoku(){
        for(auto & i : board){
            for (int j = 0; j < board[0].size(); ++j) cout << i[j] << " ";
            cout << endl;
        }
        cout << endl;
    }

    /**
     *
     * @param i
     * @param j
     * @param area_id
     * @param shown
     * @param sum
     */

    void getSum(int i, int j, int area_id, bool shown[9][9], int &sum){

        for (int x = i - 1; x <= i + 1; ++x) {
            for (int y = j - 1; y <= j + 1; ++y) {

                // Only directly beside cells are checked given that areas cannot be diagonal
                //                    0                  0 0 0
                // with condition:  0 0 0    without:    0 0 0
                //                    0                  0 0 0
                if(x == i || y == j)
                    // Checking if cell is valid and hasn't been already considered
                    if (0 <= x && x < 9 && 0 <= y && y < 9 && !shown[x][y]) {
                        // Other areas' cells are not considered
                        if (areas[x][y] != area_id)continue;
                        // Case area is not complete
                        if (board[x][y] == 0) {
                            sum = 0;
                            return;
                        }

                        shown[x][y] = true;
                        sum += board[x][y];
                        getSum(x,y,area_id,shown,sum);
                    }
            }
        }
    }

    /**
     *
     * @param i row
     * @param j col
     * @param val number from 1 to 9
     * @return whether val can be put in position i,j
     */

    bool check(int i, int j, int val)
    {
        // Checks regular sudoku rules
        int row = i - i%3, column = j - j%3;

        for(int x=0; x<9; x++)
            if(board[x][j] == val)
                return false;
        for(int y=0; y<9; y++)
            if(board[i][y] == val)
                return false;
        for(int x=0; x<3; x++)
            for(int y=0; y<3; y++)
                if(board[row+x][column+y] == val) return false;

        // Checks Killer Sudoku rules
        int area_id = areas[i][j];
        int aux = board[i][j];
        board[i][j] = val;
        bool shown[9][9] = {false};
        int sum = 0;
        getSum(i, j, area_id, shown, sum);

        // Check whether area is complete and if it is, if sum is correct
        if(sum != 0 && sum != areas_sum[area_id]) {
            board[i][j] = aux;
            return false;
        }

        return true;
    }

    bool solveSudokuFW(int i, int j)
    {
        static int count = 0;

        if(i==9) {
            cout << endl << "Combinations tried: " << count << endl;
            return true;
        }
        if(j==9) return solveSudokuFW(i + 1, 0);
        if(board[i][j] != 0) return solveSudokuFW(i, j + 1);

        for (int val = 1; val < 10; ++val) {
            if (check(i, j, val)) {
                board[i][j] = val;
                if(++count%10000000 == 0) printSudoku();
                if (solveSudokuFW(i, j + 1)) return true;
                board[i][j] = 0;
            };
        };

        return false;
    }

    bool solveSudokuBW(int i, int j)
    {
        static int count = 0;

        if(i==9) {
            cout << endl << "Combinations tried: " << count << endl;
            return true;
        }
        if(j==9) return solveSudokuBW(i + 1, 0);
        if(board[i][j] != 0) return solveSudokuBW(i, j + 1);

        for (int val = 9; val > 0; --val) {
            if (check(i, j, val)) {
                board[i][j] = val;
                if(++count%10000000 == 0) printSudoku();
                if (solveSudokuBW(i, j + 1)) return true;
                board[i][j] = 0;
            };
        };

        return false;
    }

private:


    vector<vector<int>> areas;
    vector<int> areas_sum;
    vector<vector<int>> board = {{0,0,0,0,0,0,0,0,0},
                                 {0,0,0,0,0,0,0,0,0},
                                 {0,0,0,0,0,0,0,0,0},
                                 {0,0,0,0,0,0,0,0,0},
                                 {0,0,0,0,0,0,0,0,0},
                                 {0,0,0,0,0,0,0,0,0},
                                 {0,0,0,0,0,0,0,0,0},
                                 {0,0,0,0,0,0,0,0,0},
                                 {0,0,0,0,0,0,0,0,0}};
};

class thread_obj {
public:
    void operator()(bool forward, vector<vector<int>> areas, vector<int> areas_sum)
    {
        auto start = high_resolution_clock::now();

        KillerSudoku sudoku(std::move(areas), std::move(areas_sum));

        if(forward){
            sudoku.solveSudokuFW(0,0);
        }
        else{
            sudoku.solveSudokuBW(0,0);
        }

        cout << endl << endl;
        cout << "Solved Sudoku:" << endl;

        sudoku.printSudoku();

        auto stop = high_resolution_clock::now();

        auto duration = duration_cast<seconds>(stop - start);

        cout << endl << endl << "Run Duration: " << duration.count() << " seconds.";

        exit(0);
    }
};

int main() {

//    vector<vector<int>> areas   = {{0, 0, 0, 1, 2, 3, 3, 3, 4},
//                                   {5, 0, 0, 1, 2, 6, 6, 6, 4},
//                                   {5, 0, 7, 1, 8, 9, 9, 10, 10},
//                                   {11, 11, 7, 1, 8, 12, 12, 12, 10},
//                                   {11, 11, 13, 13, 13, 14, 14, 15, 15},
//                                   {11, 11, 16, 17, 18, 19, 19, 19, 20},
//                                   {21, 22, 16, 17, 18, 23, 23, 20, 20},
//                                   {21, 22, 22, 17, 24, 25, 25, 25, 26},
//                                   {22, 22, 22, 17, 24, 27, 27, 27, 26}};
//
//    vector<int> areas_sum = {27,17,9,12,15,10,16,10,10,11,8,32,24,16,7,10,13,24,11,20,10,10,26,8,10,12,15,12};

    vector<vector<int>> areas   ={{0 , 0 , 1 , 2 , 3 , 3 , 4 , 5 , 5 },
                                  {1 , 1 , 1 , 2 , 2 , 6 , 4 , 5 , 5 },
                                  {7 , 8 , 8 , 8 , 2 , 6 , 4 , 9 , 5 },
                                  {7 , 10, 8 , 8 , 11, 12, 12, 9 , 5 },
                                  {7 , 10, 13, 13, 11, 12, 12, 14, 14},
                                  {7 , 10, 15, 15, 11, 12, 12, 16, 17},
                                  {7 , 15, 15, 15, 18, 19, 20, 16, 17},
                                  {21, 21, 21, 18, 18, 19, 20, 17, 17},
                                  {22, 22, 21, 18, 23, 23, 20, 17, 17}};

    vector<int> areas_sum = {5,18,22,15,9,36,7,31,30,7,9,15,30,8,8,30,6,33,16,17,19,15,12,7};


    cout << "Solving sudoku... Please wait." << endl;

    thread th1(thread_obj(), 0, areas, areas_sum);
    thread th2(thread_obj(), 1, areas, areas_sum);

    th1.join();

    return 0;
}

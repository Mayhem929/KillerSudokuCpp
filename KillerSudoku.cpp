/*
 * Done by Adrián Jaén Fuentes, 2º DGIIM UGR
 */

#include <iostream>
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
        this->areas = std::move(areas);
        this->areas_sum = std::move(areas_sum);
    }

    /**
     * @brief Just prints the board
     */

    void printSudoku(){
        for(int i = 0; i < 9; ++i){
            for (int j = 0; j < 9; ++j) cout << board[i][j] << " ";
            cout << endl;
        }
        cout << endl;
    }

    /**
     * @brief This method computes recursively the sum of an area according to its 'area_id'.
     * The recursion checks the 4 cells that are beside the current one and adds the one recursively.
     * We could just check the whole matrix looking for area_id, but this way its faster.
     * @param i
     * @param j
     * @param area_id the id of the area whose cell we want to check
     * @param shown bool matrix to identify cells that have not been checked yet
     * @param sum
     * @post if area is not complete, sum = 0
     */

    void getSum(int i, int j, int area_id, bool shown[9][9], int &sum){

        for (int x = i - 1; x <= i + 1; ++x) {
            for (int y = j - 1; y <= j + 1; ++y) {

                // Only directly beside cells are checked given that areas cannot be diagonal

                // with condition        0                  0 0 0
                // (x == i || y == j): 0 0 0    without:    0 0 0
                //                       0                  0 0 0
                //              (0s are meant to be the cells that are checked)

                // It is also checked if cell is valid and hasn't been already considered

                if((x == i || y == j) && ((0 <= x && x < 9) && (0 <= y && y < 9)) && !shown[x][y]){

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
     * @param i row
     * @param j col
     * @param val number from 1 to 9
     * @return true if val can be put in position i,j
     */

    bool check(int i, int j, int val)
    {
        // Checks regular sudoku rules

        int row = i - i%3, column = j - j%3;

        // Check if val is repeated on
        for(int x=0; x<9; x++)
            if(board[x][j] == val)
                return false;
        for(int y=0; y<9; y++)
            if(board[i][y] == val)
                return false;

        // Check the numbers in the same 3x3 square
        for(int x=0; x<3; x++)
            for(int y=0; y<3; y++)
                if(board[row+x][column+y] == val) return false;

        // Checks Killer Sudoku rules

        int area_id = areas[i][j];
        // We save the value of board[i][j] and switch in temporarily the value we want to check
        int aux = board[i][j];
        board[i][j] = val;
        bool shown[9][9] = {false};
        int sum = 0;
        getSum(i, j, area_id, shown, sum);

        // Checks whether area is complete and if it is, if sum is correct
        if(sum != 0 && sum != areas_sum[area_id]) {
            board[i][j] = aux;
            return false;
        }

        return true;
    }

    /**
     * @param i current row
     * @param j current col
     * @return true if Killer Sudoku is finished
     */
    bool solveSudoku(int i, int j)
    {
        // Variable to count the total amount of numbers put on the grid
        static int count = 0;

        // End condition: if i == 9 it will mean row 8 (starting from 0) is full and correct to that point,
        // so the sudoku is totally solved
        if(i==9) {
            cout << endl << endl << "Combinations tried: " << count << endl << endl;
            return true;
        }
        // Move to the next cell, go to the next row if current is already full
        if(j==9) return solveSudoku(i + 1, 0);
        if(board[i][j] != 0) return solveSudoku(i, j + 1);

        // In this loop every number will be tried, and the ones which pass the check, will be put on the grid

        for (int num = 1; num <= 9; ++num) {
            if (check(i, j, num)) {
                ++count;
                board[i][j] = num;
                if (solveSudoku(i, j + 1)) return true;
                board[i][j] = 0;
            }
        }

        // If all numbers from 1 to 9 failed the check, the number will be put back to 0 (default value) and
        // SolveSudoku will return false. After that, the number in the previous cell will be put to 0 and the next
        // number will be tried, even though the previous one passed the check function. This is due to the fact
        // that the paths we can take from that number are wrong. This is known as a backtracking algorithm.

        return false;
    }

private:
    // This matrix identifies each area with a number, unique for each one. The way the number is selected is
    // by order of apparition of the areas if the Killer Sudoku is read from left to right and up to down.
    vector<vector<int>> areas;

    // In position i, this vector contains the sum of area with id i.
    vector<int> areas_sum;

    // It's always a 9x9 matrix full of 0s, the default value
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

int main() {

    // It is highly advisable to compile using   $ g++ KillerSudoku.cpp -O3
    // given that some sudokus make take long.

    /**********************************************************************************************/
    /**************************************** Data Section ****************************************/
    /**********************************************************************************************/

    // These killer sudokus are from https://www.dailykillersudoku.com/
    // Uncomment only one to use

    // Difficulty 1 --> 0 secs
    vector<vector<int>> areas = {{ 0, 1, 2, 3, 3, 4, 5, 5, 5},
                                 { 0, 1, 2, 6, 6, 4, 7, 5, 5},
                                 { 8, 9, 9,10, 6, 6, 7, 7, 5},
                                 { 8,11,11,10,12,12,13,14,14},
                                 {15,16,16,17,17,12,13,13,18},
                                 {15,16,19,19,17,20,20,13,18},
                                 {21,21,19,19,22,23,24,25,25},
                                 {21,21,21,22,22,23,24,26,26},
                                 {21,21,21,27,27,28,28,29,29}};

    vector<int> areas_sum = {15,4,13,14,13,31,14,14,11,7,13,10,19,17,9,10,13,14,11,18,10,37,14,12,7,10,17,12,8,8};

    // Difficulty 2 --> 0 secs
//    vector<vector<int>> areas = {{ 0, 1, 2, 2, 3, 3, 4, 5, 5},
//                                 { 0, 1, 6, 6, 7, 8, 4, 5, 5},
//                                 { 9,10,10, 7, 7, 8, 4, 4, 4},
//                                 { 9,11,11,11,12,12,13,13,14},
//                                 {15,16,16,17,17,12,18,18,14},
//                                 {15,15,19,19,17,20,18,21,22},
//                                 {23,23,23,19,24,20,25,21,22},
//                                 {26,23,23,27,24,20,25,28,28},
//                                 {26,26,23,27,27,29,29,30,30}};
//
//    vector<int> areas_sum = {8,8,15,11,28,17,14,7,12,13,7,13,19,5,15,15,10,15,20,15,9,10,3,27,16,13,18,10,12,9,11};

    // Difficulty 5 ---> 242 secs (too many!), needs to check 1.566.545.149 numbers but seems to be an exception
//    vector<vector<int>> areas  = {{0 , 0 , 1 , 2 , 3 , 3 , 4 , 5 , 5 },
//                                  {1 , 1 , 1 , 2 , 2 , 6 , 4 , 5 , 5 },
//                                  {7 , 8 , 8 , 8 , 2 , 6 , 4 , 9 , 5 },
//                                  {7 , 10, 8 , 8 , 11, 12, 12, 9 , 5 },
//                                  {7 , 10, 13, 13, 11, 12, 12, 14, 14},
//                                  {7 , 10, 15, 15, 11, 12, 12, 16, 17},
//                                  {7 , 15, 15, 15, 18, 19, 20, 16, 17},
//                                  {21, 21, 21, 18, 18, 19, 20, 17, 17},
//                                  {22, 22, 21, 18, 23, 23, 20, 17, 17}};
//
//    vector<int> areas_sum = {5,18,22,15,9,36,7,31,30,7,9,15,30,8,8,30,6,33,16,17,19,15,12,7};

    // Difficulty 7 --> 0 secs
//    vector<vector<int>> areas = {{ 0, 1, 1, 2, 2, 2, 3, 3, 4},
//                                 { 0, 5, 5, 2, 2, 2, 6, 6, 4},
//                                 { 7, 5, 5, 8, 8, 8, 6, 6, 9},
//                                 { 7,10,11,11,12,13,13,14, 9},
//                                 {10,10,15,15,12,16,16,14,14},
//                                 {17,17,18,19,19,19,20,21,21},
//                                 {17,18,18,22,23,24,20,20,21},
//                                 {17,25,25,22,23,24,26,26,21},
//                                 {27,27,25,22,23,24,26,28,28}};
//
//    vector<int> areas_sum = {13,4,34,9,14,20,15,15,11,12,13,11,8,5,13,11,17,14,14,17,18,19,12,22,11,15,20,13,5};


    // Difficulty 9 --> 20 secs
//    vector<vector<int>> areas   = {{ 0, 0, 0, 1, 2, 3, 3, 3, 4},
//                                   { 5, 0, 0, 1, 2, 6, 6, 6, 4},
//                                   { 5, 0, 7, 1, 8, 9, 9,10,10},
//                                   {11,11, 7, 1, 8,12,12,12,10},
//                                   {11,11,13,13,13,14,14,15,15},
//                                   {11,11,16,17,18,19,19,19,20},
//                                   {21,22,16,17,18,23,23,20,20},
//                                   {21,22,22,17,24,25,25,25,26},
//                                   {22,22,22,17,24,27,27,27,26}};
//
//    vector<int> areas_sum = {27,17,9,12,15,10,16,10,10,11,8,32,24,16,7,10,13,24,11,20,10,10,26,8,10,12,15,12};

    // Difficulty 10 --> 65 secs
//    vector<vector<int>> areas = {{ 0, 0, 1, 1, 2, 3, 4, 4, 4},
//                                 { 5, 0, 0, 1, 2, 3, 3, 6, 6},
//                                 { 5, 7, 8, 8, 8, 9, 9, 6, 6},
//                                 { 5, 7,10,10,11, 9,12,13,14},
//                                 {15, 7,16,10,11,17,12,13,14},
//                                 {15, 7,16,18,11,17,17,13,19},
//                                 {20,20,18,18,21,21,21,13,19},
//                                 {20,20,22,22,23,24,25,25,19},
//                                 {26,26,26,22,23,24,24,25,25}};
//
//    vector<int> areas_sum = {18,18,6,14,21,16,12,20,16,20,12,20,10,21,13,7,13,16,12,13,27,11,15,8,20,17,9};

    /**********************************************************************************************/

    cout << "Solving sudoku... Please wait." << endl;

    KillerSudoku sudoku(areas, areas_sum);

    auto start = high_resolution_clock::now();

    sudoku.solveSudoku(0, 0);

    auto stop = high_resolution_clock::now();

    cout << "Solved Sudoku:" << endl;

    sudoku.printSudoku();

    auto duration = duration_cast<microseconds>(stop - start);
    double micro = duration.count();

    cout << endl << "Run Duration: " << micro/1000000 << " secs." << endl << endl;

    return 0;
}

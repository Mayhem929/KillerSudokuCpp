#include <iostream>
#include <utility>
#include <vector>

using namespace std;

const int CHAR_TO_INT = '0';

class KillerSudoku{
public:
    /**
     * @brief Class constructor
     * @param areas_map
     * @param sum
     */

    KillerSudoku(vector<vector<int>> areas_map, vector<int> sum){
        areas = move(areas_map);
        areas_sum = move(sum);
    }

    void printSudoku(){
        for(auto & i : board){
            for (int j = 0; j < board[0].size(); ++j) cout << i[j] << " ";
            cout << endl;
        }
        cout << endl;
    }

    /**
     * @param areas
     * @param board
     * @param i
     * @param j
     * @return -1 if area is not complete yet, area's sum if complete
     */

    int getSum(int i, int j, int val){

        int area_id = areas[i][j];

        int sum = 0;
        board[i][j]=val;

        bool shown[9][9] = {false};

        for(int x = 0; x < 9; ++x){
            for (int y = 0; y < 9; ++y) {
                if(areas[x][y] == area_id){
                    if (board[x][y] == board[i][j] && (x != i || y != j)){
                        board[i][j] = 0;
                        return -1;
                    }
                    if(board[x][y] == 0) {
                        return -1;
                    }
                    else {
                        sum += board[x][y];
                    }
                }
            }
        }
        board[i][j] = 0;
        return sum;
    }

    /**
     *
     * @param board
     * @param areas_map
     * @param sum
     * @param i
     * @param j
     * @param val
     * @return true if value on given position is correct
     */

    bool check(int i, int j, int val)
    {
        int row = i - i%3, column = j - j%3;
        for(int x=0; x<9; x++) if(board[x][j] == val) return false;
        for(int y=0; y<9; y++) if(board[i][y] == val) return false;
        for(int x=0; x<3; x++)
            for(int y=0; y<3; y++)
                if(board[row+x][column+y] == val) return false;

        int area_id = areas[i][j];
        int sum = getSum(i, j, val);

        if(sum != -1 && sum != areas_sum[area_id]) return false;

        return true;
    }

    bool solveSudoku(int i, int j)
    {
        static int count = 0;
        if(i==9) return true;
        if(j==9) return solveSudoku(i+1, 0);
        if(board[i][j] != 0) return solveSudoku(i, j+1);

        for (int val = 9; val >= 0; --val) {
            if (check(i, j, val)) {
                board[i][j] = val;
                if (solveSudoku(i, j + 1)) return true;
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

/*
void printSudoku(vector<vector<char>> sudoku){
    for( int i = 0; i < sudoku.size(); ++i){
        for (int j = 0; j < sudoku[0].size(); ++j) {
            cout << sudoku[i][j] << " ";
        }
        cout << endl;
    }
}

bool check(vector<vector<char>> &board, int i, int j, char val)
{
    int row = i - i%3, column = j - j%3;
    for(int x=0; x<9; x++) if(board[x][j] == val) return false;
    for(int y=0; y<9; y++) if(board[i][y] == val) return false;
    for(int x=0; x<3; x++)
        for(int y=0; y<3; y++)
            if(board[row+x][column+y] == val) return false;

    return true;
}

bool solveSudoku(vector<vector<char>> &board, int i, int j)
{
    if(i==9) return true;
    if(j==9) return solveSudoku(board, i+1, 0);
    if(board[i][j] != '.') return solveSudoku(board, i, j+1);

    for (char c = '1'; c <= '9'; c++) {
        if (check(board, i, j, c)) {
            board[i][j] = c;
            printSudoku(board);
            if (solveSudoku(board, i, j + 1)) return true;
            board[i][j] = '.';
        };
    };

    return false;
}
*/
int main() {

    vector<vector<int>> areas   = {{0, 0, 1, 2, 3, 3, 4, 5, 5},
                                   {1, 1, 1, 2, 2, 6, 4, 5, 5},
                                   {7, 8, 8, 8, 2, 6, 4, 9, 5},
                                   {7, 10, 8, 8, 11, 12, 12, 9, 5},
                                   {7, 10, 13, 13, 11, 12, 12, 14, 14},
                                   {7, 10, 15, 15, 11, 12, 12, 16, 17},
                                   {7, 15, 15, 15, 18, 19, 20, 16, 17},
                                   {21, 21, 21, 18, 18, 19, 20, 17, 17},
                                   {22, 22, 21, 18, 23, 23, 20, 17, 17}};

    vector<int> areas_sum = {5,18,22,15,9,36,7,31,30,7,9,15,30,8,8,30,6,33,16,17,19,15,12,7};

    KillerSudoku sudoku(areas, areas_sum);


    cout << "Unsolved sudoku:" << endl;

    sudoku.printSudoku();

    sudoku.solveSudoku(0,0);

    cout << endl << endl;
    cout << "Solved Sudoku:" << endl;

    sudoku.printSudoku();

    return 0;
}
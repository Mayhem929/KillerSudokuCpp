#include <iostream>
#include <vector>
#include <chrono>

using namespace std::chrono;
using namespace std;

class KillerSudoku{
public:
    /**
     * @brief Class constructor
     * @param areas_map
     * @param sum
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
                if (0 <= x && x < 9 && 0 <= y && y < 9 && !shown[x][y]) {
                    if (areas[x][y] != area_id)continue;
                    if (board[x][y] == 0)
                        sum += 100;

                    shown[x][y] = true;
                    sum += board[x][y];
                    getSum(x,y,area_id,shown,sum);
                }
            }
        }
    }

    /**
     *
     * @param i
     * @param j
     * @param val
     * @return
     */

    bool check(int i, int j, int val)
    {
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

        int area_id = areas[i][j];
        int aux = board[i][j];
        board[i][j] = val;
        bool shown[9][9] = {false};
        int sum = 0;
        getSum(i, j, area_id, shown, sum);

        if(sum < 100 && sum != areas_sum[area_id]) {
            board[i][j] = aux;
            return false;
        }

        return true;
    }

    bool solveSudoku(int i, int j)
    {
        static int count = 0;
        if(i==9) {
            cout << endl << "Iterations needed: " << count << endl;
            return true;
        }
        if(j==9) return solveSudoku(i+1, 0);
        if(board[i][j] != 0) return solveSudoku(i, j+1);

        for (int val = 9; val > 0; --val) {
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


int main() {

    auto start = high_resolution_clock::now();

    vector<vector<int>> areas   = {{0 , 0 , 1 , 2 , 3 , 3 , 4 , 5 , 5 },
                                   {1 , 1 , 1 , 2 , 2 , 6 , 4 , 5 , 5 },
                                   {7 , 8 , 8 , 8 , 2 , 6 , 4 , 9 , 5 },
                                   {7 , 10, 8 , 8 , 11, 12, 12, 9 , 5 },
                                   {7 , 10, 13, 13, 11, 12, 12, 14, 14},
                                   {7 , 10, 15, 15, 11, 12, 12, 16, 17},
                                   {7 , 15, 15, 15, 18, 19, 20, 16, 17},
                                   {21, 21, 21, 18, 18, 19, 20, 17, 17},
                                   {22, 22, 21, 18, 23, 23, 20, 17, 17}};

    vector<int> areas_sum = {5,18,22,15,9,36,7,31,30,7,9,15,30,8,8,30,6,33,16,17,19,15,12,7};

    KillerSudoku sudoku(areas, areas_sum);

    cout << "Int implementation: " << endl << endl;
    cout << "Unsolved sudoku:" << endl;

    sudoku.printSudoku();

    sudoku.solveSudoku(0,0);

    cout << endl << endl;
    cout << "Solved Sudoku:" << endl;

    sudoku.printSudoku();

    auto stop = high_resolution_clock::now();

    auto duration = duration_cast<microseconds>(stop - start);

    cout << endl << endl << "Run Duration: " << duration.count() << " microsecods.";

    return 0;
}

#include <iostream>
#include <fstream>

using namespace std;

const int GRID_SIZE = 9;
const int BLANK_CELL = 0;


void write_cell(int cells[][GRID_SIZE], int row, int col, int val) {
    // 입력받은 row, col 값에 val값을 write해주는 함수이다.
    cells[row][col] = val;

}

void erase_cell(int cells[][GRID_SIZE], int row, int col) {
    // 입력받은 row, col 값에 해당하는 위치의 숫자를 0으로 만드는 함수이다.
    cells[row][col] = 0;
}

int which_block(int row, int col) {
    // 9개의 3x3 block중에 어느 위치에 있는지 판단하여 그 위치를 return해준다.
    // 0 1 2 
    // 3 4 5
    // 6 7 8
    int row_block = row / 3;
    int col_block = col / 3; 
    // 각 row와 col을 3으로 나눈 몫을 이용하여 각 block마다 다른 값이 나오도록 한다.
    return 3 * row_block + col_block;
}

bool is_valid(int cells[][GRID_SIZE], int row, int col, int val, int game_type) {
    // 입력하고자 하는 위치에 val 값이 들어가도 되는지 판단한다.
    for (int row_check = 0; row_check < GRID_SIZE; row_check++) {
        if (cells[row_check][col] == val) {
            return false;
            
       }
    } //만약 세로 줄에 같은 숫자가 있으면 false를 return한다.

    for (int col_check = 0; col_check < GRID_SIZE; col_check++) {
        if (cells[row][col_check] == val) {
            return false;
            
        }
    } //만약 가로 줄에 같은 숫자가 있으면 false를 return 한다.
    int target_block = which_block(row, col);
    for (int block_check = 0; block_check < GRID_SIZE; block_check++) {
        for (int block_check_2 = 0; block_check_2 < GRID_SIZE; block_check_2++) {
            if (target_block == which_block(block_check, block_check_2) && val==cells[block_check][block_check_2]) {
                return false;
            }
        }
    } //만약 같은 block에 같은 숫자가 있으면 false를 return한다. block_check와 block_check_2는
    // checking하는 row와 col값이다.

    if (game_type == 2) {

        if (row == col) {
            for (int x_check = 0; x_check < GRID_SIZE; x_check++) {
                if (cells[x_check][x_check] == val) {
                    return false;
                }
            } // "\"방향 대각선 checking
        }
        if (row + col == 8) {
            for (int x_check = 0; x_check < GRID_SIZE; x_check++) {
                if (cells[GRID_SIZE - x_check-1][x_check]==val) {
                    return false;
                }
            } // '/"방향 대각선 checking
            
        }
    } // 만약 X-sudoku 중이었다면 추가로 x-sudoku 규칙에 valid한지도 확인한다.

    return true; //위에서 return되지 않았으면 그 값이 가능하다는 의미이다. 
                 // true를 return한다.

}

bool is_cell_empty(const int cells[][GRID_SIZE], int row, int col) {
    // 해당 (row,col) cell이 비어있는지, 즉 값이 0인지 확인하여 비어있을 경우 true를 return한다.
    // 비어있지 않으면 false를 return한다.
    if (cells[row][col] == 0) {
        return true;
    }
    else {
        return false;
    }
}

void print(int cells[][GRID_SIZE]) {
    // cell을 입력받으면 각 entry를 출력해준다.
    for (int row = 0; row < GRID_SIZE; row++) {
        for (int col = 0; col < GRID_SIZE; col++) {
            cout << cells[row][col];

            if (col != GRID_SIZE - 1) {
                cout << " ";
            } //마지막 열이 아니면 각 숫자마다 띄어쓰기를 해준다.
            
        }
        cout << endl; //각 열이 끝나면 줄바꿈
    }
}

bool solve(int cells[][GRID_SIZE], int game_type) {
    // cell 상태와 game_type을 입력받으면 solve된 결과를 return하는 함수이다.


    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            if (is_cell_empty(cells, i, j)) {
                // cell을 쭉 searching하면서 cell이 비어있으면 아래를 실행한다.
                for (int candidate = 1; candidate < 10; candidate++) {

                    //1~9의 후보 증에서 차례로 빈칸에 넣어간다.

                    if (is_valid(cells, i, j, candidate, game_type)) {
                        write_cell(cells,i,j,candidate);
                        
                    
                        if (solve(cells, game_type)==false) {
                            erase_cell(cells, i, j);
                           
                        } //recursion을 이용하여 숫자 1개를 채운 cell에 대하여 solve를 진행한다.
                          //만약 해당 solve에서 false가 출력되면 원래 빈칸에 넣었던 숫자를 erase하고
                          //다음 숫자를 넣는다.
                    }
                }
                if (is_cell_empty(cells, i, j)) {
                    return false;
                } // 1부터 9까지 다 넣었는데도 가능한 것이 없다면 false를 return한다.

            }


        }
      
    }

    
    return true; //return false없이 모든 과정이 끝나면 true를 return한다.
}

void get_sudoku_input(int cells[][GRID_SIZE]) {
    int digit;

    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            cin >> digit;

            write_cell(cells, i, j, digit);
        }
    }
}


void run() {
    // mandatory
    int cells[GRID_SIZE][GRID_SIZE];
    int game_type;
    int user_input;
    int target_row, target_col, target_val;

    cout << endl;
    cout << "===================================" << endl;
    cout << "|                                 |" << endl;
    cout << "|                                 |" << endl;
    cout << "|             Game of             |" << endl;
    cout << "|         - S U D O K U -         |" << endl;
    cout << "|                                 |" << endl;
    cout << "|                                 |" << endl;
    cout << "===================================" << endl;
    cout << "----- Select the Sudoku Rule ------" << endl;
    cout << "| 1. Basic                        |" << endl;
    cout << "| 2. X-Sudoku                     |" << endl;
    cout << "-----------------------------------" << endl;
    cout << "> ";  cin >> game_type;

    cout << "> Initial Sudoku Grid: " << endl;
    get_sudoku_input(cells);

    cout << endl;
    cout << "------------- OPTIONS -------------" << endl;
    cout << "| 1: Write                        |" << endl;
    cout << "| 2: Erase                        |" << endl;
    cout << "| 3: Solve                        |" << endl;
    cout << "-----------------------------------" << endl;
    cout << "> ";  cin >> user_input;

    // 각 menu에서는 sudoku의 종류와 수행할 옵션을 숫자로 입력받는다.
    if (user_input == 1) {
        //write를 하려는 경우 row, col, val값을 받아서 write_cell 함수를 실행하고
        //print를 이용해 결과를 출력한다.
        cin >> target_row;
        cin >> target_col;
        cin >> target_val;

        write_cell(cells, target_row-1, target_col-1, target_val);
        print(cells);

    }
    else if (user_input == 2) {
        //erase를 하는 경우 row와 col값을 받아서 erase_cell함수를 실행하고
        //결과를 출력한다.
        cin >> target_row;
        cin >> target_col;

        erase_cell(cells, target_row - 1, target_col - 1);
        print(cells);
    }
    else if (user_input == 3) {
        //solve를 하는 경우 cell과 game_type을 입력한 것을 solve함수로 보낸다.
        if (solve(cells, game_type)) {
            print(cells);
        } //만약 solve가 정상적으로 이루어져서 true가 return되면 solve 결과인 cells를 출력한다.
        else {
            cout << "invalid solution";
        } //모든 숫자가 들어갈 수 없어 false가 return된 경우 invalid solution을 출력한다.
        
    }
}



int main(void) {
    run();
    
    return 0;
} //main()함수로 run() 함수가 실행되면 menu가 출력된다.
#include <iostream>
#include <fstream>

using namespace std;

const int GRID_SIZE = 9;
const int BLANK_CELL = 0;


void write_cell(int cells[][GRID_SIZE], int row, int col, int val) {
    // �Է¹��� row, col ���� val���� write���ִ� �Լ��̴�.
    cells[row][col] = val;

}

void erase_cell(int cells[][GRID_SIZE], int row, int col) {
    // �Է¹��� row, col ���� �ش��ϴ� ��ġ�� ���ڸ� 0���� ����� �Լ��̴�.
    cells[row][col] = 0;
}

int which_block(int row, int col) {
    // 9���� 3x3 block�߿� ��� ��ġ�� �ִ��� �Ǵ��Ͽ� �� ��ġ�� return���ش�.
    // 0 1 2 
    // 3 4 5
    // 6 7 8
    int row_block = row / 3;
    int col_block = col / 3; 
    // �� row�� col�� 3���� ���� ���� �̿��Ͽ� �� block���� �ٸ� ���� �������� �Ѵ�.
    return 3 * row_block + col_block;
}

bool is_valid(int cells[][GRID_SIZE], int row, int col, int val, int game_type) {
    // �Է��ϰ��� �ϴ� ��ġ�� val ���� ���� �Ǵ��� �Ǵ��Ѵ�.
    for (int row_check = 0; row_check < GRID_SIZE; row_check++) {
        if (cells[row_check][col] == val) {
            return false;
            
       }
    } //���� ���� �ٿ� ���� ���ڰ� ������ false�� return�Ѵ�.

    for (int col_check = 0; col_check < GRID_SIZE; col_check++) {
        if (cells[row][col_check] == val) {
            return false;
            
        }
    } //���� ���� �ٿ� ���� ���ڰ� ������ false�� return �Ѵ�.
    int target_block = which_block(row, col);
    for (int block_check = 0; block_check < GRID_SIZE; block_check++) {
        for (int block_check_2 = 0; block_check_2 < GRID_SIZE; block_check_2++) {
            if (target_block == which_block(block_check, block_check_2) && val==cells[block_check][block_check_2]) {
                return false;
            }
        }
    } //���� ���� block�� ���� ���ڰ� ������ false�� return�Ѵ�. block_check�� block_check_2��
    // checking�ϴ� row�� col���̴�.

    if (game_type == 2) {

        if (row == col) {
            for (int x_check = 0; x_check < GRID_SIZE; x_check++) {
                if (cells[x_check][x_check] == val) {
                    return false;
                }
            } // "\"���� �밢�� checking
        }
        if (row + col == 8) {
            for (int x_check = 0; x_check < GRID_SIZE; x_check++) {
                if (cells[GRID_SIZE - x_check-1][x_check]==val) {
                    return false;
                }
            } // '/"���� �밢�� checking
            
        }
    } // ���� X-sudoku ���̾��ٸ� �߰��� x-sudoku ��Ģ�� valid������ Ȯ���Ѵ�.

    return true; //������ return���� �ʾ����� �� ���� �����ϴٴ� �ǹ��̴�. 
                 // true�� return�Ѵ�.

}

bool is_cell_empty(const int cells[][GRID_SIZE], int row, int col) {
    // �ش� (row,col) cell�� ����ִ���, �� ���� 0���� Ȯ���Ͽ� ������� ��� true�� return�Ѵ�.
    // ������� ������ false�� return�Ѵ�.
    if (cells[row][col] == 0) {
        return true;
    }
    else {
        return false;
    }
}

void print(int cells[][GRID_SIZE]) {
    // cell�� �Է¹����� �� entry�� ������ش�.
    for (int row = 0; row < GRID_SIZE; row++) {
        for (int col = 0; col < GRID_SIZE; col++) {
            cout << cells[row][col];

            if (col != GRID_SIZE - 1) {
                cout << " ";
            } //������ ���� �ƴϸ� �� ���ڸ��� ���⸦ ���ش�.
            
        }
        cout << endl; //�� ���� ������ �ٹٲ�
    }
}

bool solve(int cells[][GRID_SIZE], int game_type) {
    // cell ���¿� game_type�� �Է¹����� solve�� ����� return�ϴ� �Լ��̴�.


    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            if (is_cell_empty(cells, i, j)) {
                // cell�� �� searching�ϸ鼭 cell�� ��������� �Ʒ��� �����Ѵ�.
                for (int candidate = 1; candidate < 10; candidate++) {

                    //1~9�� �ĺ� ������ ���ʷ� ��ĭ�� �־��.

                    if (is_valid(cells, i, j, candidate, game_type)) {
                        write_cell(cells,i,j,candidate);
                        
                    
                        if (solve(cells, game_type)==false) {
                            erase_cell(cells, i, j);
                           
                        } //recursion�� �̿��Ͽ� ���� 1���� ä�� cell�� ���Ͽ� solve�� �����Ѵ�.
                          //���� �ش� solve���� false�� ��µǸ� ���� ��ĭ�� �־��� ���ڸ� erase�ϰ�
                          //���� ���ڸ� �ִ´�.
                    }
                }
                if (is_cell_empty(cells, i, j)) {
                    return false;
                } // 1���� 9���� �� �־��µ��� ������ ���� ���ٸ� false�� return�Ѵ�.

            }


        }
      
    }

    
    return true; //return false���� ��� ������ ������ true�� return�Ѵ�.
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

    // �� menu������ sudoku�� ������ ������ �ɼ��� ���ڷ� �Է¹޴´�.
    if (user_input == 1) {
        //write�� �Ϸ��� ��� row, col, val���� �޾Ƽ� write_cell �Լ��� �����ϰ�
        //print�� �̿��� ����� ����Ѵ�.
        cin >> target_row;
        cin >> target_col;
        cin >> target_val;

        write_cell(cells, target_row-1, target_col-1, target_val);
        print(cells);

    }
    else if (user_input == 2) {
        //erase�� �ϴ� ��� row�� col���� �޾Ƽ� erase_cell�Լ��� �����ϰ�
        //����� ����Ѵ�.
        cin >> target_row;
        cin >> target_col;

        erase_cell(cells, target_row - 1, target_col - 1);
        print(cells);
    }
    else if (user_input == 3) {
        //solve�� �ϴ� ��� cell�� game_type�� �Է��� ���� solve�Լ��� ������.
        if (solve(cells, game_type)) {
            print(cells);
        } //���� solve�� ���������� �̷������ true�� return�Ǹ� solve ����� cells�� ����Ѵ�.
        else {
            cout << "invalid solution";
        } //��� ���ڰ� �� �� ���� false�� return�� ��� invalid solution�� ����Ѵ�.
        
    }
}



int main(void) {
    run();
    
    return 0;
} //main()�Լ��� run() �Լ��� ����Ǹ� menu�� ��µȴ�.
#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>
#include <time.h>

using namespace std;

const sf::Color ALIVE_COLOR = sf::Color::Black;
const sf::Color OLD_COLOR = sf::Color(100, 100, 100);
const sf::Color Pink = sf::Color(255, 182, 193);
const sf::Color Red = sf::Color::Red;
const sf::Color White = sf::Color(150, 150, 150);

/*
 * Enum of three possible cell states.
 */
enum class CellState {
    DEAD,
    ALIVE,
    OLD,
    RED,
    PINK,
    WHITE
    // You can add more states if needed for custom cell
};

/*
 * Struct that holds three values of cell's row number, column number, and its corresponding state
 * This is used for representing initial cell configurations as the location and the desired state of a cell.
 */
struct CellCoord {
    int row;
    int col;
    CellState state;
};

/*
 * Class representing a single cell in the grid of Game of Life.
 *
 * It holds information of a cell, including its location in the window, current state, color, future state, and its neighbors.
 * By using the information about its neighbors, the cell class also implements how to decide the next state of a cell.
 * This class also acts as the base cell for variant cells for different game modes.
 */
class Cell {
public:
    Cell(float xVal, float yVal) : x(xVal), y(yVal) {}

    float getX() const { return x; }
    float getY() const { return y; }
    CellState getState() const { return state; }
    sf::Color getColor() const { return color; }
    bool isAlive() const; /* TODO */

    void update() { state = nextState; }
    void setState(CellState newState) { state = newState; }
    virtual void computeNextState(); /* TODO */
    void setNeighbor(const Cell* cell); /* TODO */
    void resetNeighbor(); /* Reset 시 모든 cell의 neighbor을 reset한다.*/
    void resetColor(); /* Reset 시 모든 cell의 color를 reset한다.*/

protected:
    sf::Color color = ALIVE_COLOR;
    CellState state = CellState::DEAD;
    CellState nextState = CellState::DEAD;
    std::vector<const Cell*> neighbors;
private:
    float x;
    float y;
};

void Cell::resetColor() {
     color = ALIVE_COLOR; /*OLD COLOR가 된 cell을 reset시 초기화해준다.
                          reset을 통해 모두 DEAD로 만들어도 지정된 색깔은 reset되지 않아
                          따로 member function을 지정하였다.*/
}

bool Cell::isAlive() const {
    // TODO
    /* 세포가 DEAD라면 false, 아니라면 true를 반환*/
    
    if (state == CellState::DEAD) {
        return false;
    }
    else {
        return true;
    }

}

void Cell::computeNextState() { /*다음 상태가 무엇일지 계산한 뒤, nextState 변수에 저장해놓는다.*/
    // TODO
    if (neighbors.size() < 2 || neighbors.size() > 3) {
        nextState = CellState::DEAD;
    } /*이웃의 수가 2보다 적거나 3보다 크면 그 세포는 사멸*/
    else if (neighbors.size() == 3) {
        nextState = CellState::ALIVE;
    } /* 죽은 세포 주위에 3개의 이웃이 있다면 세포가 생성됨*/
    else {
        nextState = state;
    } /*그 외에는 현재 상태 유지*/

}

void Cell::setNeighbor(const Cell* cell) {


    neighbors.push_back(cell);
    /*neighbor vector에 입력받은 cell을 push back한다. 이는 해당 cell이 현재
    cell object에 인접해있음을 나타낸다.*/

}

void Cell::resetNeighbor() {
    neighbors.clear(); /*reset을 하여 모든 세포를 DEAD로 만들어도 neightbor이 바뀌지 않아
                       따로 neighbor을 초기화하는 member function을 지정하였다.*/
}

/*
 * Class representing an aging cell in the Aging variant of Game of Life.
 */
 /* TODO: AgingCell */

class AgingCell : public Cell {
public:
   virtual void computeNextState();
   AgingCell(float xVal, float yVal) : Cell(xVal,yVal) {}
private:
    int t = 0; /*time 변수*/
};

void AgingCell::computeNextState() {
    /* 다음 상태가 무엇인지 판단하는 함수이다. */

    if (neighbors.size() < 2 || neighbors.size() > 3) {
        nextState = CellState::DEAD;
    }
    else if (state==CellState::OLD){
        
        nextState = CellState::DEAD;
        /*old한 cell의 다음 상태는 항상 dead이다.*/
    }
    else if (neighbors.size() == 3 && !isAlive()) {
        nextState = CellState::ALIVE;
        t = 0;
    }
    else if (t == 2 && state==CellState::ALIVE) {
        /*만약 살아있는 상태에서 t가 2라면, OLD 상태로 전환한다.*/
        nextState = CellState::OLD;
        color = OLD_COLOR;

    }
    else {
        if (state == CellState::ALIVE) {
            t += 1;
        } /*그 외에 계속 살아있던 세포들에게 time을 1씩 추가하고 상태를 유지한다.*/
        nextState = state;
    }

}

 /*
  * Class representing a rule-based cell that follows the given rule variant of Game of Life.
  */
  /* TODO: RuleBasedCell  */

class RuleBasedCell : public Cell {
public :
    RuleBasedCell(float xVal, float yVal, string rule_) : Cell(xVal, yVal), rule(rule_) { set_rule(); }
    virtual void computeNextState();
    void set_rule();
private:
    string rule; /*gameRule을 받아온 변수*/
    string born_lst;
    string surv_lst;  /*각 string은 태어나는 조건과 생존하는 조건을 담은 변수이다.
                      순서는 고려되어있지 않다.*/

};
void RuleBasedCell::set_rule(){

    int slicing = 0;
    while (rule[slicing] != '/') {
        slicing++;
    } /* '/' 기준으로 입력된 rule을 나눠준다.*/


    born_lst = rule.substr(1, slicing - 1);
    surv_lst = rule.substr(slicing + 2, rule.length() - slicing - 2);
    /*slicing 해주어 각각의 변수에 할당*/
}
void RuleBasedCell::computeNextState() {
   
   

    if (surv_lst.find(to_string(neighbors.size()))==string::npos) {
        nextState = CellState::DEAD;
    }
    else if (born_lst.find(to_string(neighbors.size())) != string::npos) {
        nextState = CellState::ALIVE;
    }
    else {
        nextState = state;
    }
} /*현재 neighrbor의 수가, 탄생 조건 string에 포함되어 있다면 세포를 탄생시킨다.
  또, 살아있는 세포에서 생존조건에 없는 수의 neighbor가 있는경우 해당 세포를 DEAD로 만든다.
  이를 위하여 string의 find 함수를 사용하였다.*/

  /*
   * Class representing a custom cell of your own variant of Game of Life.
   */
   /* TODO: CustomCell  */
   /*
    * Enum of four available game modes.
    */

/*
Custom cell은 분꽃의 중간유전(불완전우성) 알고리즘을 이용하여 분꽃의 군체가 어떻게 퍼져나가는지
관찰할 수 있는 시뮬레이션을 구성하였다. 분꽃에게는 '색깔'유전인자가 있는데, 실제 멘델의 법칙에 맞게
태어나는 분꽃(cell)의 색깔을 설정하였다. 더 자세한 동기 및 배경이론은 레포트에 기재하였다.

*/

class CustomCell : public Cell {
public:
    CustomCell(float xVal, float yVal) : Cell(xVal, yVal) {

        if (state == CellState::RED) { color = Red; }
        if (state == CellState::PINK) { color = Pink; }
        if (state == CellState::WHITE) { color = White; } /*처음 꽃 색깔 설정*/
    }
    virtual void computeNextState();
    void inheritance(const Cell* a, const Cell* b); /*유전 함수*/
    void randomize_color(); /*돌연변이 함수*/
};

void CustomCell::randomize_color() {
    /*돌연변이 함수이다. srand()로 랜덤하게 발생한 돌연변이는, 해당 꽃 상태를
    임의의 색깔로 바꾸어버린다. 부모의 색과 무관하다. 유전자의 다양성을 생성한다.*/
    int rd = (rand() % 3);

    if (rd == 0) {
        nextState = CellState::RED;
        color = Red;
    }
    if (rd == 1) {
        nextState = CellState::PINK;
        color = Pink;
    }
    if (rd ==2){
        nextState = CellState::WHITE;
        color = White;
    }
}

void CustomCell::inheritance(const Cell* a, const Cell* b) {
    /*부모의 Cell에서 각각 color를 받아와, 멘델의 법칙에 따라서 다음 state의 색깔을 결정한다.*/
    if (a->getColor() == Red && b->getColor() == Red) {
        nextState = CellState::RED;
        color = Red;
        return;
    }
    else if (a->getColor() == White && b->getColor() == White) {
        nextState = CellState::WHITE;
        color = White;
        return;
    }
    else if ((a->getColor() == Red && b->getColor() == White) || (a->getColor() == White && b->getColor() == Red)) {
        nextState = CellState::PINK;
        color = Pink;
        return;
    }
    else if (a->getColor() == Pink && b->getColor() == Pink) {
        

        int rd = (rand() % 4);

        if (rd == 0 || rd == 1) {
            nextState = CellState::PINK;
            color = Pink;
            return;
        }
        else if (rd == 2) {
            nextState = CellState::RED;
            color = Red;
            return;
        }
        else if (rd ==3){
            nextState = CellState::WHITE;
            color = White;
            return;
        }

    }
    else if ((a->getColor() == Red && b->getColor() == Pink)|| (a->getColor() == Pink && b->getColor() == Red)) {
        int rd = (rand() % 2);

        if (rd == 0) {
            nextState = CellState::PINK;
            color = Pink;
            return;
        }
        
        else {
            nextState = CellState::RED;
            color = Red;
            return;
        }

    }
    else {
        int rd = (rand() % 2);

        if (rd == 0) {
            nextState = CellState::PINK;
            color = Pink;
            return;
        }

        else if (rd ==1) {
            nextState = CellState::WHITE;
            color = White;
            return;
        }

    }

    
}


void CustomCell::computeNextState() { /*다음 상태가 무엇일지 계산한 뒤, nextState 변수에 저장해놓는다.*/
    // TODO
    if (neighbors.size() < 2 || neighbors.size() > 3) {
        nextState = CellState::DEAD; /*2개보다 작거나 3개보다 크면 죽는다. 이는 BASIC과 동일*/
    }

    else if (neighbors.size() == 2) {

        const Cell* a = neighbors[0]; /*부모의 유전인자를 가져온다.*/
        const Cell* b = neighbors[1];
        inheritance(a,b);
        int i = rand() % 100;
        if (i == 0) {
            randomize_color(); /* 1%의 확률로 돌연변이가 발생하도록 한다.*/
        }
    }


    else {
        nextState = state; /*아무것도 해당이 안된다면 그냥 상태를 유지한다.*/
    }

    
    
}





enum class GameMode {
    BASIC = 1,
    AGING,
    RULE_BASED,
    CUSTOM
};

/*
 * Struct containing various configuration values for the game ranging from program window sizes to game mode.
 *
 * You should keep the default given configuration values for basic/aging/rule_based game modes.
 * You are allowed to use custom configuration for your custom game mode.
 * The grid width/height is computed using window width/height - 2 * marginSize
 */
struct GameConfig {
    int windowWidth = 600;
    int windowHeight = 600;
    float marginSize = 60;
    int stepSpeedInMilliseconds = 80;
    sf::Color backgroundColor = sf::Color::White;
    sf::Color fontColor = sf::Color::Black;
    std::string fontPath = "Arial.ttf";
    int fontSize = 13;
    std::string windowTitle = "Conway's Game of Life";
    int numRows=10;
    int numCols=10;
    GameMode gameMode = GameMode::BASIC;
    std::string gameRule = "";
    sf::Color gridLineColor = sf::Color(200, 200, 200);
    float gridLineThickness = 1.0;
};

/*
 * Class that holds and manages all the cells in the grid of Game of Life.
 *
 * Its main use is to apply batch operations on cells such as initializing, resetting, drawing and updating.
 */
class Grid {
public:
    Grid(const GameConfig& cfg); /* TODO */
    ~Grid(); /* TODO */
    void initializeCells(const std::vector<CellCoord>& coords); /* TODO */
    void updateCells(); /* TODO */
    void resetCells(); /* TODO */
    const Cell* getCell(int rowIdx, int colIdx) const { return cells.at(rowIdx).at(colIdx); }

    // Draw the cells on given window
    void drawOn(sf::RenderWindow& window) const;
private:
    GameConfig config;
    std::vector<std::vector<Cell*>> cells;
};

// Helper functions to compute grid-related dimensions from game configuration
float getGridWidth(const GameConfig& config);
float getGridHeight(const GameConfig& config);
float getCellWidth(const GameConfig& config);
float getCellHeight(const GameConfig& config);


Grid::Grid(const GameConfig& cfg) {
    // TODO
    config = cfg;
    float width = getCellWidth(cfg);
    float height = getCellHeight(cfg);
    float gh = getGridHeight(cfg);
    float gw = getGridWidth(cfg); /*row / col 개수에 맞추어 cell 표시 위치 조정을 위해 지정한 변수*/

    if (config.gameMode == ::GameMode::AGING) {
        if (cells.size() == 0) {
            for (int i = 0; i < config.numRows; i++) {
                std::vector<Cell*> column_cell; /*row vector를 형성한다.*/
                for (int j = 0; j < config.numCols; j++) {
                    AgingCell tcell(((600 - gw) / 2) + (j)*width, ((600 - gh) / 2) + (i)*height); 
                    /*벡터에 cell object를 push back해준다. 이때 window 크기 - grid 크기로 (0,0) cell의 위치를 조정하였다. */
                    AgingCell* pnt = new AgingCell(tcell); /*dynamic array 형성*/
                    column_cell.push_back(pnt);

                }
                cells.push_back(column_cell);
            }
        }
    }






    if (config.gameMode == ::GameMode::BASIC) { /*gamemode가 BASIC일 때의 경우로 위의 AGING과 class를 제외하고 동일하다.*/
        if (cells.size() == 0) {
            for (int i = 0; i < config.numRows; i++) {
                std::vector<Cell*> column_cell;
                for (int j = 0; j < config.numCols; j++) {
                    Cell tcell(((600 - gw) / 2) + (j)*width, ((600 - gh) / 2) + (i)*height);
                    Cell* pnt = new Cell(tcell);
                    column_cell.push_back(pnt);

                }
                cells.push_back(column_cell);
            }
        }
    }

    if (config.gameMode == ::GameMode::CUSTOM) { /*custom에서의 initializing*/
        if (cells.size() == 0) {
            for (int i = 0; i < config.numRows; i++) {
                std::vector<Cell*> column_cell; /*row vector를 형성한다.*/
                for (int j = 0; j < config.numCols; j++) {
                    CustomCell tcell(((600 - gw) / 2) + (j)*width, ((600 - gh) / 2) + (i)*height);
                    /*벡터에 cell object를 push back해준다. 이때 window 크기 - grid 크기로 (0,0) cell의 위치를 조정하였다. */
                    CustomCell* pnt = new CustomCell(tcell); /*dynamic array 형성*/
                    column_cell.push_back(pnt);

                }
                cells.push_back(column_cell);
            }
        }
    }


    if (config.gameMode == ::GameMode::RULE_BASED) { /*gamemode가 RULE_BASED일 때로 위 2개의 모드와 class를 제외하고 동일한 역할을 한다.*/
        if (cells.size() == 0) {
            for (int i = 0; i < config.numRows; i++) {
                std::vector<Cell*> column_cell;
                for (int j = 0; j < config.numCols; j++) {
                    RuleBasedCell tcell(((600 - gw) / 2) + (j)*width, ((600 - gh) / 2) + (i)*height, config.gameRule);
                    RuleBasedCell* pnt = new RuleBasedCell(tcell);
                    column_cell.push_back(pnt);

                }
                cells.push_back(column_cell);
            }
        }
    }


}

Grid::~Grid() { /*grid의 destructor이다. dynamic array에 할당된 것들을 제거한다.*/
    // TODO
    for (int i = 0; i < config.numRows; i++) {
        for (int j = 0; j < config.numCols; j++) {
            delete[] cells[i][j];
            cells[i][j] = NULL;
        }
    }
}

void Grid::initializeCells(const std::vector<CellCoord>& coords) {
    /*맨 처음 cell들을 initialize한다. 초기 조건 정보가 담긴 vector를 입력받으면, 
    해당 위치의 cell을 ALIVE로 바꾼다.*/
    
    for (int i = 0; i < coords.size(); i++) {
        cells[coords[i].row][coords[i].col]->setState(coords[i].state);
    }
}

void Grid::updateCells() {
    /*cell 상태를 update한다. 이때 먼저 모든 cell에 대하여 살아있는 cell들 주변의 neighbor에
    * 살아있는 세포를 neighbor로 지정해준다*/
    /*이 경우 먼저 edge 조건을 고려하여 가운데 부분을 먼저 처리하였다.*/
    int r = config.numRows-1;
    int c = config.numCols-1;
    for (int i = 1; i < config.numRows-1; i++) {
        for (int j = 1; j < config.numCols-1; j++) {
            if (cells[i][j]->getState() != CellState::DEAD) {
                cells[i][j+1]->setNeighbor(cells[i][j]);
                cells[i+1][j+1]->setNeighbor(cells[i][j]);
                cells[i-1][j+1]->setNeighbor(cells[i][j]);
                cells[i-1][j]->setNeighbor(cells[i][j]);
                cells[i+1][j]->setNeighbor(cells[i][j]);
                cells[i-1][j-1]->setNeighbor(cells[i][j]);
                cells[i][j-1]->setNeighbor(cells[i][j]);
                cells[i+1][j-1]->setNeighbor(cells[i][j]);
            }
        }

    }
    /*아래의 4개는 각 모서리의 edge 조건을 고려한다.*/
    for (int i = 1; i < r; i++) {
        if (cells[i][0]->getState() != CellState::DEAD) {
            cells[i][1]->setNeighbor(cells[i][0]);
            cells[i+1][1]->setNeighbor(cells[i][0]);
            cells[i-1][1]->setNeighbor(cells[i][0]);
            cells[i-1][0]->setNeighbor(cells[i][0]);
            cells[i+1][0]->setNeighbor(cells[i][0]);
            cells[i-1][c]->setNeighbor(cells[i][0]);
            cells[i][c]->setNeighbor(cells[i][0]);
            cells[i+1][c]->setNeighbor(cells[i][0]);
        }
    }

    for (int i = 1; i < config.numRows-1; i++) {
        if (cells[i][c]->getState() != CellState::DEAD) {
            cells[i][0]->setNeighbor(cells[i][c]);
            cells[i+1][0]->setNeighbor(cells[i][c]);
            cells[i-1][0]->setNeighbor(cells[i][c]);
            cells[i-1][c]->setNeighbor(cells[i][c]);
            cells[i+1][c]->setNeighbor(cells[i][c]);
            cells[i-1][c-1]->setNeighbor(cells[i][c]);
            cells[i][c-1]->setNeighbor(cells[i][c]);
            cells[i+1][c-1]->setNeighbor(cells[i][c]);
        }
        }

    for (int j = 1; j < config.numCols-1; j++) {
        if (cells[0][j]->getState() != CellState::DEAD) {
            cells[0][j+1]->setNeighbor(cells[0][j]);
            cells[1][j+1]->setNeighbor(cells[0][j]);
            cells[r][j+1]->setNeighbor(cells[0][j]);
            cells[r][j]->setNeighbor(cells[0][j]);
            cells[1][j]->setNeighbor(cells[0][j]);
            cells[r][j-1]->setNeighbor(cells[0][j]);
            cells[0][j-1]->setNeighbor(cells[0][j]);
            cells[1][j-1]->setNeighbor(cells[0][j]);
        }
       }

    for (int j = 1; j < config.numCols-1; j++) {
        if (cells[r][j]->getState() != CellState::DEAD) {
            cells[r][j+1]->setNeighbor(cells[r][j]);
            cells[0][j+1]->setNeighbor(cells[r][j]);
            cells[r-1][j+1]->setNeighbor(cells[r][j]);
            cells[0][j]->setNeighbor(cells[r][j]);
            cells[r-1][j]->setNeighbor(cells[r][j]);
            cells[r-1][j-1]->setNeighbor(cells[r][j]);
            cells[r][j-1]->setNeighbor(cells[r][j]);
            cells[0][j-1]->setNeighbor(cells[r][j]);
        }
    }

    /*아래의 4개는 네 꼭짓점의 조건을 고려한다.*/

    if (cells[0][0]->getState() != CellState::DEAD) {
        cells[0][1]->setNeighbor(cells[0][0]);
        cells[1][1]->setNeighbor(cells[0][0]);
        cells[1][0]->setNeighbor(cells[0][0]);
        cells[0][c]->setNeighbor(cells[0][0]);
        cells[1][c]->setNeighbor(cells[0][0]);
        cells[r][c]->setNeighbor(cells[0][0]);
        cells[r][0]->setNeighbor(cells[0][0]);
        cells[r][1]->setNeighbor(cells[0][0]);
    }
    if (cells[0][c]->getState() != CellState::DEAD) {
        cells[0][0]->setNeighbor(cells[0][c]);
        cells[r][0]->setNeighbor(cells[0][c]);
        cells[r][c-1]->setNeighbor(cells[0][c]);
        cells[0][c-1]->setNeighbor(cells[0][c]);
        cells[1][c-1]->setNeighbor(cells[0][c]);
        cells[1][c]->setNeighbor(cells[0][c]);
        cells[r][c]->setNeighbor(cells[0][c]);
        cells[1][0]->setNeighbor(cells[0][c]);
    }
    if (cells[r][0]->getState() != CellState::DEAD) {
        cells[0][0]->setNeighbor(cells[r][0]);
        cells[0][1]->setNeighbor(cells[r][0]);
        cells[0][c]->setNeighbor(cells[r][0]);
        cells[r-1][0]->setNeighbor(cells[r][0]);
        cells[r-1][1]->setNeighbor(cells[r][0]);
        cells[r-1][c]->setNeighbor(cells[r][0]);
        cells[r][1]->setNeighbor(cells[r][0]);
        cells[r][c]->setNeighbor(cells[r][0]);
    }
    if (cells[r][c]->getState() != CellState::DEAD) {
        cells[0][0]->setNeighbor(cells[r][c]);
        cells[0][c-1]->setNeighbor(cells[r][c]);
        cells[0][c]->setNeighbor(cells[r][c]);
        cells[r-1][0]->setNeighbor(cells[r][c]);
        cells[r][0]->setNeighbor(cells[r][c]);
        cells[r-1][c-1]->setNeighbor(cells[r][c]);
        cells[r-1][c]->setNeighbor(cells[r][c]);
        cells[r][c-1]->setNeighbor(cells[r][c]);
    }

    /*이후 각 cell의 nextState를 계산하고 update()함수를 통해 갱신한다. 이때 이웃 정보는 reset하여
    다음 세대에서 영향이 없도록 한다.*/
    for (int i = 0; i < r+1; i++) {
        for (int j = 0; j < c+1; j++) {
            cells[i][j]->computeNextState();
            cells[i][j]->update();
            cells[i][j]->resetNeighbor();

        }
    }

}

void Grid::resetCells() {
    /* 모든 cell에 대하여 상태를 DEAD로 만든다. 이때 neighbor와 color도 초기화한다.*/
    

    for (int i = 0; i < config.numRows; i++) {
        for (int j = 0; j < config.numCols; j++) {
            cells[i][j]->setState(CellState::DEAD);
            cells[i][j]->resetNeighbor();
            cells[i][j]->resetColor();
            
        }
    }

}

float getGridWidth(const GameConfig& config) {
    return static_cast<float>(config.windowWidth) - 2 * config.marginSize;
}

float getGridHeight(const GameConfig& config) {
    return static_cast<float>(config.windowHeight) - 2 * config.marginSize;
}

float getCellWidth(const GameConfig& config) {
    return getGridWidth(config) / static_cast<float>(config.numCols);
}

float getCellHeight(const GameConfig& config) {
    return getGridHeight(config) / static_cast<float>(config.numRows);
}

void Grid::drawOn(sf::RenderWindow& window) const {
    // Draw cell if alive
    if (!cells.empty()) {
        for (int i = 0; i < config.numRows; i++) {
            for (int j = 0; j < config.numCols; j++) {
                if (cells[i][j]->isAlive()) {
                    sf::RectangleShape rect(sf::Vector2f(getCellWidth(config), getCellHeight(config)));
                    rect.setPosition(sf::Vector2f(cells[i][j]->getX(), cells[i][j]->getY()));
                    rect.setFillColor(cells[i][j]->getColor());
                    window.draw(rect);
                }
            }
        }
    }

    // Draw grid lines
    for (int i = 0; i <= config.numRows; i++) {  // horizontal lines
        
        sf::RectangleShape line(sf::Vector2f(getGridWidth(config), config.gridLineThickness));
        line.setPosition(sf::Vector2f(config.marginSize, config.marginSize + static_cast<float>(i) * getCellHeight(config)));
        line.setFillColor(config.gridLineColor);
        window.draw(line);
    }
    for (int i = 0; i <= config.numCols; i++) {  // vertical lines
        sf::RectangleShape line(sf::Vector2f(config.gridLineThickness, getGridHeight(config)));
        line.setPosition(sf::Vector2f(config.marginSize + static_cast<float>(i) * getCellWidth(config), config.marginSize));
        line.setFillColor(config.gridLineColor);
        window.draw(line);
    }
}

/*
 * Enum of two possible game states.
 */
enum class GameState {
    PLAYING,
    PAUSED
};

/*
 * Class that manages the entire game. Holds the grid object and the main rendering loop of the program.
 */
class GameManager {
public:
    // Construct a GameManager class with given configuration
    GameManager(const GameConfig& cfg);

    // Save the given initial cell coordinates in order to set and reset the grid during the game.
    void setInitialCoords(const std::vector<CellCoord>& coords) { initialCoords = coords; }

    // Main rendering loop that checks for keyboard events, updates program graphics, and calls grid updates.
    void run();
private:
    GameConfig config;
    sf::Font textFont;
    std::vector<CellCoord> initialCoords;
    sf::RenderWindow window;
    Grid grid;
    GameState state = GameState::PAUSED;
    int num_steps = 0;

    // Internal helper function for drawing information text in the window
    void drawInterface();
};

GameManager::GameManager(const GameConfig& cfg) : grid(cfg), config(cfg) {
    // load font file
    textFont.loadFromFile(config.fontPath);
    std::cout << "GameManager Initialized!" << std::endl;
    srand(time(NULL));
}

void GameManager::run() {
    std::cout << "Starting game..." << std::endl;
    // Create program window and initialize grid cells
    window.create(sf::VideoMode(config.windowWidth, config.windowHeight), config.windowTitle);
    grid.initializeCells(initialCoords);
    window.clear(config.backgroundColor);

    // Run main program rendering/update loop
    sf::Clock clock;
    while (window.isOpen()) {
        // Check for program events (close window, keyboard press)
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::KeyReleased) {
                switch (event.key.code) {
                    // toggle play/pause program when Space key is pressed
                case sf::Keyboard::Space:
                    state = state == GameState::PLAYING ? GameState::PAUSED : GameState::PLAYING;
                    clock.restart();
                    break;
                    // reset program when R key is pressed
                case sf::Keyboard::R:
                    state = GameState::PAUSED;
                    grid.resetCells();
                    grid.initializeCells(initialCoords);
                    num_steps = 0;
                    clock.restart();
                    break;
                    // apply single grid update and pause when N key is pressed
                case sf::Keyboard::N:
                    state = GameState::PAUSED;
                    num_steps++;
                    grid.updateCells();
                    break;
                }
            }
        }

        if (state == GameState::PLAYING) {
            // update grid cells after every fixed amount of time is elapsed
            if (clock.getElapsedTime().asMilliseconds() >= config.stepSpeedInMilliseconds) {
                num_steps++;
                grid.updateCells();
                clock.restart();
            }
        }

        // redraw interface and grid
        window.clear(config.backgroundColor);
        drawInterface();
        grid.drawOn(window);
        window.display();
    }
}

void GameManager::drawInterface() {
    float topY = config.marginSize / 2 - 5;
    float bottomY = static_cast<float>(config.windowWidth) - config.marginSize / 2;
    float centerX = static_cast<float>(config.windowWidth) / 2;

    // create text objects

    // top
    sf::Text topText("Press space to play/pause, R to reset, N to update once", textFont, config.fontSize);
    topText.setFillColor(config.fontColor);
    auto topBounds = topText.getLocalBounds();
    topText.setPosition(centerX - topBounds.width / 2, topY - topBounds.height / 2);

    // bottom
    sf::Text bottomText((state == GameState::PLAYING ? "PLAYING" : "PAUSED"), textFont, config.fontSize);
    bottomText.setFillColor(config.fontColor);
    auto bottomBounds = bottomText.getLocalBounds();
    bottomText.setPosition(centerX - bottomBounds.width / 2, bottomY - bottomBounds.height / 2);

    // bottom-left
    sf::Text leftBottomText("t=" + std::to_string(num_steps), textFont, config.fontSize);
    leftBottomText.setFillColor(config.fontColor);
    auto lbBounds = leftBottomText.getLocalBounds();
    leftBottomText.setPosition(config.marginSize, bottomY - lbBounds.height / 2);

    // right-bottom
    std::string gameModeName;
    switch (config.gameMode) {
    case GameMode::BASIC:
        gameModeName = "BASIC";
        break;
    case GameMode::AGING:
        gameModeName = "AGING";
        break;
    case GameMode::RULE_BASED:
        gameModeName = "RULE(" + config.gameRule + ")";
        break;
    case GameMode::CUSTOM:
        gameModeName = "CUSTOM";
        break;
    }
    sf::Text rightBottomText("MODE: " + gameModeName, textFont, config.fontSize);
    auto rbBounds = rightBottomText.getLocalBounds();
    rightBottomText.setFillColor(sf::Color::Black);
    rightBottomText.setPosition(static_cast<float>(config.windowWidth) - config.marginSize - rbBounds.width, bottomY - rbBounds.height / 2);

    // draw created text objects
    window.draw(topText);
    window.draw(bottomText);
    window.draw(leftBottomText);
    window.draw(rightBottomText);
}

#endif
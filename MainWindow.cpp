/* Reference: https://www.geeksforgeeks.org/minimax-algorithm-in-game-theory-set-3-tic-tac-toe-ai-finding-optimal-move/ */

#include <QMessageBox>
#include <algorithm>

#include "MainWindow.h"

MainWindow::MainWindow(QMainWindow* parent)
    : QMainWindow(parent), ui{std::make_unique<Ui::MainWindow>()}
{
    ui->setupUi(this);
    
    // Build Icons X and O from Ui::MainWindow form properties
    QVariant o (QObject::property("O_icon"));
    QVariant x (QObject::property("X_icon"));
    o_icon = o.value<QIcon>();
    x_icon = x.value<QIcon>();
    
    // Statusbar
    ui->statusbar->addPermanentWidget(&lab);
    
    //this array will be used to map buttons to coordinates
    btn_tbl = {nullptr, ui->pushButton_1, ui->pushButton_2, ui->pushButton_3, 
               ui->pushButton_4, ui->pushButton_5, ui->pushButton_6, 
               ui->pushButton_7, ui->pushButton_8, ui->pushButton_9};
}

bool MainWindow::check_win()
{
    int score = evaluate();
    if (score == -10) {
        ++win;
        block_cells();
        QMessageBox::information(this, "Congratulations", "You Win!!!");
        
        return true;
    }
    else if (score == +10) {
        ++loose;
        block_cells();
        QMessageBox::warning(this, "Ouch!", "You Loose!!");
        
        return true;
    }
    else if (!has_moves_left()) {
        ++draw;
        block_cells();
        QMessageBox::warning(this, "Ouch!", "DRAW!!!");
        
        return true;
    }
    
    return false;
}

void MainWindow::buttonClicked()
{
    // Your turn 
    auto button = static_cast<QPushButton*>(sender());
    button->setEnabled(false);
    button->setIcon(o_icon);
    int number = button->property("number").toInt();
    auto coord = get_coordinates(number);
    board[coord.first][coord.second] = 'o';
    if (check_win()) {
        lab.setText("Win:"+QString::number(win)+
                    " Loose:"+QString::number(loose)+
                    " Draw:"+QString::number(draw));
        return;
    }
    
    // Computer turn
    auto best = find_best_move();
    board[best.first][best.second] = 'x';
    auto cell = best.first*3 + best.second + 1; // convert (x,y) coord to btn number
    auto button2 = btn_tbl[cell];
       
    button2->setEnabled(false);
    button2->setIcon(x_icon);
    if (check_win()) {
        lab.setText("Win:"+QString::number(win)+
                    " Loose:"+QString::number(loose)+
                    " Draw:"+QString::number(draw));
    }
}

inline void MainWindow::block_cells()
{
    for (auto it=btn_tbl.begin()+1; it != btn_tbl.end(); ++it) {
        (*it)->setEnabled(false);                                      
    }
}

inline std::pair<int,int> MainWindow::get_coordinates(int val)
{ // returns (row, col) coordinate of given cell
    if (val < 1 || val > 9) throw std::runtime_error("");
    return std::pair<int,int> ((val-1)/3, (val-1)%3);
}

inline bool MainWindow::has_moves_left()
{
    for (auto& row: board) 
        for (auto& cell: row) 
            if (cell == ' ') 
                return true;

    return false;
}

// This is the evaluation function
int MainWindow::evaluate()
{
    // Checking for Rows for X or O victory.
    for (auto& row: board) {
        if (std::all_of(row.begin(), row.end(), [&row](auto& a){return a==*row.begin();})){
            if (*row.begin() == 'x') {
                return +10;
            }
            else if (*row.begin() == 'o') {
                return -10;
            }
        }
    }
    
    // Checking for Columns for X or O victory.
    for (decltype(board)::size_type col = 0; col<board.size(); ++col)
    {
        if (board[0][col]==board[1][col] &&
            board[1][col]==board[2][col])
        {
            if (board[0][col]=='x')
                return +10;
 
            else if (board[0][col]=='o')
                return -10;
        }
    }
 
    // Checking for Diagonals for X or O victory.
    if (board[0][0]==board[1][1] && board[1][1]==board[2][2])
    {
        if (board[0][0]=='x')
            return +10;
        else if (board[0][0]=='o')
            return -10;
    }
 
    if (board[0][2]==board[1][1] && board[1][1]==board[2][0])
    {
        if (board[0][2]=='x')
            return +10;
        else if (board[0][2]=='o')
            return -10;
    }
 
    // Else if none of them have won then return 0
    return 0;
}

// This is the minimax function. It considers all
// the possible ways the game can go and returns
// the value of the board
int MainWindow::minimax(int depth, bool isMax)
{
    int score = evaluate();
 
    // If Maximizer has won the game return his/her
    // evaluated score minus depth (to make it smarter)
    if (score == 10)
        return score - depth;
 
    // If Minimizer has won the game return his/her
    // evaluated score plus depth (to make it smarter)
    if (score == -10)
        return score + depth;
 
    // If there are no more moves and no winner then
    // it is a tie
    if (!has_moves_left())
        return 0;
 
    // If this maximizer's move
    if (isMax)
    {
        int best = -1000;
 
        // Traverse all cells
        for (decltype(board)::size_type i = 0; i<board.size(); ++i) {
            for (decltype(i) j = 0; j<board[0].size(); ++j) {
                // Check if cell is empty
                if (board[i][j]==' ') {
                    // Make the move
                    board[i][j] = 'x';
 
                    // Call minimax recursively and choose
                    // the maximum value
                    best = std::max(best, minimax(depth+1, !isMax));
 
                    // Undo the move
                    board[i][j] = ' ';
                }
            }
        }
        return best;
    }
 
    // If this minimizer's move
    else
    {
        int best = 1000;
 
        // Traverse all cells
        for (decltype(board)::size_type i = 0; i<board.size(); ++i) {
            for (decltype(i) j = 0; j<board[0].size(); ++j) {
                // Check if cell is empty
                if (board[i][j]==' ') {
                    // Make the move
                    board[i][j] = 'o';
 
                    // Call minimax recursively and choose
                    // the minimum value
                    best = std::min(best, minimax(depth+1, !isMax));
 
                    // Undo the move
                    board[i][j] = ' ';
                }
            }
        }
        return best;
    }
}

std::pair<int,int> MainWindow::find_best_move()
{
    int bestVal = -1000, row = -1, col = -1;
    
    // Traverse all cells, evalutae minimax function for
    // all empty cells. And return the cell with optimal
    // value.
    for (decltype(board)::size_type i = 0; i<board.size(); ++i) {
        for (decltype(i) j = 0; j<board[0].size(); ++j) {
            // Check if celll is empty
            if (board[i][j]==' ') {
                // Make the move
                board[i][j] = 'x';
 
                // compute evaluation function for this
                // move.
                int moveVal = minimax(0, false);
 
                // Undo the move
                board[i][j] = ' ';
 
                // If the value of the current move is
                // more than the best value, then update
                // best/
                if (moveVal > bestVal) {
                    row = i;
                    col = j;
                    bestVal = moveVal;
                }
            }
        }
    }
    
    return std::pair<int,int>(row, col);
}

void MainWindow::newgame()
{
    for (auto& row: board)
        for (auto& cell: row)
            cell = ' ';
        
    auto icon = QIcon();
        
    for (auto it=btn_tbl.begin()+1; it != btn_tbl.end(); ++it) {
        (*it)->setEnabled(true);
        (*it)->setIcon(icon);
    }
}

void MainWindow::about()
{
    QMessageBox::information(this, "About",
      "TicTacToe version 0.1\nAuthor: Fernando B. Giannasi\nmar/2018");
}
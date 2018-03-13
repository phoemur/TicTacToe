#ifndef TICTACTOE_MAIN_HEADER
#define TICTACTOE_MAIN_HEADER

#include <QMainWindow>
#include <QIcon>
#include <QLabel>
#include <QString>
#include <QPushButton>
#include <memory>
#include <array>
#include <utility>

#include "ui_MainWindow.h"

namespace Ui {
    class MainWindow; // forward declaration
}

class MainWindow : public QMainWindow {
    Q_OBJECT
    std::unique_ptr<Ui::MainWindow> ui;
    QIcon x_icon, o_icon;

    std::array<std::array<char, 3> ,3> board {{{' ', ' ', ' '},
                                               {' ', ' ', ' '},
                                               {' ', ' ', ' '}}};
    QLabel lab {QString("")};
    int win=0, draw=0, loose=0;

    // table to map buttons to coordinates
    std::array<QPushButton*, 10> btn_tbl;

public:
    MainWindow(QMainWindow* parent = nullptr);

private slots:
    void buttonClicked();
    void newgame();
    void about();

private:
    std::pair<int,int> get_coordinates(int val);
    bool has_moves_left();
    int evaluate();
    int minimax(int depth, bool isMax);
    std::pair<int,int> find_best_move();
    bool check_win();
    void block_cells();

}; // class MainWindow

#endif // TICTACTOE_MAIN_HEADER

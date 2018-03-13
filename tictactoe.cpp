#include <QApplication>
#include "MainWindow.h"

int main(int argc, char* argv[])
{
    QApplication app (argc, argv);
    MainWindow win;
    win.move(200, 200);
    win.show();
    
    return app.exec();
}

#include "SeaBattleWindow.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SeaBattleWindow w;
    w.show();
    return a.exec();
}

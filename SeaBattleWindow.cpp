#include "SeaBattleWindow.h"

SeaBattleWindow::SeaBattleWindow(QWidget *parent)
    : QDialog(parent)
{
    ui.setupUi(this);
    setWindowFlag(Qt::WindowContextHelpButtonHint, false);
    setWindowFlag(Qt::WindowMinimizeButtonHint, true);
    ui.fvPlayer->initShips();
}

void SeaBattleWindow::on_bWaitConnection_clicked()
{
}

void SeaBattleWindow::on_bConnect_clicked()
{
}

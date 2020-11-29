#include "SeaBattleWindow.h"

SeaBattleWindow::SeaBattleWindow(QWidget *parent)
    : QDialog(parent)
{
    ui.setupUi(this);
    setWindowFlag(Qt::WindowContextHelpButtonHint, false);
    setWindowFlag(Qt::WindowMinimizeButtonHint, true);
    ui.fvPlayer->initShips();
    ui.fvEnemy->enableAim();
}

void SeaBattleWindow::on_bWaitConnection_clicked()
{
}

void SeaBattleWindow::on_bConnect_clicked()
{
}

void SeaBattleWindow::on_fvEnemy_playerShot(int i, int j)
{
    // Для отладки
    ui.fvEnemy->playerShotResult(i, j, ui.fvPlayer->enemyShot(i, j));
    if (!ui.fvPlayer->gameOver())
        ui.fvEnemy->enableAim();
}

#include "SeaBattleWindow.h"
#include "SeaBattleControl.h"
#include <QMessageBox>

SeaBattleWindow::SeaBattleWindow(QWidget *parent)
    : QDialog(parent)
{
    ui.setupUi(this);
    setWindowFlag(Qt::WindowContextHelpButtonHint, false);
    setWindowFlag(Qt::WindowMinimizeButtonHint, true);
    control = new SeaBattleControl(this);
    control->initGame();
}

SeaBattleWindow::~SeaBattleWindow()
{
    delete control;
}

void SeaBattleWindow::on_bWaitConnection_clicked()
{
    if (ui.bWaitConnection->text() == "Ждать подключения")
    {
        ui.leIPAddress->hide();
        ui.bConnect->hide();
        ui.bWaitConnection->setText("Остановить ожидание");
        QApplication::processEvents();
        control->startServer();
    }
    else
    {
        control->stopServer();
        restoreButtons();
        restoreFields();
    }
}

void SeaBattleWindow::on_bConnect_clicked()
{
    if (ui.bConnect->text() == "Подключиться")
    {
        QString ipAddress = ui.leIPAddress->text();
        ui.leIPAddress->setDisabled(true);
        ui.bWaitConnection->hide();
        ui.bConnect->setText("Остановить");
        QApplication::processEvents();
        control->connectClient(ipAddress);
    }
    else
    {
        control->disconnectClient();
        restoreButtons();
        restoreFields();
    }
}

void SeaBattleWindow::gameOver(bool victory)
{
    if (ui.bWaitConnection->isVisible())
        control->stopServer();
    else
        control->disconnectClient();
    restoreButtons();
    QMessageBox::information(this, "Игра окончена",
        victory ? "Поздравляю, Вы победили!" : "К сожалению, Вы проиграли!");
    restoreFields();
}

void SeaBattleWindow::serverConnected()
{
    ui.bWaitConnection->setText("Отключиться");
}

void SeaBattleWindow::clientConnected()
{
    ui.bConnect->setText("Отключиться");
}

void SeaBattleWindow::networkError(const QString& text)
{
    if (ui.bWaitConnection->isVisible())
        control->stopServer();
    else
        control->disconnectClient();
    restoreButtons();
    QMessageBox::critical(this, "Ошибка", text);
    restoreFields();
}

void SeaBattleWindow::restoreButtons()
{
    ui.bWaitConnection->setText("Ждать подключения");
    ui.bWaitConnection->show();
    ui.leIPAddress->setEnabled(true);
    ui.leIPAddress->show();
    ui.bConnect->setText("Подключиться");
    ui.bConnect->show();
}

void SeaBattleWindow::restoreFields()
{
    ui.fvEnemy->disableAim();
    ui.fvEnemy->clearField();
    ui.fvPlayer->restoreShips();
}

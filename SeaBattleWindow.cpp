#include "SeaBattleWindow.h"
#include "SeaBattleControl.h"
#include "SettingsDialog.h"

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
        ui.bWaitConnection->setText("Остановить ожидание");
        ui.bSettings->hide();
        ui.lAddress->hide();
        ui.leAddress->hide();
        ui.bConnect->hide();
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

void SeaBattleWindow::on_bSettings_clicked()
{
    SettingsDialog* dlg = new SettingsDialog(this);
    dlg->setPort(control->getPort());
    dlg->setFirstMove(control->getFirstMove());
    if (QDialog::Accepted == dlg->exec())
    {
        control->setPort(dlg->getPort());
        control->setFirstMove(dlg->getFirstMove());
    }
}

void SeaBattleWindow::on_bConnect_clicked()
{
    if (ui.bConnect->text() == "Подключиться")
    {
        QString address = ui.leAddress->text();
        if (address.isEmpty())
        {
            messageBox(QMessageBox::Warning, "Ошибка",
                "Введите IP-адрес или DNS-имя узла для подключения");
        }
        else
        {
            ui.bWaitConnection->hide();
            ui.bSettings->hide();
            ui.lAddress->setDisabled(true);
            ui.leAddress->setDisabled(true);
            ui.bConnect->setText("Остановить");
            QApplication::processEvents();
            control->connectClient(address);
        }
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
    messageBox(QMessageBox::Information, "Игра окончена",
        victory ? "Вы победили!" : "Вы проиграли!");
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
    messageBox(QMessageBox::Critical, "Ошибка", text);
    restoreFields();
}

void SeaBattleWindow::messageBox(QMessageBox::Icon icon, const QString& title, const QString& text)
{
    QMessageBox* mb = new QMessageBox(icon, title, text, QMessageBox::Ok, this);
    mb->setFont(ui.lAddress->font());
    mb->exec();
    delete mb;
}

void SeaBattleWindow::restoreButtons()
{
    ui.bWaitConnection->setText("Ждать подключения");
    ui.bWaitConnection->show();
    ui.bSettings->show();
    ui.lAddress->setEnabled(true);
    ui.lAddress->show();
    ui.leAddress->setEnabled(true);
    ui.leAddress->show();
    ui.bConnect->setText("Подключиться");
    ui.bConnect->show();
}

void SeaBattleWindow::restoreFields()
{
    ui.fvEnemy->disableAim();
    ui.fvEnemy->clearField();
    ui.fvPlayer->restoreShips();
}

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
    ui.fvPlayer->initShips();
}

SeaBattleWindow::~SeaBattleWindow()
{
    delete control;
}

void SeaBattleWindow::showStartMessage()
{
    messageBox(QMessageBox::Information, windowTitle(),
        "<html><head/><body>"
        "<p>Для начала расставьте Ваши корабли по игровому полю:</p>"
        "<ul><li>1 корабль из четырёх клеток (авианосец),</li>"
        "<li>2 корабля из трёх клеток (линкоры),</li>"
        "<li>3 корабля из двух клеток (эсминцы),</li>"
        "<li>4 корабля из одной клетки (торпедные катера).</li></ul>"
        "<p>Корабли не должны касаться друг друга!</p>"
        "<p>Затем нажмите кнопку <i>\"Ждать подключения\"</i> "
        "или введите адрес и нажмите кнопку <i>\"Подключиться\"</i>, чтобы начать игру. "
        "Один из игроков должен ждать подключения, а другой - подключаться.</p>"
        "</body></html>");
    ui.fvPlayer->enableEditor();
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

void SeaBattleWindow::on_fvPlayer_validPlacementChanged(bool valid)
{
    ui.bWaitConnection->setEnabled(valid);
    ui.lAddress->setEnabled(valid);
    ui.leAddress->setEnabled(valid);
    ui.bConnect->setEnabled(valid);
}

void SeaBattleWindow::gameOver(bool victory)
{
    messageBox(QMessageBox::Information, "Игра окончена",
        victory ? "Вы победили!" : "Вы проиграли!");
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
    if (ui.fvPlayer->gameOver() || ui.fvEnemy->gameOver())
        messageBox(QMessageBox::Information, "Игра окончена", text);
    else
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
    ui.fvPlayer->enableEditor();
}

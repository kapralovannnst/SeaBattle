#pragma once

#include <QtWidgets/QDialog>
#include "ui_SeaBattleWindow.h"

class SeaBattleControl;

// Главное окно игры "Морской бой"
class SeaBattleWindow : public QDialog
{
    Q_OBJECT

public:
    SeaBattleWindow(QWidget *parent = Q_NULLPTR);
    ~SeaBattleWindow();

    FieldView* getPlayerFV() { return ui.fvPlayer; }
    FieldView* getEnemyFV() { return ui.fvEnemy; }

public slots:
    // Нажатие на кнопку "Ждать подключения"
    void on_bWaitConnection_clicked();
    // Нажатие на кнопку "Подключиться"
    void on_bConnect_clicked();
    // Игра окончена
    void gameOver(bool victory);
    // Подключение к серверу
    void serverConnected();
    // Подключение клиента
    void clientConnected();
    // Ошибки сервера и клиента
    void networkError(const QString& text);

private:
    // Восстановить начальное состояние кнопок
    void restoreButtons();
    // Восстановить игровые поля
    void restoreFields();
    // GUI
    Ui::SeaBattleWindowClass ui;
    // Управляющий объект
    SeaBattleControl* control;
};

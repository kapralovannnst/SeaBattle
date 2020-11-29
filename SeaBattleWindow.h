#pragma once

#include <QtWidgets/QDialog>
#include "ui_SeaBattleWindow.h"

// Главное окно игры "Морской бой"
class SeaBattleWindow : public QDialog
{
    Q_OBJECT

public:
    SeaBattleWindow(QWidget *parent = Q_NULLPTR);

public slots:
    // Нажатие на кнопку "Ждать подключения"
    void on_bWaitConnection_clicked();
    // Нажатие на кнопку "Подключиться"
    void on_bConnect_clicked();
    // Обработка выстрела игрока
    void on_fvEnemy_playerShot(int i, int j);

private:
    Ui::SeaBattleWindowClass ui;
};

#pragma once

#include <QDialog>
#include "ui_SettingsDialog.h"

// Диалоговое окно с настройками игры
class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    SettingsDialog(QWidget *parent = Q_NULLPTR);
    ~SettingsDialog();

    // Вернуть порт
    unsigned short getPort() const { return ui.sbPort->value(); }
    // Вернуть первый ход
    unsigned int getFirstMove() const { return ui.cbFirstMove->currentIndex(); }
    // Установить порт
    void setPort(unsigned short p) { ui.sbPort->setValue(p); }
    // Установить первый ход
    void setFirstMove(unsigned int fm) { ui.cbFirstMove->setCurrentIndex(fm); }

private:
    Ui::SettingsDialog ui;
};

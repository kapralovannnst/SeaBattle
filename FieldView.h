#pragma once

#include <QFrame>
#include <QPainter>
#include <QPen>
#include <QBrush>
#include "Field.h"

// Виджет для просмотра игрового поля
class FieldView : public QFrame
{
    Q_OBJECT

public:
    FieldView(QWidget* parent = Q_NULLPTR);
    ~FieldView();

    // Вернуть указатель на игровое поле
    Field* getField() { return field; }
    // Начальная расстановка кораблей
    void initShips() { field->initShips(); }
    // Игра окончена?
    bool gameOver() { return field->gameOver(); }
    // Восстановить корабли
    void restoreShips();
    // Очистить поле
    void clearField();
    // Включить режим прицеливания
    void enableAim();
    // Выключить режим прицеливания
    void disableAim();
    // Обработка выстрела противника
    unsigned int enemyShot(int i, int j);
    // Обработка результата выстрела игрока
    void playerShotResult(int i, int j, unsigned int result);

signals:
    // Сигнал - выстрел игрока
    void playerShot(int i, int j);

protected:
    // Событие рисования
    void paintEvent(QPaintEvent *e);
    // Событие перемещения мышки
    void mouseMoveEvent(QMouseEvent *e);
    // Событие нажатия кнопки мышки
    void mousePressEvent(QMouseEvent* e);

private:
    // Нарисовать поле
    void drawField(QPainter& p);
    // Нарисовать прицел
    void drawAim(QPainter& p);
    // Для рисования виджета
    static QPen shipPen;
    static QPen missPen;
    static QPen hitPen;
    static QPen aimPen;
    static QPen aimFramePen;
    static QBrush shipBrush;
    static QBrush missBrush;
    // Игровое поле
    Field* field;
    // Режим прицеливания
    bool aim;
    // Позиция прицела
    int aim_i;
    int aim_j;
};

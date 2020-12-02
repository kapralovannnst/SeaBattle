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
    // Включить режим редактирования
    void enableEditor();
    // Выключить режим редактирования
    void disableEditor();
    // Обработка выстрела противника
    unsigned int enemyShot(int i, int j);
    // Обработка результата выстрела игрока
    void playerShotResult(int i, int j, unsigned int result);

signals:
    // Сигнал - выстрел игрока
    void playerShot(int i, int j);
    // Сигнал - изменилась правильность расположения кораблей
    void validPlacementChanged(bool valid);

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
    // Мигание клетки выстрела
    void animateFire(int i, int j);
    // Рисование для редактора размещения кораблей
    void drawEditor(QPainter& p);

    // Для рисования виджета
    static QPen shipPen;
    static QPen missPen;
    static QPen hitPen;
    static QPen aimPen;
    static QPen aimFramePen;
    static QPen editorFramePen;
    static QBrush shipBrush;
    static QBrush missBrush;
    static QBrush editorBrush;

    // Игровое поле
    Field* field;
    // Режим прицеливания
    bool aim;
    // Режим редактирования
    bool editor;
    // Правильность размещения кораблей
    bool validPlacement;
    // Позиция курсора
    int cur_i;
    int cur_j;
    // Координаты выстрела противника
    int shot_i;
    int shot_j;
    // Подсветить клетку выстрела
    bool fire;
};

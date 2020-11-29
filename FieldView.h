#pragma once

#include <QFrame>
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

protected:
    // Событие рисования
    void paintEvent(QPaintEvent *e);

private:
    // Нарисовать поле
    void drawField();
    // Для рисования виджета
    static QPen shipPen;
    static QPen missPen;
    static QPen hitPen;
    static QBrush shipBrush;
    static QBrush missBrush;
    // Игровое поле
    Field *field;
};

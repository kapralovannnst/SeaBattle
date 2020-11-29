#include "FieldView.h"
#include <QPainter>

QPen FieldView::shipPen(Qt::blue, 3, Qt::SolidLine, Qt::SquareCap, Qt::MiterJoin);
QPen FieldView::missPen(Qt::black, 1);
QPen FieldView::hitPen(Qt::red, 3, Qt::SolidLine, Qt::RoundCap);
QBrush FieldView::shipBrush("#A0A0FF");
QBrush FieldView::missBrush(Qt::black);

FieldView::FieldView(QWidget *parent)
    : QFrame(parent)
{
    field = new Field;
}

FieldView::~FieldView()
{
    delete field;
}

void FieldView::paintEvent(QPaintEvent* e)
{
    Q_UNUSED(e);
    drawField();
    QFrame::paintEvent(e);
}

void FieldView::drawField()
{
    QPainter painter(this);

    // Фон
    painter.fillRect(0, 0, width(), height(), Qt::white);
    
    // Рисование сетки
    painter.setPen(Qt::blue);
    for (int i = 30; i <= 330; i += 30)
    {
        painter.drawLine(30, i, 330, i);
        painter.drawLine(i, 30, i, 330);
    }

    // Рисование клеток
    for (int i = 0; i < 10; i++)
        for (int j = 0; j < 10; j++)
        {
            int x = 30 * j + 30;
            int y = 30 * i + 30;
            unsigned int st = field->get(i, j);

            if (Field::ship == st || Field::hit == st)
            {
                painter.setBrush(shipBrush);
                painter.setPen(shipPen);
                painter.drawRect(x, y, 30, 30);
                if (Field::hit == st)
                {
                    painter.setPen(hitPen);
                    painter.drawLine(x + 3, y + 3, x + 28, y + 28);
                    painter.drawLine(x + 3, y + 28, x + 28, y + 3);
                }
            }
            else if (Field::miss == st)
            {
                painter.setBrush(missBrush);
                painter.setPen(missPen);
                painter.drawEllipse(x + 12, y + 12, 6, 6);
            }
        }
}

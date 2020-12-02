#include "FieldView.h"
#include <QMouseEvent>
#include <QThread>

QPen FieldView::shipPen(Qt::blue, 3, Qt::SolidLine, Qt::SquareCap, Qt::MiterJoin);
QPen FieldView::missPen(Qt::black, 1);
QPen FieldView::hitPen(Qt::red, 3, Qt::SolidLine, Qt::RoundCap);
QPen FieldView::aimPen(Qt::green, 1);
QPen FieldView::aimFramePen(Qt::green, 3, Qt::SolidLine, Qt::SquareCap, Qt::MiterJoin);
QPen FieldView::editorFramePen(Qt::red, 3, Qt::SolidLine, Qt::SquareCap, Qt::MiterJoin);
QBrush FieldView::shipBrush("#A0A0FF");
QBrush FieldView::missBrush(Qt::black);
QBrush FieldView::editorBrush(Qt::green, Qt::DiagCrossPattern);

FieldView::FieldView(QWidget *parent)
    : QFrame(parent)
{
    field = new Field;
    aim = false;
    editor = false;
    validPlacement = true;
    cur_i = -1;
    cur_j = -1;
    shot_i = -1;
    shot_j = -1;
    fire = false;
}

FieldView::~FieldView()
{
    delete field;
}

void FieldView::restoreShips()
{
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            switch (field->get(i, j))
            {
            case Field::hit:
                field->set(i, j, Field::ship);
                break;
            case Field::miss:
                field->set(i, j, Field::empty);
                break;
            }
        }
    }
    repaint();
}

void FieldView::clearField()
{
    field->clear();
    repaint();
}

void FieldView::enableAim()
{
    aim = true;
    repaint();
    setMouseTracking(true);
}

void FieldView::disableAim()
{
    setMouseTracking(false);
    unsetCursor();
    aim = false;
    cur_i = -1;
    cur_j = -1;
    repaint();
}

void FieldView::enableEditor()
{
    editor = true;
    repaint();
    setMouseTracking(true);
}

void FieldView::disableEditor()
{
    setMouseTracking(false);
    unsetCursor();
    editor = false;
    cur_i = -1;
    cur_j = -1;
    repaint();
}

unsigned int FieldView::enemyShot(int i, int j)
{
    unsigned int result = field->shot(i, j);
    animateFire(i, j);
    return result;
}

void FieldView::playerShotResult(int i, int j, unsigned int result)
{
    if (Field::death == result)
    {
        field->set(i, j, Field::hit);
        field->setDeadShipBorder(i, j);
    }
    else
        field->set(i, j, result);
    repaint();
}

void FieldView::paintEvent(QPaintEvent* e)
{
    Q_UNUSED(e);

    QPainter painter(this);
    drawField(painter);
    if (aim)
        drawAim(painter);
    if (editor)
        drawEditor(painter);
    QFrame::paintEvent(e);
}

void FieldView::mouseMoveEvent(QMouseEvent* e)
{
    if (aim || editor)
    {
        int old_i = cur_i, old_j = cur_j;
        cur_i = e->y() / 30 - 1;
        cur_j = e->x() / 30 - 1;
        if (cur_i < 0 || cur_i > 9 || cur_j < 0 || cur_j > 9)
        {
            cur_i = -1;
            cur_j = -1;
        }

        if (cur_i != old_i || cur_j != old_j)
            repaint();
    }
}

void FieldView::mousePressEvent(QMouseEvent* e)
{
    if (aim && Qt::LeftButton == e->button())
    {
        if (cur_i >= 0 && cur_j >= 0 && Field::empty == field->get(cur_i, cur_j))
        {
            int i = cur_i, j = cur_j;
            disableAim();
            emit playerShot(i, j);
            animateFire(i, j);
        }
    }

    if (editor && Qt::LeftButton == e->button())
    {
        if (cur_i >= 0 && cur_j >= 0)
        {
            bool old = validPlacement;
            field->toggle(cur_i, cur_j);
            validPlacement = field->validPlacement();
            repaint();
            if (validPlacement != old)
                emit validPlacementChanged(validPlacement);
        }
    }
}

void FieldView::drawField(QPainter& p)
{
    // Фон
    p.fillRect(0, 0, width(), height(), Qt::white);
    
    // Рисование сетки
    p.setPen(Qt::blue);
    for (int i = 30; i <= 330; i += 30)
    {
        p.drawLine(30, i, 330, i);
        p.drawLine(i, 30, i, 330);
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
                p.setBrush(shipBrush);
                p.setPen(shipPen);
                p.drawRect(x, y, 30, 30);
                if (Field::hit == st)
                {
                    p.setPen(hitPen);
                    p.drawLine(x + 3, y + 3, x + 28, y + 28);
                    p.drawLine(x + 3, y + 28, x + 28, y + 3);
                }
            }
            else if (Field::miss == st)
            {
                p.setBrush(missBrush);
                p.setPen(missPen);
                p.drawEllipse(x + 12, y + 12, 6, 6);
            }
        }

    // Подсветка клетки выстрела
    if (fire)
        p.fillRect(shot_j * 30 + 31, shot_i * 30 + 31, 29, 29, Qt::red);
}

void FieldView::drawAim(QPainter& p)
{
    // Рамка прицела
    p.setPen(aimFramePen);
    p.setBrush(Qt::transparent);
    p.drawRect(30, 30, 300, 300);

    if (cur_i >= 0 && cur_j >= 0 && Field::empty == field->get(cur_i, cur_j))
    {
        // Прицел
        int x = 30 * cur_j + 30;
        int y = 30 * cur_i + 30;
        p.setPen(aimPen);
        p.drawEllipse(x + 3, y + 3, 24, 24);
        p.drawEllipse(x + 6, y + 6, 18, 18);
        p.drawEllipse(x + 9, y + 9, 12, 12);
        p.drawLine(x + 15, 30, x + 15, y + 12);
        p.drawLine(x + 15, y + 18, x + 15, 330);
        p.drawLine(30, y + 15, x + 12, y + 15);
        p.drawLine(x + 18, y + 15, 330, y + 15);
        setCursor(Qt::BlankCursor);
    }
    else
    {
        unsetCursor();
    }
}

void FieldView::animateFire(int i, int j)
{
    shot_i = i;
    shot_j = j;
    fire = true;
    repaint();
    for (int f = 0; f < 5; f++)
    {
        QThread::msleep(100);
        fire = f & 1;
        repaint();
    }
}

void FieldView::drawEditor(QPainter& p)
{
    // Большая рамка
    if (validPlacement)
        p.setPen(aimFramePen);
    else
        p.setPen(editorFramePen);
    p.setBrush(Qt::transparent);
    p.drawRect(30, 30, 300, 300);

    if (cur_i >= 0 && cur_j >= 0)
    {
        // Маленькая зелёная рамка
        p.setPen(aimFramePen);
        p.setBrush(editorBrush);
        p.drawRect(30 * cur_j + 30, 30 * cur_i + 30, 30, 30);
        setCursor(Qt::PointingHandCursor);
    }
    else
    {
        unsetCursor();
    }
}

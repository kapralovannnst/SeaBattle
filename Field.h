#pragma once
#include <cassert>  // assert

// Поле для игры (10x10 клеток)
class Field
{
public:
    static const unsigned int empty = 0;    // Пустая клетка
    static const unsigned int ship  = 1;    // Корабль
    static const unsigned int miss  = 2;    // Промах
    static const unsigned int hit   = 3;    // Попадание
    static const unsigned int death = 4;    // Гибель корабля - только код возрата для shot

    Field();
    ~Field();

    // Вернуть состояние клетки
    unsigned int get(int i, int j) const
    {
        assert((i >= 0) && (i < 10) && (j >= 0) && (j < 10));
        return data[i][j];
    }

    // Установить состояние клетки
    void set(int i, int j, unsigned int state)
    {
        assert((i >= 0) && (i < 10) && (j >= 0) && (j < 10));
        assert(state <= hit);
        data[i][j] = state;
    }

    // Очистить поле
    void clear();
    // Начальная расстановка кораблей
    void initShips();
    // Переключить клетку (корабль/пусто)
    void toggle(int i, int j);
    // Правильное размещение кораблей?
    bool validPlacement();
    // Обработка выстрела противника
    unsigned int shot(int i, int j);
    // Игра окончена?
    bool gameOver() const;
    // Поставить точки-промахи вокруг погибшего корабля
    void setDeadShipBorder(int i, int j);

private:
    // Корабль погиб?
    bool isDeadShip(int i, int j) const;
    // Данные поля
    unsigned char data[10][10];
};


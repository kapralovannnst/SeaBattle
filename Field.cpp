#include "Field.h"
#include <cstring>  // memset

Field::Field()
{
    clear();
}

Field::~Field()
{
}

void Field::clear()
{
    memset(data, 0, sizeof(data));
}

void Field::initShips()
{
    for (int j = 1; j <= 4; j++)
        data[1][j] = ship;
    for (int j = 1; j <= 7; j++)
        if (j != 4)
            data[3][j] = ship;
    for (int j = 1; j <= 8; j++)
        if (j != 3 && j != 6)
            data[5][j] = ship;
    for (int j = 1; j <= 7; j++)
        if (j & 1)
            data[7][j] = ship;
}

void Field::toggle(int i, int j)
{
    switch (get(i, j))
    {
    case empty:
        data[i][j] = ship;
        break;
    case ship:
        data[i][j] = empty;
        break;
    }
}

bool Field::validPlacement()
{
    // 1. Кол-во клеток-кораблей должно быть 20
    int count = 0;
    for (int i = 0; i < 10; i++)
        for (int j = 0; j < 10; j++)
            if (ship == data[i][j])
                ++count;
    if (count != 20)
        return false;
    // 2. Не должно быть клеток-кораблей, касающихся углами друг друга
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 10; j++)
        {
            if (ship == data[i][j])
            {
                if (j > 0 && ship == data[i + 1][j - 1])
                    return false;
                if (j < 9 && ship == data[i + 1][j + 1])
                    return false;
            }
        }
    // 3. Должен быть 1 корабль из 4-х клеток, 2 из 3-х, 3 и 2-х
    int c2 = 0;
    int c3 = 0;
    int c4 = 0;
    // по горизонтали
    for (int i = 0; i < 10; i++)
    {
        count = 0;
        for (int j = 0; j < 10; j++)
        {
            if (ship == data[i][j])
            {
                ++count;
                continue;
            }
            if (empty == data[i][j])
            {
                switch (count)
                {
                case 0:
                case 1: break;
                case 2: ++c2; break;
                case 3: ++c3; break;
                case 4: ++c4; break;
                default: return false;
                }
                count = 0;
            }
        }
        switch (count)
        {
        case 0:
        case 1: break;
        case 2: ++c2; break;
        case 3: ++c3; break;
        case 4: ++c4; break;
        default: return false;
        }
    }

    // по вертикали
    for (int j = 0; j < 10; j++)
    {
        count = 0;
        for (int i = 0; i < 10; i++)
        {
            if (ship == data[i][j])
            {
                ++count;
                continue;
            }
            if (empty == data[i][j])
            {
                switch (count)
                {
                case 0:
                case 1: break;
                case 2: ++c2; break;
                case 3: ++c3; break;
                case 4: ++c4; break;
                default: return false;
                }
                count = 0;
            }
        }
        switch (count)
        {
        case 0:
        case 1: break;
        case 2: ++c2; break;
        case 3: ++c3; break;
        case 4: ++c4; break;
        default: return false;
        }
    }
    // проверка кол-ва
    if (c2 != 3 || c3 != 2 || c4 != 1)
        return false;

    return true;
}

unsigned int Field::shot(int i, int j)
{
    unsigned int s = get(i, j);
    switch (s)
    {
    case empty:
        data[i][j] = miss;
    case miss:
        return miss;
    case ship:
        data[i][j] = hit;
    case hit:
        if (isDeadShip(i, j))
        {
            setDeadShipBorder(i, j);
            return death;
        }
        return hit;
    }
    return s;
}

bool Field::gameOver() const
{
    // Игра окончена, когда все корабли уничтожены
    int count = 0;
    for (int i = 0; i < 10; i++)
        for (int j = 0; j < 10; j++)
            if (hit == data[i][j])
                ++count;
    return (20 == count);
}

void Field::setDeadShipBorder(int i, int j)
{
    int up = i, left = j, down = i, right = j;

    if (i > 0)  // вверх
    {
        --up;
        if (hit == data[i - 1][j] && (i > 1))
        {
            --up;
            if ((hit == data[i - 2][j]) && (i > 2))
            {
                --up;
                if ((hit == data[i - 3][j]) && (i > 3))
                    --up;
            }
        }
    }
    
    if (i < 9)  // вниз
    {
        ++down;
        if (hit == data[i + 1][j] && (i < 8))
        {
            ++down;
            if ((hit == data[i + 2][j]) && (i < 7))
            {
                ++down;
                if ((hit == data[i + 3][j]) && (i < 6))
                    ++down;
            }
        }
    }

    if (j > 0)  // влево
    {
        --left;
        if (hit == data[i][j - 1] && (j > 1))
        {
            --left;
            if ((hit == data[i][j - 2]) && (j > 2))
            {
                --left;
                if ((hit == data[i][j - 3]) && (j > 3))
                    --left;
            }
        }
    }

    if (j < 9)  // вправо
    {
        ++right;
        if (hit == data[i][j + 1] && (j < 8))
        {
            ++right;
            if ((hit == data[i][j + 2]) && (j < 7))
            {
                ++right;
                if ((hit == data[i][j + 3]) && (j < 6))
                    ++right;
            }
        }
    }

    assert((up >= 0) && (up < 10));
    assert((down >= 0) && (down < 10));
    assert((left >= 0) && (left < 10));
    assert((right >= 0) && (right < 10));

    // Поставить промахи во все пустые клетки вокруг корабля
    for (i = up; i <= down; i++)
        for (j = left; j <= right; j++)
            if (empty == data[i][j])
                data[i][j] = miss;
}

bool Field::isDeadShip(int i, int j) const
{
    if (i > 0)  // вверх
    {
        if (ship == data[i - 1][j])
            return false;
        if ((hit == data[i - 1][j]) && (i > 1))
        {
            if (ship == data[i - 2][j])
                return false;
            if ((hit == data[i - 2][j]) && (i > 2))
            {
                if (ship == data[i - 3][j])
                    return false;
            }
        }
    }

    if (i < 9)  // вниз
    {
        if (ship == data[i + 1][j])
            return false;
        if ((hit == data[i + 1][j]) && (i < 8))
        {
            if (ship == data[i + 2][j])
                return false;
            if ((hit == data[i + 2][j]) && (i < 7))
            {
                if (ship == data[i + 3][j])
                    return false;
            }
        }
    }

    if (j > 0)  // влево
    {
        if (ship == data[i][j - 1])
            return false;
        if ((hit == data[i][j - 1]) && (j > 1))
        {
            if (ship == data[i][j - 2])
                return false;
            if ((hit == data[i][j - 2]) && (j > 2))
            {
                if (ship == data[i][j - 3])
                    return false;
            }
        }
    }

    if (j < 9)  // вправо
    {
        if (ship == data[i][j + 1])
            return false;
        if ((hit == data[i][j + 1]) && (j < 8))
        {
            if (ship == data[i][j + 2])
                return false;
            if ((hit == data[i][j + 2]) && (j < 7))
            {
                if (ship == data[i][j + 3])
                    return false;
            }
        }
    }

    return true;
}

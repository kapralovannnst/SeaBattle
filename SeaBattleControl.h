#pragma once

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>

class SeaBattleWindow;

// Управление игрой "Морской бой"
class SeaBattleControl : public QObject
{
    Q_OBJECT

public:
    // Сетевой порт по умолчанию
    static const unsigned short defaultPort = 57382;

    // Режим выбора первого хода (только сервер)
    static const unsigned int firstMoveRandom = 0;  // Случайно
    static const unsigned int firstMoveServer = 1;  // Сервер
    static const unsigned int firstMoveClient = 2;  // Клиент

    SeaBattleControl(SeaBattleWindow* parent);
    ~SeaBattleControl();

    // Получить порт
    unsigned short getPort() const { return port; }
    // Установить порт
    void setPort(unsigned short p) { port = p; }
    // Получить режим выбора первого хода
    unsigned int getFirstMove() const { return firstMove; }
    // Установить режим выбора первого хода
    void setFirstMove(unsigned int fm)
    {
        if (fm <= firstMoveClient)
            firstMove = fm;
    }

    // Инициализация игры
    void initGame();
    // Запустить сервер
    void startServer();
    // Остановить сервер
    void stopServer();
    // Подключиться к серверу
    void connectClient(const QString& address);
    // Отключиться от сервера
    void disconnectClient();

signals:
    // Сигнал - игра окончена
    void gameOver(bool victory);
    // Сигнал - ошибка сети
    void networkError(const QString& text);

private slots:
    // Выстрел игрока
    void playerShot(int i, int j);
    // Новое соединение с сервером
    void serverNewConnection();
    // Ошибка сервера
    void serverAcceptError();
    // Клиент подключился
    void clientConnected();
    // Ошибка сокета
    void errorOccurred(QAbstractSocket::SocketError socketError);
    // Получение данных по сети
    void socketReadyRead();

private:
    // Сообщить, кто ходит первым
    void sendFirstMove();
    // Принять сообщение о первом ходе
    void receiveFirstMove();
    // Принять выстрел
    void receiveShot();
    // Отправить результат выстрела
    void sendShotResult(unsigned int r);
    // Принять результат выстрела
    void receiveShotResult();
    // Отправить данные игрового поля
    void sendFieldData();
    // Принять данные игрового поля
    void receiveFieldData();

    // Константы для mode:
    static const int waitFirstMove  = 1;    // Ждать сообщение о первом ходе
    static const int waitShot       = 2;    // Ждать координаты выстрела
    static const int waitShotResult = 3;    // Ждать результат выстрела
    static const int waitFieldData  = 4;    // Ждать данные о поле противника

    // Главное окно игры
    SeaBattleWindow* window;
    // Сетевой порт
    unsigned short port;
    // Режим выбора первого хода
    unsigned int firstMove;
    // Ход (true - игрок, false - противник)
    bool move;
    // Режим ожидания данных
    int mode;
    // Координаты выстрела игрока
    int shot_i;
    int shot_j;
    // Объект-сервер
    QTcpServer tcpServer;
    // Указатель на сокет для передачи данных
    QTcpSocket* socket;
};

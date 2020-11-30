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
    // Порт сервера
    static const unsigned short serverPort = 57382;

    SeaBattleControl(SeaBattleWindow* parent);
    ~SeaBattleControl();

    // Инициализация игры
    void initGame();
    // Запустить сервер
    void startServer();
    // Остановить сервер
    void stopServer();
    // Подключиться к серверу
    void connectClient(const QString& ipAddress);
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

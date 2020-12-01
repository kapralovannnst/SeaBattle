#include "SeaBattleControl.h"
#include "SeaBattleWindow.h"
#include <QRandomGenerator>
#include <QDebug>

SeaBattleControl::SeaBattleControl(SeaBattleWindow* parent)
    : QObject(parent)
{
    window = parent;
    port = defaultPort;
    firstMove = firstMoveRandom;
    move = false;
    mode = 0;
    shot_i = -1;
    shot_j = -1;
    socket = nullptr;
    connect(window->getEnemyFV(), SIGNAL(playerShot(int, int)), this, SLOT(playerShot(int, int)));
    connect(this, SIGNAL(gameOver(bool)), window, SLOT(gameOver(bool)), Qt::QueuedConnection);
    connect(this, SIGNAL(networkError(const QString&)), window, SLOT(networkError(const QString&)), Qt::QueuedConnection);
    connect(&tcpServer, SIGNAL(newConnection()), this, SLOT(serverNewConnection()));
    connect(&tcpServer, SIGNAL(acceptError()), this, SLOT(serverAcceptError()));
}

SeaBattleControl::~SeaBattleControl()
{
    if (socket)
        delete socket;
}

void SeaBattleControl::initGame()
{
    window->getPlayerFV()->initShips();
}

void SeaBattleControl::startServer()
{
    tcpServer.listen(QHostAddress::Any, port);
}

void SeaBattleControl::stopServer()
{
    mode = 0;
    if (socket)
    {
        socket->disconnect(SIGNAL(readyRead()));
        socket->disconnect(SIGNAL(errorOccurred(QAbstractSocket::SocketError)));
        socket->close();
        delete socket;
        socket = nullptr;
    }
    if (tcpServer.isListening())
        tcpServer.close();
}

void SeaBattleControl::connectClient(const QString& address)
{
    socket = new QTcpSocket;
    connect(socket, SIGNAL(connected()), this, SLOT(clientConnected()));
    connect(socket, SIGNAL(errorOccurred(QAbstractSocket::SocketError)),
        this, SLOT(errorOccurred(QAbstractSocket::SocketError)));
    socket->connectToHost(address, port);
}

void SeaBattleControl::disconnectClient()
{
    mode = 0;
    socket->disconnect(SIGNAL(connected()));
    socket->disconnect(SIGNAL(errorOccurred(QAbstractSocket::SocketError)));
    socket->disconnect(SIGNAL(readyRead()));
    socket->close();
    delete socket;
    socket = nullptr;
}

void SeaBattleControl::playerShot(int i, int j)
{
    shot_i = i;
    shot_j = j;
    char x = i * 10 + j;
    mode = waitShotResult;
    socket->write(&x, 1);
    socket->flush();
}

void SeaBattleControl::serverNewConnection()
{
    socket = tcpServer.nextPendingConnection();
    if (socket)
    {
        tcpServer.close();
        sendFirstMove();
        connect(socket, SIGNAL(readyRead()), this, SLOT(socketReadyRead()));
        connect(socket, SIGNAL(errorOccurred(QAbstractSocket::SocketError)),
            this, SLOT(errorOccurred(QAbstractSocket::SocketError)));
        if (move)
            window->getEnemyFV()->enableAim();
        else
            mode = waitShot;
        window->serverConnected();
    }
}

void SeaBattleControl::serverAcceptError()
{
    emit networkError(tcpServer.errorString());
}

void SeaBattleControl::clientConnected()
{
    mode = waitFirstMove;
    connect(socket, SIGNAL(readyRead()), this, SLOT(socketReadyRead()));
    window->clientConnected();
}

void SeaBattleControl::errorOccurred(QAbstractSocket::SocketError socketError)
{
    emit networkError(socket->errorString());
}

void SeaBattleControl::socketReadyRead()
{
    if (waitFirstMove == mode)
    {
        mode = 0;
        receiveFirstMove();
    }
    else if (waitShot == mode)
    {
        mode = 0;
        receiveShot();
    }
    else if (waitShotResult == mode)
    {
        mode = 0;
        receiveShotResult();
    }
    else if (waitFieldData == mode)
    {
        mode = 0;
        receiveFieldData();
    }
}

void SeaBattleControl::sendFirstMove()
{
    switch (firstMove)
    {
    case firstMoveServer:
        move = true;
        break;
    case firstMoveClient:
        move = false;
        break;
    default:
        // Взять младший бит псевдослучайного числа
        move = (QRandomGenerator::global()->generate() & 1);
        break;
    }

    char x = move ? 0 : 1;
    socket->write(&x, 1);
    socket->flush();
}

void SeaBattleControl::receiveFirstMove()
{
    char x;
    socket->read(&x, 1);
    move = x;
    if (move)
        window->getEnemyFV()->enableAim();
    else
        mode = waitShot;
}

void SeaBattleControl::receiveShot()
{
    char x;
    socket->read(&x, 1);
    int i = x / 10;
    int j = x % 10;
    unsigned int result = window->getPlayerFV()->enemyShot(i, j);
    sendShotResult(result);
    if (Field::miss == result)
    {
        move = true;
        window->getEnemyFV()->enableAim();
    }
    else
    {
        if (Field::death == result && window->getPlayerFV()->gameOver())
            mode = waitFieldData;
        else
            mode = waitShot;
    }
}

void SeaBattleControl::sendShotResult(unsigned int r)
{
    char x = r;
    socket->write(&x, 1);
    socket->flush();
}

void SeaBattleControl::receiveShotResult()
{
    char x;
    socket->read(&x, 1);
    window->getEnemyFV()->playerShotResult(shot_i, shot_j, x);
    if (Field::miss == x)
    {
        move = false;
        mode = waitShot;
    }
    else
    {
        if (Field::death == x && window->getEnemyFV()->gameOver())
            sendFieldData();
        else
            window->getEnemyFV()->enableAim();
    }
}

void SeaBattleControl::sendFieldData()
{
    Field* f = window->getPlayerFV()->getField();
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            char x = f->get(i, j);
            socket->write(&x, 1);
        }
    }
    socket->flush();
    emit gameOver(true);
}

void SeaBattleControl::receiveFieldData()
{
    if (socket->bytesAvailable() < 100)
        return;
    Field* f = window->getEnemyFV()->getField();
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            char x;
            socket->read(&x, 1);
            f->set(i, j, x);
        }
    }
    window->getEnemyFV()->repaint();
    emit gameOver(false);
}

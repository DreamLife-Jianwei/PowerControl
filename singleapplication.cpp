#include "singleapplication.h"
#include <QWidget>
#include <QtNetwork/QLocalSocket>
#include <QtNetwork/QLocalServer>
#include <QFileInfo>
#include <QLibrary>
SingleApplication::SingleApplication(int &argc, char **argv) :QApplication(argc,argv),bRunning(false),localServer(nullptr),mainWindow(nullptr)
{
    serverName = QFileInfo(QCoreApplication::applicationFilePath()).fileName();
    initLocalConnection();
}

bool SingleApplication::isRuning()
{
    return bRunning;
}

void SingleApplication::newLocalConnectioin()
{
    QLocalSocket *socket = localServer->nextPendingConnection();
    if(!socket)
        return;
    socket->waitForReadyRead(1000);
    QTextStream stream(socket);
    delete socket;
    if(mainWindow != NULL)
    {
        mainWindow->raise();
        mainWindow->activateWindow();
        mainWindow->setWindowState((mainWindow->windowState() & ~Qt::WindowMinimized) | Qt::WindowActive);
        mainWindow->show();
    }
}

void SingleApplication::initLocalConnection()
{
    bRunning = false;
    QLocalSocket socket;
    socket.connectToServer(serverName);
    if(socket.waitForConnected(500))
    {
        bRunning = true;
        QTextStream stream(&socket);
        QStringList args = QCoreApplication::arguments();

        if(args.count() > 1)
            stream << args.last();
        else
            stream << QString();
        stream.flush();
        socket.waitForBytesWritten();
        return;

    }
    newLocalServer();

}

void SingleApplication::newLocalServer()
{
    localServer = new QLocalServer(this);
    connect(localServer,SIGNAL(newConnection()),this,SLOT(newLocalConnectioin()));
    if(!localServer->listen(serverName))
    {
        if(localServer->serverError() == QAbstractSocket::AddressInUseError)
        {
            QLocalServer::removeServer(serverName);
            localServer->listen(serverName);
        }
    }

}

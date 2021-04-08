#ifndef SINGLEAPPLICATION_H
#define SINGLEAPPLICATION_H

#include <QApplication>
#include <QObject>

class QWidget;
class QLocalServer;

class SingleApplication : public QApplication
{
    Q_OBJECT
public:
    SingleApplication(int &argc,char **argv);
    bool isRuning();
    QWidget *mainWindow;

private slots:
    void newLocalConnectioin();

private:
    void initLocalConnection();
    void newLocalServer();
    bool bRunning;
    QLocalServer *localServer;
    QString serverName;


};

#endif // SINGLEAPPLICATION_H

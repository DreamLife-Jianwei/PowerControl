#include "mainwindow.h"
#include <QApplication>
#include <singleapplication.h>
void autoStart()
{
    QString appName = QApplication::applicationName();
    QString appPath = QApplication::applicationFilePath();
    appPath = appPath.replace("/","\\");
    QSettings *reg=new QSettings("HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run",QSettings::NativeFormat);
    QString val = reg->value(appName).toString();
    if(val != appPath)
        reg->setValue(appName,appPath);
    reg->deleteLater();
}
//int main(int argc, char *argv[])
//{
//    QApplication a(argc, argv);
//    autoStart();
//    MainWindow w;
////    w.show();
//    return a.exec();
//}


int main(int argc, char *argv[])
{
    SingleApplication a(argc, argv);
    if(!a.isRuning())
    {
        autoStart();
        MainWindow w;
        a.mainWindow = &w;
        //    w.show();
        return a.exec();
    }
    return 0;
}

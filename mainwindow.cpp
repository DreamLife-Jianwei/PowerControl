#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("网络关机助手");
    this->setWindowFlag(Qt::FramelessWindowHint);
    ui->label_mainBack->setStyleSheet("QLabel{border-image:url(:/images/images/mainBack.png);}");
    ui->label_logo->setStyleSheet("QLabel{border-image:url(:/images/images/logo_W.png);}");
    ui->pushButton_poweroff->setToolTip("关闭所有在线计算机");
    ui->pushButton_close->setToolTip("最小化到任务栏");
    ui->pushButton_poweroff->setStyleSheet("QPushButton{border-image:url(:/images/images/pw_n.png);}QPushButton:hover{border-image:url(:/images/images/pw_h.png);}QPushButton:pressed{border-image:url(:/images/images/pw_p.png);}");
    ui->label_title->setText("网络关机助手");
    readConfig();
    initUDPSocket();
    initMySystemTrayIcon();
    initGetTimeTimer();
    getSystemInfor();

}

MainWindow::~MainWindow()
{
    if(m_getTimeTimer)
    {
        m_getTimeTimer->stop();
        delete m_getTimeTimer;
        m_getTimeTimer = nullptr;
    }
    delete ui;
}

void MainWindow::powerOffBat()
{
    QString strinfo;
    QProcess p(NULL);
    QString curPath = QCoreApplication::applicationDirPath();
    QString toolPath;
    toolPath = "/BAT";
    curPath.append(toolPath);
    p.setWorkingDirectory(curPath);
    toolPath = "/poweroff.bat";
    curPath.append(toolPath);
    p.start(curPath);
    if(p.waitForFinished())
    {
        qDebug() << "成功";
    }
    else
    {
        QMessageBox::warning(this,"警告","执行关机脚本失败\r\n请检查程序根目录下BAT文件中是否存在‘poweroff.bat’");
    }
}

void MainWindow::sentConmad()
{
    m_Socket->writeDatagram(QString::number(1,16).toLatin1(),QHostAddress::Broadcast,8088);
}
void MainWindow::on_pushButton_poweroff_clicked()
{
    /*
     * 关机指令需要测试，UDP广播应该是自己也可以收到，所以这里应该不需要单独执行关机指令
     */
    sentConmad();
}

void MainWindow::getUDPDate()
{
    while (m_Socket->hasPendingDatagrams())
    {
        QByteArray datagram;
        datagram.resize(m_Socket->pendingDatagramSize());
        m_Socket->readDatagram(datagram.data(),datagram.size());
        if(datagram.toInt() == 0x01)
        {

            if(this->isHidden())
            {
                this->show();
            }
            powerOffBat();
        }
    }
}

void MainWindow::on_pushButton_close_clicked()
{
    //    this->close();          //最后注释
    this->hide();
}
void MainWindow::initMySystemTrayIcon()
{
    /*
     * 设置系统托盘内容
     */
    m_trayIcon = new QSystemTrayIcon(this);
    m_trayIcon->setIcon(QIcon(":/images/images/logo.ico"));
    m_trayIcon->setToolTip("关机助手");
    m_trayIcon->show();
    connect(m_trayIcon,&QSystemTrayIcon::activated,this,[=](QSystemTrayIcon::ActivationReason temp){
        switch (temp) {
        case QSystemTrayIcon::Trigger:
        {
            //单击图标时间
            break;
        }
        case QSystemTrayIcon::DoubleClick:
        {
            if(this->isHidden())
            {
                this->showNormal();
            }
            else
            {
                this->hide();
            }
            break;
        }
        }
    });
    initMySystemTrayIconAction();
    initMySystemTrayIconMenu();
    //    m_trayIcon->showMessage("Tip","PowerControl is running",QSystemTrayIcon::MessageIcon::Information,3);
}

void MainWindow::initMySystemTrayIconAction()
{
    m_showWindowAction = new QAction(QIcon(":/images/images/logo.ico"),"显示界面",this);
    connect(m_showWindowAction,&QAction::triggered,this,[=](){this->show();});
    m_exitAppAction = new QAction(QIcon(":/images/images/exit.ico"),"退出程序",this);
    connect(m_exitAppAction,&QAction::triggered,this,[=](){this->close();});
    m_powerOffAppAction = new QAction(QIcon(":/images/images/logo.ico"),"一键关机",this);
    connect(m_powerOffAppAction,&QAction::triggered,this,&MainWindow::on_pushButton_poweroff_clicked);
}

void MainWindow::initMySystemTrayIconMenu()
{
    m_trayIconMenu = new QMenu(this);
    m_trayIconMenu->addAction(m_powerOffAppAction);
    m_trayIconMenu->addSeparator();
    m_trayIconMenu->addAction(m_showWindowAction);
    m_trayIconMenu->addAction(m_exitAppAction);
    m_trayIcon->setContextMenu(m_trayIconMenu);
}

void MainWindow::initUDPSocket()
{
    m_Socket = new QUdpSocket();
    m_Socket->bind(8088,QUdpSocket::ShareAddress);
    connect(m_Socket,&QUdpSocket::readyRead,this,&MainWindow::getUDPDate);
}
static QPoint last(0,0);        //保存坐标
const int TITLE_HEIGHT = 50;    //这里也可以使用宏定义，保存标题高度，也就是鼠标点击区域的高度
void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if(event->position().y()<TITLE_HEIGHT)
    {
        last = event->globalPosition().toPoint();
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if(event->position().y()<TITLE_HEIGHT)
    {
        int dx = event->globalPosition().x() - last.x();
        int dy = event->globalPosition().y() - last.y();
        last = event->globalPosition().toPoint();
        this->move(this->x()+dx,this->y()+dy);
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->position().y()<TITLE_HEIGHT)
    {
        int dx = event->globalPosition().x() - last.x();
        int dy = event->globalPosition().y() - last.y();
        this->move(this->x()+dx,this->y()+dy);
    }
}

QString MainWindow::getTime(QString format)
{
    QTime mTime = QTime::currentTime();
    if(ui->checkBox_isTimerOff->isChecked())
        if(mTime.hour() == mTimeOff.hour() && mTime.minute() == mTimeOff.minute())
        {
            ui->checkBox_isTimerOff->setChecked(false);
            QSettings *config = new QSettings("CONFIG.ini",QSettings::IniFormat);
            config->setValue("TimerOff/HH",mTime.hour());
            config->setValue("TimerOff/MM",mTime.minute());
            sentConmad();
        }

    return mTime.toString(format);
}

void MainWindow::initGetTimeTimer()
{
    if(!m_getTimeTimer)
    {
        m_getTimeTimer = new QTimer(this);
    }
    connect(m_getTimeTimer,&QTimer::timeout,this,[=](){
        ui->pushButton_close->setText(getTime("hh:mm"));
    });
    m_getTimeTimer->start(500);
}

void MainWindow::getSystemInfor()
{
    m_systemName = QHostInfo::localHostName();
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    for (int i = 0; i < ipAddressesList.size(); ++i) {
        if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
                ipAddressesList.at(i).toIPv4Address()) {
            m_systemIp = ipAddressesList.at(i).toString();
            break;
        }
    }
    if (m_systemIp.isEmpty())
        m_systemIp = QHostAddress(QHostAddress::LocalHost).toString();
    ui->label_sysInfor->setText(QString("设备名称：%1\r\nI P 地址：%2").arg(m_systemName).arg(m_systemIp));
}

void MainWindow::readConfig()
{
    QString curPath = QCoreApplication::applicationDirPath();
    curPath.append("/CONFIG.ini");
    QSettings *config = new QSettings(curPath,QSettings::IniFormat);
    ui->timeEdit_off->setTime(QTime(config->value("TimerOff/HH").toInt(),config->value("TimerOff/MM").toInt(),0,0));
    if(config->value("TimerOff/FLAG").toInt())
    {
        ui->checkBox_isTimerOff->setChecked(true);
    }

}
void MainWindow::on_timeEdit_off_userTimeChanged(const QTime &time)
{
    mTimeOff = time;
}

void MainWindow::on_checkBox_isTimerOff_clicked(bool checked)
{
    QString curPath = QCoreApplication::applicationDirPath();
    curPath.append("/CONFIG.ini");
    QSettings *config = new QSettings(curPath,QSettings::IniFormat);
    if(checked)
    {
        config->setValue("TimerOff/FLAG",1);
        config->setValue("TimerOff/HH",ui->timeEdit_off->time().hour());
        config->setValue("TimerOff/MM",ui->timeEdit_off->time().minute());
    }
    else
        config->setValue("TimerOff/FLAG",0);

}

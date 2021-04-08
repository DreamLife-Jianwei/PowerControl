#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QUdpSocket>
#include <QHostAddress>
#include <QProcess>
#include <QDir>
#include <QMouseEvent>
#include <QSystemTrayIcon>  //操作系统托盘操作类
#include <QMenu>
#include <QTabBar>
#include <QTabWidget>
#include <QTime>
#include <QTimer>
#include <QMessageBox>
#include <QHostInfo>
#include <QHostAddress>
#include <QString>
#include <QSettings>
#include <QNetworkInterface>
#include <QTextCodec>
#include <QtDebug>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    /**
     * @brief powerOffBat
     * bat 脚本关机
     */
    void powerOffBat();
    /**
     * @brief sentConmad
     * 发送指令
     */
    void sentConmad();
    /**
     * @brief initMySystemTrayIcon
     * 创建系统托盘
     */
    void initMySystemTrayIcon();
    /**
     * @brief initMySystemTrayIconAction
     * 创建系统托盘按钮
     */
    void initMySystemTrayIconAction();
    /**
     * @brief initMySystemTrayIconMenu
     * 创建系统托盘菜单
     */
    void initMySystemTrayIconMenu();
    /**
     * @brief initUDPSocket
     * 初始化UDP Socket
     */
    void initUDPSocket();
    /**
     * @brief mousePressEvent
     * @param event
     * 鼠标按下
     */
    void mousePressEvent(QMouseEvent* event);
    /**
     * @brief mouseMoveEvent
     * @param event
     * 鼠标移动
     */
    void mouseMoveEvent(QMouseEvent* event);
    /**
     * @brief mouseReleaseEvent
     * @param event
     * 鼠标释放
     */
    void mouseReleaseEvent(QMouseEvent* event);
    /**
     * @brief getTime
     * @param format    显示时间格式
     * @return 时间
     * 获取时间函数
     */
    QString getTime(QString format);
    /**
     * @brief initGetTimeTimer
     * 初始化获取时间定时器
     */
    void initGetTimeTimer();
    /**
     * @brief getSystemInfor
     * 获取计算机系统信息
     */
    void getSystemInfor();
    /**
     * @brief readConfig
     * 读取配置文件
     */
    void readConfig();


private slots:
    void on_pushButton_poweroff_clicked();
    /**
     * @brief getUDPDate
     * 解析网路数据
     */
    void getUDPDate();
    /**
     * @brief on_pushButton_close_clicked
     * 关闭函数
     */
    void on_pushButton_close_clicked();

    void on_timeEdit_off_userTimeChanged(const QTime &time);


    void on_checkBox_isTimerOff_clicked(bool checked);

private:
    Ui::MainWindow *ui;
    QSystemTrayIcon *m_trayIcon = nullptr;   //系统托盘类
    QMenu *m_trayIconMenu = nullptr;         //系统托盘菜单
    QAction *m_showWindowAction = nullptr;  //显示窗口
    QAction *m_exitAppAction = nullptr;     //退出程序
    QAction *m_powerOffAppAction = nullptr; //关闭电脑
    QUdpSocket *m_Socket = nullptr;         //UDP Socket
    QTimer *m_getTimeTimer = nullptr;       //时间定时器
    QTime mTimeOff;
    QString m_systemName;           //计算机名称
    QString m_systemIp;             //计算机IP地址
};
#endif // MAINWINDOW_H

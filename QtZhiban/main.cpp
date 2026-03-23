#include <QDesktopWidget>
#include <QFileInfo>
#include <QTextCodec>

#include "QtZhiban.h"
#include <QtWidgets/QApplication>
#include <QSharedMemory>
#include <QMessageBox>
#include <QString>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QFormLayout>
#include "Tip/MessageDialog.h"
#include "Tip/MessagePanel.h"
#include "Tip/TipPanel.h"
#include "Log/QtCLog.h"
#include "Common/global.h"
#include "Login.h"

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/QtZhiban/Resources/MyTitle/titleIcon.png"));

    // 创建“SingleApp”的共享内存块，单程序运行
    static QSharedMemory* singleApp = new QSharedMemory("QtZhiban");
    if (!singleApp->create(1))// 创建失败，说明已经有一个程序运行，
    {
        // 弹出提示框 注意：该提示应该在 qApp->quit() 之前，否则提示框将会一闪而过
        QMessageBox::critical(NULL, QString::fromLocal8Bit("警告"), QString::fromLocal8Bit("不能重复打开软件！"), QMessageBox::Ok, QMessageBox::Ok);
        qApp->quit();
        return -1;
    }

    // 消息提醒窗口初始化
    TipPanel::instance();
    MessageDialog::instance();
    MessagePanel::instance();
    CLog::LogConfig log_config;
    memoryDataService = MemoryDataService::instance();

    // 日志初始化
    log_config.isRecord2File = true;
    log_config.level = 0;
    CLog::init(log_config);

 //    // 读取席位列表
 //    QString configPath = qApp->applicationDirPath() + "/config/config.ini";
 //    QSettings settings(QApplication::applicationDirPath() + "/config/config.ini", QSettings::IniFormat);
 //    settings.setIniCodec(QTextCodec::codecForName("UTF-8"));
 //    QFileInfo configFile(configPath);
 //    if (!configFile.exists())
 //    {
 //        LOG_ERROR(QString::fromLocal8Bit("配置文件不存在！"));
 //        return -1;
 //    }
 //    QString aaa = settings.value("Seat/xiwei").toString();
 //    QString ss = QString::fromUtf8(settings.value("Seat/xiwei").toByteArray());
 //    QStringList sslist = ss.split(QString::fromLocal8Bit("，"));
 //    QString namelist = settings.value("Seat/xiwei", "").toString();
 //    
 //    QString zhiwu = QString::fromUtf8(settings.value("Seat/zhiwu").toByteArray());
 //    QStringList zhiwulist = zhiwu.split(QString::fromLocal8Bit("，"));
 //    memoryDataService->AddZhiwuList(zhiwulist);
 //
	// // 登录界面
 //    QDialog dialog;
 //    dialog.setWindowTitle("Login");
 //
	// QVBoxLayout layout;
 //    QLabel usernameLabel(QString::fromLocal8Bit("用户名:"));
 //    // QLineEdit usernameEdit;
 //    QComboBox combox;
 //    combox.addItems(sslist);
 //    QLabel passwordLabel(QString::fromLocal8Bit("密码:"));
 //    QLineEdit passwordEdit;
 //    passwordEdit.setEchoMode(QLineEdit::Password); // 密码模式
 //    QPushButton loginButton(QString::fromLocal8Bit("登录"));
 //
 //    layout.addWidget(&usernameLabel);
 //    // layout.addWidget(&usernameEdit);
 //    layout.addWidget(&combox);
 //    layout.addWidget(&passwordLabel);
 //    layout.addWidget(&passwordEdit);
 //    layout.addWidget(&loginButton);
 //    dialog.setLayout(&layout);
 //    dialog.setFixedSize(500, 500); // 设置窗口大小不可变
 //    QObject::connect(&loginButton, &QPushButton::clicked, [&]() {
 //        // QString user = usernameEdit.text();
 //        // qtzhiban.cpp 103行拿到这里，席位编号从1开始
 //        int seatindex = combox.currentIndex() + 1;
 //        memoryDataService->SetCurrentLoginID(seatindex);
 //        QString pass = passwordEdit.text();
 //        // 这里应该是验证用户名和密码的逻辑
 //        if (/*user == "admin" && */pass == "2020")
 //        {
 //            // QMessageBox::information(&dialog, "Success", "Login successful!");
 //            dialog.accept();
 //        }
 //        else
 //        {
 //            QMessageBox::critical(&dialog, QString::fromLocal8Bit("警告"), QStringLiteral("用户名或密码错误！"));
 //        }
 //    });
 //    if (dialog.exec())
 //    {

    // wordViewer* word_viewer = new wordViewer();
    // word_viewer->show();

    Login* login = new Login();
    if (login->exec())
    {
        QtZhiban* QtZhibanWidget = new QtZhiban();
    
        // 计算屏幕居中的位置
        QDesktopWidget desktop;
        int screenWidth = desktop.screenGeometry().width();
        int screenHeight = desktop.screenGeometry().height();
        int windowWidth = QtZhibanWidget->geometry().width();
        int windowHeight = QtZhibanWidget->geometry().height();
        
        int xPos = (screenWidth - windowWidth) / 2;
        int yPos = (screenHeight - windowHeight) / 2;
        
        // 移动窗口到屏幕居中的位置
        QtZhibanWidget->move(xPos, yPos);
    
        // QtZhibanWidget->setWindowIcon(QIcon(":/QtZhiban/Resources/MyTitle/titleIcon.png"));
        QtZhibanWidget->show();
    
    
        return a.exec();
    }
    a.exit(0);
    // return a.exec();
}

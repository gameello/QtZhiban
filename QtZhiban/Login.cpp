#include "Login.h"

#include <QFileInfo>
#include <QMessageBox>
#include <QSqlRecord>
#include <QTextCodec>
#include <QSet>
#include "Common/MemoryDataService.h"
#include "Database/ConnectionPool.h"
#include <QThread>
#include "Common/MemoryData.h"


Login::Login(QWidget *parent) : ModalDialog(parent), ui(new Ui::LoginClass())
{
	// ui->setupUi(this);
	SETUP_UI(ui)

	// memoryDataService = MemoryDataService::instance();
	// settingService = SettingService::instance();
	// database_service = DatabaseService::instance();

	SetTitleText(QString::fromLocal8Bit("登录"));
	Login::InitWidget();
	setAttribute(Qt::WA_DeleteOnClose);
    connect(ui->btn_signin, SIGNAL(clicked()), this, SLOT(on_btn_signin_clicked()));
}

Login::~Login()
{
	delete ui;
}

void Login::InitWidget()
{
    // // 读取席位列表
    // QString configPath = qApp->applicationDirPath() + "/config/config.ini";
    // QSettings settings(QApplication::applicationDirPath() + "/config/config.ini", QSettings::IniFormat);
    // settings.setIniCodec(QTextCodec::codecForName("UTF-8"));
    // QFileInfo configFile(configPath);
    // if (!configFile.exists())
    // {
    //     LOG_ERROR(QString::fromLocal8Bit("配置文件不存在！"));
    //     return;
    // }
    // QString aaa = settings.value("Seat/xiwei").toString();
    // QString ss = QString::fromUtf8(settings.value("Seat/xiwei").toByteArray());
    // QStringList sslist = ss.split(QString::fromLocal8Bit("，"));
    // QString namelist = settings.value("Seat/xiwei", "").toString();
    //
    // QString zhiwu = QString::fromUtf8(settings.value("Seat/zhiwu").toByteArray());
    // QStringList zhiwulist = zhiwu.split(QString::fromLocal8Bit("，"));
    // memoryDataService->AddZhiwuList(zhiwulist);
    // ui->comboBox->addItems(sslist);

    // 从数据库读取席位列表
    memoryDataService = MemoryDataService::instance();
    messageTipService = MessageTipService::instance();

    // 查询数据库
    QSqlDatabase Connection = ConnectionPool::openConnection();
    QSqlQuery query(Connection);
    if (!Connection.isOpen())
    {
        QString err = query.lastError().databaseText();
        LOG_ERROR(QString::fromLocal8Bit("数据库连接失败：%s"), err.toStdString());
        QMessageBox::critical(nullptr, QString::fromLocal8Bit("连接失败"), QString::fromLocal8Bit("无法连接到服务器,请检查网络连接。"));
        // messageTipService->showTip(QString::fromLocal8Bit("数据库连接失败！"), TT_FAILED);
        // messageTipService->showDialog("sss", DT_ERROR);
        // messageTipService->showMessage("sss", MT_ERROR);
        exit(0);
    }
    else
    {
        query.prepare(QString("SELECT CURRENT_DATE;"));
        bool success = query.exec();
        if (!success)
        {
            QString err = query.lastError().databaseText();
            LOG_ERROR(QString::fromLocal8Bit("数据库读取时间失败：%s"), err.toStdString());
            messageTipService->showTip(QStringLiteral("数据库读取时间失败！"), TipType::TT_FAILED);
        }
        if (success)
        {
            QDate currdate;
            while (query.next())
            {
                QSqlRecord dataLine = query.record();
                currdate = dataLine.value(0).toDate();
                // messageTipService->showTip(QStringLiteral("更新成功！"), TipType::TT_SUCESS);
            }
            QString date = currdate.toString("yyyy-MM-dd");
            memoryDataService->SetcurrDate(currdate);
        }

        query.clear();
        query.prepare(QString("SELECT id, xiwei, password, level FROM public.\"login\" order by level,id;"));
        success = query.exec();
        if (!success)
        {
            QString err = query.lastError().databaseText();
            LOG_ERROR(QString::fromLocal8Bit("数据库读取席位失败：%s"), err.toStdString());
            messageTipService->showTip(QStringLiteral("数据库读取席位失败！"), TipType::TT_FAILED);
        }
        if (success)
        {
            QList<loginStruct> xiweistructs;
            QMap<int, loginStruct> xiweistructs_map;
            while (query.next())
            {
                QSqlRecord dataLine = query.record();
                loginStruct xiwei;
                xiwei.id = dataLine.value("id").toInt();
                xiwei.name = dataLine.value("xiwei").toString();
                xiwei.password = dataLine.value("password").toString();
                xiwei.level = dataLine.value("level").toInt();
                xiweistructs.append(xiwei);
                xiweistructs_map.insert(xiwei.id, xiwei);
                // messageTipService->showTip(QStringLiteral("更新成功！"), TipType::TT_SUCESS);
            }
            memoryDataService->AddSeatNameList(xiweistructs);
            memoryDataService->AddSeatNameMap(xiweistructs_map);
        	for (auto xiweistruct :xiweistructs)
            {
                ui->comboBox->addItem(xiweistruct.name);
            }
        }

        query.clear();
		query.prepare(QString("SELECT id, type, info, level FROM public.\"config\" order by id;"));
		success = query.exec();
		if (!success)
		{
		    QString err = query.lastError().databaseText();
		    LOG_ERROR(QString::fromLocal8Bit("数据库读取配置失败：%s"), err.toStdString());
		    messageTipService->showTip(QStringLiteral("数据库读取配置失败！"), TipType::TT_FAILED);
		}
		if (success)
		{
            QMap<int, sqlConfig> sql_configs;
		    while (query.next())
		    {
		        QSqlRecord dataLine = query.record();
                sqlConfig config;
                config.id = dataLine.value("id").toInt();
                config.type = dataLine.value("type").toString();
                config.info = dataLine.value("info").toString();
                config.level = dataLine.value("level").toInt();
                sql_configs.insert(config.id, config);
		    }
		    //memoryDataService->SetSqlconfig(sql_configs);
		}


        // query.clear();
        // query.prepare(QString("SELECT id, zhiwu FROM public.\"zhiwu\" order by id;"));
        // success = query.exec();
        // if (!success)
        // {
        //     QString err = query.lastError().databaseText();
        //     LOG_ERROR(QString::fromLocal8Bit("数据库读取职务失败：%s"), err.toStdString());
        //     messageTipService->showTip(QStringLiteral("数据库读取职务失败！"), TipType::TT_FAILED);
        // }
        // if (success)
        // {
        //     QStringList zhiwulist;
        //     while (query.next())
        //     {
        //         QSqlRecord dataLine = query.record();
        //         zhiwulist.append(dataLine.value("zhiwu").toString());
        //         // messageTipService->showTip(QStringLiteral("更新成功！"), TipType::TT_SUCESS);
        //     }
        //     memoryDataService->AddZhiwuList(zhiwulist);
        // }
    
        query.clear();
        query.prepare(QString("SELECT id, name, zhiwu, xiwei FROM public.\"renyuan\" order by id;"));
        success = query.exec();
        if (!success)
        {
            QString err = query.lastError().databaseText();
            LOG_ERROR(QString::fromLocal8Bit("数据库读取人员失败：%s"), err.toStdString());
            messageTipService->showTip(QStringLiteral("数据库读取人员失败！"), TipType::TT_FAILED);
        }
        if (success)
        {
            QMap<int, QList<contactUserStruct>> map;
            QMap<int, QList<QString>> renyuanmap;
            QMap<int, QList<QString>> zhiwumap;
            while (query.next())
            {
                QSqlRecord dataLine = query.record();
                contactUserStruct renyuanstruc;
                renyuanstruc.id = dataLine.value("id").toInt();
                renyuanstruc.name = dataLine.value("name").toString();
                renyuanstruc.zhiwu = dataLine.value("zhiwu").toString();
                renyuanstruc.xiwei = dataLine.value("xiwei").toInt();
                QList<contactUserStruct> renyuanstructs = map.value(renyuanstruc.xiwei);
                renyuanstructs.append(renyuanstruc);
                map.insert(renyuanstruc.xiwei, renyuanstructs);

                QList<QString> renyuanlist = renyuanmap.value(renyuanstruc.xiwei);
                renyuanlist.append(renyuanstruc.name);
	            {
		            QSet<QString> seen;
                	QList<QString> uniqueList;
                	for (auto num : renyuanlist) {
                		if (!seen.contains(num)) {
                			seen.insert(num);
                			uniqueList.append(num);
                		}
                	}
                	renyuanlist = uniqueList;
	            }
                // QSet<QString> set = renyuanlist.toSet();
                // renyuanlist = set.toList();
                renyuanmap.insert(renyuanstruc.xiwei, renyuanlist);

                QList<QString> zhiwulist = zhiwumap.value(renyuanstruc.xiwei);
                zhiwulist.append(renyuanstruc.zhiwu);
                {
                    QSet<QString> seen2;
                    QList<QString> uniqueList2;
                    for (auto num : zhiwulist) {
                        if (!seen2.contains(num)) {
                            seen2.insert(num);
                            uniqueList2.append(num);
                        }
                    }
                    zhiwulist = uniqueList2;
                }
                // set = zhiwulist.toSet();
                // zhiwulist = set.toList();
                zhiwumap.insert(renyuanstruc.xiwei, zhiwulist);
            }
            memoryDataService->SetRenyuanStructs(map);
            memoryDataService->SetRenyuanMap(renyuanmap);
            memoryDataService->SetZhiwuMap(zhiwumap);
        }

        // query.clear();
        // query.prepare(QString("SELECT id, type, info FROM public.\"config\" order by id,type;"));
        // success = query.exec();
        // if (!success)
        // {
        //     QString err = query.lastError().databaseText();
        //     LOG_ERROR(QString::fromLocal8Bit("数据库读取tab名称失败：%s"), err.toStdString());
        //     messageTipService->showTip(QStringLiteral("数据库读取tab名称失败！"), TipType::TT_FAILED);
        // }
        // if (success)
        // {
        //     QList<loginStruct> sslist;
        //     while (query.next())
        //     {
        //         QSqlRecord dataLine = query.record();
        //         loginStruct xiwei;
        //         xiwei.id = dataLine.value("id").toInt();
        //         xiwei.name = dataLine.value("xiwei").toString();
        //         xiwei.password = dataLine.value("password").toString();
        //         xiwei.level = dataLine.value("level").toInt();
        //         sslist.append(xiwei);
        //         // messageTipService->showTip(QStringLiteral("更新成功！"), TipType::TT_SUCESS);
        //     }
        //     memoryDataService->AddSeatNameList(sslist);
        //     int a = 1;
        //     for (auto name : sslist)
        //     {
        //         if (a > 6)
        //         {
        //             ssslist.append(name.name);
        //         }
        //         a++;
        //     }
        //
        //     ui->comboBox->addItems(ssslist);
        // }
    
        // 关闭数据库连接
        ConnectionPool::closeConnection(Connection);
    }
    
    ui->lineEdit_password->setEchoMode(QLineEdit::Password); // 密码模式
}

// void Login::SlotCloseBtnClicked()
// {
// 	ModalessDialog::SlotCloseBtnClicked();
// }

void Login::on_btn_signin_clicked()
{
    // qtzhiban.cpp 103行拿到这里，席位编号从1开始
    // 第二次修改，前6个席位登录界面设置为一个席位，汇报界面隐藏前6个席位
    // 第三次修改，取消上级机关下的6个席位
    QList<loginStruct> xiweistructs = memoryDataService->GetSeatList();
    int seatID = 0;
    QString password;
    for (auto xiweistruct : xiweistructs)
    {
	    if (ui->comboBox->currentText() == xiweistruct.name)
	    {
            seatID = xiweistruct.id;
            password = xiweistruct.password;
	    }
    }
	memoryDataService->SetCurrSeatName(seatID);

    if (password != ui->lineEdit_password->text())
    {
        // QMessageBox::information(this, "登录认证", "登录失败,账户或者密码错误");
        QMessageBox::critical(this, QString::fromLocal8Bit("警告"), QStringLiteral("用户名或密码错误！"));
    }
    else
    {
        accept();
    }
}
#include "QtZhiban.h"
#include <QFileInfo>
#include <QTextCodec>
#include <QDesktopWidget>
#include <QSqlRecord>
#include <QSqlQuery>
#include "common/Utils.h"
#include <QTableWidget>
#include <QFileDialog>
#include <QDesktopServices>
#include <QMessageBox>
#include <QAxObject>
#include "Todo/mainwindow.h"	
#include "Database/ConnectionPool.h"
#include <QSystemTrayIcon>
#include <QMenu>

#include "Common/IdGenerateHelper.h"
#include "PlanParse/treemodel.h"

QtZhiban::QtZhiban(QWidget *parent) : QWidget(parent)
{
    ui = new Ui::QtZhibanClass();
    // SETUP_UI(ui);
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint);

    memoryDataService = MemoryDataService::instance();
    settingService = SettingService::instance();
    idHelper = IdGenerateHelper::instance();

    Loadini();
    InitWidget();
    SetStatusWidget();
    // ui.tabWidget_2->setTabPosition(QTabWidget::West); // 设置标签位于左侧

    // // 右下角托盘
    // trayIcon = new QSystemTrayIcon(this);
    // trayIcon->setIcon(QIcon(":/QtZhiban/Resources/MyTitle/titleIcon.png"));
    // trayIcon->setToolTip(QString::fromLocal8Bit("值班辅助系统"));
    // trayIcon->show();
    // connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(trayActivated(QSystemTrayIcon::ActivationReason)));
    //
    // // QString titlec = QString::fromLocal8Bit("Hello");
    // // QString textc = QString::fromLocal8Bit("Hello QT?");
    // // trayIcon->showMessage(titlec, textc, QSystemTrayIcon::Information, 5000);
    // // setWindowTitle(tr("Systray"));
    //
    // //可以定义在.h文件中
    // QMenu* trayIconMenu;
    // // QAction* minimizeAction;
    // // QAction* maximizeAction;
    // QAction* restoreAction;
    // QAction* quitAction;
    // //下面的构造函数中进行执行
    // // minimizeAction = new QAction(QString::fromLocal8Bit( "最小化"), this);
    // // connect(minimizeAction, &QAction::triggered, this, &QtZhiban::hide);
    // // maximizeAction = new QAction(QString::fromLocal8Bit("最大化"), this);
    // // connect(maximizeAction, &QAction::triggered, this, &QtZhiban::showMaximized);
    // restoreAction = new QAction(QString::fromLocal8Bit("还原"), this);
    // connect(restoreAction, &QAction::triggered, this, &QtZhiban::showNormal);
    // quitAction = new QAction(QString::fromLocal8Bit("退出"), this);
    // //connect(quitAction,&QAction::triggered,this,&Widget::close);    //关联窗口的close不太合适
    // connect(quitAction, &QAction::triggered, this, &QCoreApplication::quit);   //应用程序的退出
    //
    // //创建菜单，添加菜单项
    // trayIconMenu = new QMenu(this);
    // // trayIconMenu->addAction(minimizeAction);
    // // trayIconMenu->addAction(maximizeAction);
    // trayIconMenu->addAction(restoreAction);
    // trayIconMenu->addSeparator();   //分割线
    // trayIconMenu->addAction(quitAction);
    // //给系统托盘添加右键菜单
    // trayIcon->setContextMenu(trayIconMenu);
}

QtZhiban::~QtZhiban()
= default;

void QtZhiban::InitWidget()
{
// #include <QCoreApplication>
// #include <QTimer>
// #include <QThread>
// #include <QDebug>
//
//     class Worker : public QObject {
//         Q_OBJECT
//     public slots:
//         void doWork() {
//             qDebug() << "Work done in thread" << QThread::currentThreadId();
//             emit finished(); // 通知主线程工作完成
//         }
//     signals:
//         void finished(); // 信号，通知主线程工作完成
//     };
//
//     int main(int argc, char* argv[]) {
//         QCoreApplication app(argc, argv);
//         QThread workerThread;
//
//         Worker* worker = new Worker;
//         worker->moveToThread(&workerThread); // 将worker对象移动到新线程
//         QObject::connect(&workerThread, &QThread::started, worker, &Worker::doWork); // 启动线程时执行工作
//         QObject::connect(worker, &Worker::finished, &workerThread, &QThread::quit); // 工作完成后退出线程
//         QObject::connect(worker, &Worker::finished, worker, &Worker::deleteLater); // 清理资源
//         QObject::connect(&workerThread, &QThread::finished, worker, &QObject::deleteLater); // 清理资源
//         workerThread.start(); // 启动线程
//         return app.exec(); // 进入Qt的事件循环，等待线程完成
//     }


    //如果应用对时间精度要求很高（如实时系统、动画、游戏等），应该选择 Qt::PreciseTimer。
	//如果应用对时间精度要求一般（如定时更新界面、定时保存等），可以选择 Qt::CoarseTimer。
	//如果应用对时间精度要求很低（如定期检查状态、定时任务等），可以选择 Qt::VeryCoarseTimer。
    timer_RefreshcurrTime.setTimerType(Qt::VeryCoarseTimer);
    connect(&timer_RefreshcurrTime, SIGNAL(timeout()), this, SLOT(RefreshcurrTime()));
    timer_RefreshcurrTime.start(7200000);
    // timer_RefreshcurrTime.start(30000);

    // this->setStyleSheet("QWidget{border:1px solid #0000FF;}");
    m_titleBar = new MyTitleBar(this);
    m_titleBar->move(0, 0);
    // m_titleBar->setTitleRoll();

    connect(m_titleBar, SIGNAL(signalButtonMinClicked()), this, SLOT(onButtonMinClicked()));
    connect(m_titleBar, SIGNAL(signalButtonRestoreClicked()), this, SLOT(onButtonRestoreClicked()));
    connect(m_titleBar, SIGNAL(signalButtonMaxClicked()), this, SLOT(onButtonMaxClicked()));
    connect(m_titleBar, SIGNAL(signalButtonCloseClicked()), this, SLOT(onButtonCloseClicked()));

    m_titleBar->setTitleIcon(":/QtZhiban/Resources/MyTitle/titleIcon.png");
    QString xiweiName = memoryDataService->GetLoginMap().value(xiweiID).name;
    m_titleBar->setTitleContent(QStringLiteral("一号台辅助应用   登录席位：") + xiweiName);
    m_titleBar->setButtonType(MIN_MAX_BUTTON);
    m_titleBar->setTitleWidth(this->width());
    // m_titleBar->setTitleRoll();

    // SetTitleText(QString::fromLocal8Bit("值班排班汇报软件"));

    connect(ui->tabWidget, SIGNAL(tabBarClicked(int)), this, SLOT(tabwidgetchanged(int)));

    // 读取sqlconfig文件，添加tab页
    QMap<int, sqlConfig> sql_configs = memoryDataService->Getsqlconfig();
    int tabindex = 1;
    for (auto config : sql_configs)
    {
	    if (config.type == "tab")
	    {
            QWidget* tab = new QWidget;
            ui->tabWidget->insertTab(tabindex++, tab, config.info);
            // ui->tabWidget->addTab(tab, config.info);
	    }
    }


    // 通讯录管理界面
    QStringList headers;
    headers << QStringLiteral("单位名称") << QStringLiteral("详细描述");

    QFile file("D:/4_SZ/QtZhiban/default.txt");
    file.open(QIODevice::ReadOnly);
    TreeModel* model = new TreeModel(headers, file.readAll());
    file.close();

    ui->view->setModel(model);
    for (int column = 0; column < model->columnCount(); ++column)
        ui->view->resizeColumnToContents(column);

    connect(ui->view->selectionModel(), &QItemSelectionModel::selectionChanged,this, &QtZhiban::updateActions);
    connect(ui->insertRowAction, &QPushButton::clicked, this, &QtZhiban::insertRow);
    // connect(ui->insertColumnAction, &QPushButton::clicked, this, &QtZhiban::insertColumn);
    connect(ui->removeRowAction, &QPushButton::clicked, this, &QtZhiban::removeRow);
    // connect(ui->removeColumnAction, &QPushButton::clicked, this, &QtZhiban::removeColumn);
    connect(ui->insertChildAction, &QPushButton::clicked, this, &QtZhiban::insertChild);
    updateActions();
}

void QtZhiban::Loadini()
{
	// // 从ini读取席位列表
 //    QString configPath = qApp->applicationDirPath() + "/config/config.ini";
 //    QSettings settings(QApplication::applicationDirPath() + "/config/config.ini", QSettings::IniFormat);
 //    settings.setIniCodec(QTextCodec::codecForName("UTF-8"));
 //    QFileInfo configFile(configPath);
 //    if (!configFile.exists()) 
 //    {
 //        LOG_ERROR(QString::fromLocal8Bit("配置文件不存在！"));
 //        return;
 //    }
 //    QString aaa = settings.value("Seat/xiwei").toString();
 //    QString ss = QString::fromUtf8(settings.value("Seat/xiwei").toByteArray());
 //    QStringList sslist = ss.split(QString::fromLocal8Bit("，"));
 //    QString namelist = settings.value("Seat/xiwei", "").toString();
	// // QString Seatnames = settingService->GetQStringProperty((char*)"Seat/xiwei", (char*)"");
	// // QStringList sa = Seatnames.split(QString::fromLocal8Bit("，"));
	// memoryDataService->SetLoginList(sslist);


    // 打开文档编辑界面，默认席位七
    currSeeXiweiID = memoryDataService->GetCurrentLoginID();

    // 读取当前席位,从1开始
    // xiweiID = settings.value("Seat/currSeat").toInt();
    // memoryDataService->SetCurrentLoginID(xiweiID);
    xiweiID = memoryDataService->GetCurrentLoginID();
}

void QtZhiban::SetStatusWidget()
{
    // 设置窗体透明度
    this->setWindowOpacity(0.95);

    //设置样式表
    Ui::SetStyleSheet(this, ":/qss/modaldialog.qss");

    m_pStatusWidget = new QWidget(this);
	// 设置状态栏
    int statusWidgetH = 19;
    m_pStatusWidget->setFixedHeight(statusWidgetH);
    m_pStatusWidget->setStyleSheet("background:#015293;border:none;");
    QHBoxLayout* statusLayout = new QHBoxLayout();
    statusLayout->setContentsMargins(0, 0, 0, 0);
    statusLayout->setSpacing(0);
    QWidget* statusLeft = new QWidget(m_pStatusWidget);
    QWidget* statusRight = new QWidget(m_pStatusWidget);
    int LeftWidgetW = 68;
    statusLeft->setFixedWidth(LeftWidgetW);
    statusLeft->setStyleSheet("background:url(:/images/bottom/right_img_bottom_left.png);border:none;");
    statusRight->setStyleSheet("border-image:url(:/images/bottom/right_img_bottom_right.png);border:none;");
    statusLayout->addWidget(statusLeft);
    statusLayout->addWidget(statusRight);
    m_pStatusWidget->setLayout(statusLayout);


	// //对话框整体布局
	// QVBoxLayout* m_pVLayout = new QVBoxLayout();
	// m_pVLayout->setContentsMargins(0, 0, 0, 0);
	// m_pVLayout->setSpacing(0);
	//
	// //添加标题栏到整体布局
	// m_pVLayout->addWidget(m_titleBar);
	// m_titleBar->move(0, 0);
	// m_pVLayout->addWidget(ui->tabWidget);
	// m_pVLayout->addWidget(m_pStatusWidget);
	//
	// //设置整体布局
	// setLayout(m_pVLayout);
    
    // this->layout()->addWidget(m_titleBar);
    // m_titleBar->move(0, 0);
    // this->layout()->addWidget(ui->tabWidget);
    this->layout()->addWidget(m_pStatusWidget);
}

void QtZhiban::onButtonMinClicked()
{
    showMinimized();
}

void QtZhiban::onButtonRestoreClicked()
{
    QPoint windowPos;
    QSize windowSize;
    m_titleBar->getRestoreInfo(windowPos, windowSize);
    this->setGeometry(QRect(windowPos, windowSize));
}

void QtZhiban::onButtonMaxClicked()
{
    m_titleBar->saveRestoreInfo(this->pos(), QSize(this->width(), this->height()));
    QRect desktopRect = QApplication::desktop()->availableGeometry();
    QRect FactRect = QRect(desktopRect.x() - 3, desktopRect.y() - 3, desktopRect.width() + 6, desktopRect.height() + 6);
    setGeometry(FactRect);
}

void QtZhiban::onButtonCloseClicked()
{
    ConnectionPool::release();
    this->close();
    qApp->exit();
    // hide();  // 右下角托盘时使用
}

void QtZhiban::btn_clicked()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    // button->setStyleSheet("background-color : red");
    button->setChecked(true);
    currSeeXiweiID = button->objectName().split("-")[1].toInt();
    int index = ui->tabWidget->currentIndex();
    sqlConfig config = memoryDataService->Getsqlconfig().value(index);
    if (config.level == 9999)
    {

    }
    else
    {

    }
}

void QtZhiban::tabwidgetchanged(int index)
{

}

//第一个参数是页面上的表格，第二个是导出文件的表头数据
void QtZhiban::Table2ExcelByHtml(QTableWidget* table, QString title)
{
    QString fileName = QFileDialog::getSaveFileName(table, QString::fromLocal8Bit("保存"), QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation), QString::fromLocal8Bit("Excel 文件(*.xlsx)"));
    if (fileName != "")
    {
        QAxObject* excel = new QAxObject;
        if (excel->setControl("Excel.Application")) //连接Excel控件
        {
            excel->dynamicCall("SetVisible (bool Visible)", "false");//不显示窗体
            excel->setProperty("DisplayAlerts", false);//不显示任何警告信息。如果为true那么在关闭是会出现类似“文件已修改，是否保存”的提示
            QAxObject* workbooks = excel->querySubObject("WorkBooks");//获取工作簿集合
            workbooks->dynamicCall("Add");//新建一个工作簿
            QAxObject* workbook = excel->querySubObject("ActiveWorkBook");//获取当前工作簿
            QAxObject* worksheet = workbook->querySubObject("Worksheets(int)", 1);

            int i, j;
            //QTablewidget 获取数据的列数
            int colcount = table->columnCount();
            //QTablewidget 获取数据的行数
            int rowscount = table->rowCount();

                //QTableView 获取列数
           //int colount=ui->tableview->model->columnCount();
                //QTableView 获取行数
           //int rowcount=ui->tableview->model->rowCount();

                QAxObject * cell, * col;
            //标题行
            cell = worksheet->querySubObject("Cells(int,int)", 1, 1);
            cell->dynamicCall("SetValue(const QString&)", title);
            cell->querySubObject("Font")->setProperty("Size", 18);
            //调整行高
            worksheet->querySubObject("Range(const QString&)", "1:1")->setProperty("RowHeight", 30);
            //合并标题行
            QString cellTitle;
            cellTitle.append("A1:");
            cellTitle.append(QChar(colcount - 1 + 'A'));
            cellTitle.append(QString::number(1));
            QAxObject* range = worksheet->querySubObject("Range(const QString&)", cellTitle);
            range->setProperty("WrapText", true);
            range->setProperty("MergeCells", true);
            range->setProperty("HorizontalAlignment", -4108);//xlCenter
            range->setProperty("VerticalAlignment", -4108);//xlCenter
            //列标题
            for (i = 0; i < colcount; i++)
            {
                QString columnName;
                columnName.append(QChar(i + 'A'));
                columnName.append(":");
                columnName.append(QChar(i + 'A'));
                col = worksheet->querySubObject("Columns(const QString&)", columnName);
                col->setProperty("ColumnWidth", table->columnWidth(i) / 6);
                cell = worksheet->querySubObject("Cells(int,int)", 2, i + 1);
                //QTableWidget 获取表格头部文字信息
                columnName = table->horizontalHeaderItem(i)->text();
                //QTableView 获取表格头部文字信息
                // columnName=ui->tableView_right->model()->headerData(i,Qt::Horizontal,Qt::DisplayRole).toString();
                cell->dynamicCall("SetValue(const QString&)", columnName);
                cell->querySubObject("Font")->setProperty("Bold", true);
                cell->querySubObject("Interior")->setProperty("Color", QColor(191, 191, 191));
                cell->setProperty("HorizontalAlignment", -4108);//xlCenter
                cell->setProperty("VerticalAlignment", -4108);//xlCenter
            }

            //数据区

            //QTableWidget 获取表格数据部分
            for (i = 0; i < rowscount; i++) 
            {
                for (j = 0; j < colcount; j++)
                {
                    // worksheet->querySubObject("Cells(int,int)", i + 3, j + 1)->dynamicCall("SetValue(const QString&)", table->item(i, j) ? table->item(i, j)->text() : "");

                    QAxObject* cell;
                    cell = worksheet->querySubObject("Cells(int,int)", i + 3, j + 1);
                    cell->dynamicCall("SetValue(const QString&)", table->item(i, j) ? table->item(i, j)->text() : "");
                    // cell->setProperty("NumberFormat", "@");           // "@" 表示文本格式
                    cell->setProperty("HorizontalAlignment", -4108);  // 水平居中
                    cell->setProperty("VerticalAlignment", -4108);    // 垂直居中
                    delete cell;
                }
            }


            //QTableView 获取表格数据部分
            //  for(i=0;i<rowcount;i++) //行数
            //     {
            //         for (j=0;j<colcount;j++)   //列数
            //         {
            //             QModelIndex index = ui->tableView_right->model()->index(i, j);
            //             QString strdata=ui->tableView_right->model()->data(index).toString();
            //             worksheet->querySubObject("Cells(int,int)", i+3, j+1)->dynamicCall("SetValue(const QString&)", strdata);
            //         }
            //     }


             //画框线
            QString lrange;
            lrange.append("A2:");
            lrange.append(colcount - 1 + 'A');
            lrange.append(QString::number(table->rowCount() + 2));
            range = worksheet->querySubObject("Range(const QString&)", lrange);
            range->querySubObject("Borders")->setProperty("LineStyle", QString::number(1));
            range->querySubObject("Borders")->setProperty("Color", QColor(0, 0, 0));
            //调整数据区行高
            QString rowsName;
            rowsName.append("2:");
            rowsName.append(QString::number(table->rowCount() + 2));
            range = worksheet->querySubObject("Range(const QString&)", rowsName);
            range->setProperty("RowHeight", 20);
            workbook->dynamicCall("SaveAs(const QString&)", QDir::toNativeSeparators(fileName));//保存至fileName
            workbook->dynamicCall("Close()");//关闭工作簿
            excel->dynamicCall("Quit()");//关闭excel
            delete excel;
            excel = NULL;
            if (QMessageBox::question(NULL, QString::fromLocal8Bit("完成"), QString::fromLocal8Bit("文件已经导出，是否现在打开？"), QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
            {
                QDesktopServices::openUrl(QUrl("file:///" + QDir::toNativeSeparators(fileName)));
            }
        }
        else
        {
            QMessageBox::warning(NULL, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("未能创建 Excel 对象，请安装 Microsoft Excel。"), QMessageBox::Apply);
        }
    }
}

void QtZhiban::widget_shake()
{
    pShakeAnimation = new QPropertyAnimation(this, "pos");		//窗口类初始化或构造函数中创建动画属性指针
    QPoint pos = this->pos();
    //如果动画尚未结束，就先结束动画然后继续
    if (pShakeAnimation->state() == QPropertyAnimation::Running)
    {
        pShakeAnimation->stop();
    }
    //也可以在动画尚未结束之时直接返回等待动画结束后方可第二次启动抖动动画

    //设置动画时长，这里可以用配置，一般来说设置600足够，可以通过配置属性来实现自定义时长
    pShakeAnimation->setDuration(600);

    //记录初始位置
    pShakeAnimation->setStartValue(pos);

    int offset = 0;                    //设置偏移量
    double dOffset = (double)1 / 30;   //设置抖动次数进度递增因子，300是指抖动的次数为三百次
    double dIndex = dOffset;           //初始抖动进度因子

    //代码中的300是指抖动三百次
    for (int i = 1; i < 300; i++) {
        offset = i % 2 == 0 ? -10 : 10;       //计算偏移量，偶数和奇数时不同
        dIndex += dOffset;            //进度递增

        /*注意这里的示例只是抖动次数为偶数时向左向上偏移10像素
            抖动次数为奇数时向右和向下偏移10像素往复循环。如果要实现其他的抖动规则情自行实现偏移规则*/

            //设置动画属性进度和偏移值
        pShakeAnimation->setKeyValueAt(dIndex, pos + QPoint((int)offset, (int)offset));
    }

    pShakeAnimation->setEndValue(pos);
    pShakeAnimation->start();

    ui->tabWidget->setCurrentIndex(8);
}

void QtZhiban::onOpacityWindow()
{
    QPropertyAnimation* pAnimation = new QPropertyAnimation(this, "windowOpacity");
    pAnimation->setDuration(1000);
    pAnimation->setKeyValueAt(0, 1);
    pAnimation->setKeyValueAt(0.5, 0);
    pAnimation->setKeyValueAt(1, 1);
    pAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}

void QtZhiban::onDropWindow()
{
    QPropertyAnimation* pAnimation = new QPropertyAnimation(this, "geometry");

    QDesktopWidget* pDesktopWidget = QApplication::desktop();
    int x = (pDesktopWidget->availableGeometry().width() - width()) / 2;
    int y = (pDesktopWidget->availableGeometry().height() - height()) / 2;

    pAnimation->setDuration(1000);
    pAnimation->setStartValue(QRect(x, 0, width(), height()));
    pAnimation->setEndValue(QRect(x, y, width(), height()));
    pAnimation->setEasingCurve(QEasingCurve::OutElastic);
    pAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}

void QtZhiban::RefreshcurrTime()
{
    // 查询数据库
    QSqlDatabase Connection = ConnectionPool::openConnection();
    if (!Connection.isOpen())
    {
        messageTipService->showTip(QString::fromLocal8Bit("服务器连接失败！"), TT_FAILED);
        return;
    }

    QSqlQuery query(Connection);
    query.prepare(QString("SELECT CURRENT_DATE;"));
    bool success = query.exec();
    if (!success)
    {
        QString err = query.lastError().databaseText();
        LOG_ERROR(QString::fromLocal8Bit("读取服务器时间失败：%s"), err.toStdString());
        messageTipService->showTip(QStringLiteral("读取服务器时间失败！"), TipType::TT_FAILED);
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
    // 关闭数据库连接
    ConnectionPool::closeConnection(Connection);
}

void QtZhiban::updateActions()
{
    bool hasSelection = !ui->view->selectionModel()->selection().isEmpty();
    ui->removeRowAction->setEnabled(hasSelection);
    // ui->removeColumnAction->setEnabled(hasSelection);

    bool hasCurrent = ui->view->selectionModel()->currentIndex().isValid();
    ui->insertRowAction->setEnabled(hasCurrent);
    // ui->insertColumnAction->setEnabled(hasCurrent);

    if (hasCurrent) {
        ui->view->closePersistentEditor(ui->view->selectionModel()->currentIndex());

        int row = ui->view->selectionModel()->currentIndex().row();
        int column = ui->view->selectionModel()->currentIndex().column();
        //if (view->selectionModel()->currentIndex().parent().isValid())
        //    statusBar()->showMessage(tr("Position: (%1,%2)").arg(row).arg(column));
        //else
        //    statusBar()->showMessage(tr("Position: (%1,%2) in top level").arg(row).arg(column));
    }
}

void QtZhiban::insertChild()
{
    QModelIndex index = ui->view->selectionModel()->currentIndex();
    QAbstractItemModel* model = ui->view->model();

    if (model->columnCount(index) == 0) {
        if (!model->insertColumn(0, index))
            return;
    }

    if (!model->insertRow(0, index))
        return;

    for (int column = 0; column < model->columnCount(index); ++column) {
        QModelIndex child = model->index(0, column, index);
        model->setData(child, QVariant("[No data]"), Qt::EditRole);
        if (!model->headerData(column, Qt::Horizontal).isValid())
            model->setHeaderData(column, Qt::Horizontal, QVariant("[No header]"), Qt::EditRole);
    }

    ui->view->selectionModel()->setCurrentIndex(model->index(0, 0, index),
        QItemSelectionModel::ClearAndSelect);
    updateActions();
}

bool QtZhiban::insertColumn()
{
    QAbstractItemModel* model = ui->view->model();
    int column = ui->view->selectionModel()->currentIndex().column();

    // Insert a column in the parent item.
    bool changed = model->insertColumn(column + 1);
    if (changed)
        model->setHeaderData(column + 1, Qt::Horizontal, QVariant("[No header]"), Qt::EditRole);

    updateActions();

    return changed;
}

void QtZhiban::insertRow()
{
    QModelIndex index = ui->view->selectionModel()->currentIndex();
    QAbstractItemModel* model = ui->view->model();

    if (!model->insertRow(index.row() + 1, index.parent()))
        return;

    updateActions();

    for (int column = 0; column < model->columnCount(index.parent()); ++column) {
        QModelIndex child = model->index(index.row() + 1, column, index.parent());
        model->setData(child, QVariant("[No data]"), Qt::EditRole);
    }
}

bool QtZhiban::removeColumn()
{
    QAbstractItemModel* model = ui->view->model();
    int column = ui->view->selectionModel()->currentIndex().column();

    // Insert columns in each child of the parent item.
    bool changed = model->removeColumn(column);

    if (changed)
        updateActions();

    return changed;
}

void QtZhiban::removeRow()
{
    QModelIndex index = ui->view->selectionModel()->currentIndex();
    QAbstractItemModel* model = ui->view->model();
    if (model->removeRow(index.row(), index.parent()))
        updateActions();
}

// void QtZhiban::trayActivated(QSystemTrayIcon::ActivationReason reason)
// {
//     switch (reason)
//     {
//     case QSystemTrayIcon::DoubleClick:  //双击
//         this->show();
//         break;
//     case QSystemTrayIcon::Context:  //点击
//         trayIcon->contextMenu()->show();
//         break;
//     case QSystemTrayIcon::MiddleClick:  //中键点击
//         this->show();
//         break;
//     }
// }

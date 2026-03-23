#pragma once
#include <QMap>
#include <QList>
#include <QString>
#include <QVector>
#include <QDateTime>
#include <QObject>
#include "PlanParse/PlanTemplate.h"

// 登录用户
struct loginStruct
{
	qint64 id;					// 64位UUID
	QString name = "";			// 登录用户名
	QString password = "";		// 密码
	int level = 0;				// 话务员级别，不同级别登录软件界面显示不同
	bool xingbie = 0;			// 默认为女0，男为1
	QString dianhua = "";		// 电话
	QString shoujihao = "";		// 手机号
	QString danwei = "";		// 单位
	QString zhiwu = "";			// 职务
};

// 通讯录用户
struct contactUserStruct
{
	qint64 id;					// 64位UUID
	QString name = "";			// 姓名
	int xiwei = 0;				// 所属席位，无用
	bool xingbie = 0;			// 默认为女0，男为1
	QString dianhua = "";		// 电话
	QString shoujihao = "";		// 手机号
	QString danwei = "";		// 单位
	QString zhiwu = "";			// 职务
};

struct sqlConfig
{
	int id;
	QString type = "";
	QString info = "";
	int level = 0;
};

struct todoStruct
{
	qint64 idGenerate;
	int xiwei;
	QDate date;
	QString title;
	int percent;
	QString miaoshu;
};

class MemoryData
{
public:
	MemoryData();
	~MemoryData();

	// 当前日期
	QDate currentDate;

	// 当前登录用户ID
	qint64 currentloginID;

	// 登录用户数据
	QStringList loginList_name;
	QList<loginStruct> loginList;
	QMap<int, loginStruct> loginMap;

	// 数据库配置表，包含tab页名称
	QMap<int, sqlConfig> sql_configs;

	// 人员信息
	QStringList Renyuanlist;
	QStringList Zhiwulist;
	QMap<int, QList<contactUserStruct>> Renyuanstructs;
	QMap<int, QList<QString>> renyuanmap;
	QMap<int, QList<QString>> zhiwumap;
	QHash<QString, QString> Renyuanzhiwu;

	//存放模板文件的内容和标题
	QMap<QString, QString> templateFiles;
	// 计划方案文件信息
	PlanFileInfo planFileInfo;
	// 记录组内其他成员正在编辑的章节目录
	QList<QString> chapters;
	// 记录其他席位已编辑完成的章节内容,每次更新之后清空
	QMap<QString, QString> chapterContents;
};

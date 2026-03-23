#pragma once

#include <QObject>
#include "MemoryData.h"

class MemoryDataService : public QObject
{
	Q_OBJECT

public:

    MemoryDataService();
    ~MemoryDataService();
    static MemoryDataService *instance();

	QDate GetcurrDate();
	void SetcurrDate(QDate currDate);

	// 登录用户
	QList<loginStruct> GetLoginList();
	void SetLoginList(QList<loginStruct> namelist);
	QMap<int, loginStruct> GetLoginMap();
	void SetLoginMap(QMap<int, loginStruct> nameMap);
	int GetCurrentLoginID();
	void SetCurrentLoginID(int currentloginID);

	// 数据库配置表，包含tab页
	QMap<int, sqlConfig> Getsqlconfig();
	void SetSqlconfig(QMap<int, sqlConfig>);

	// // 职务
	// QStringList GetZhiwuList();
	// void AddZhiwuList(QStringList zhiwuList);

	// 人员
	// QStringList GetRenyuanList();
	// void SetRenyuanList(QStringList renmingList);
	QMap<int, QList<contactUserStruct>>  GetRenyuanStructs();
	void SetRenyuanStructs(QMap<int, QList<contactUserStruct>> renmingList);
	QMap<int, QList<QString>> GetRenyuanMap();
	void SetRenyuanMap(QMap<int, QList<QString>> renyuanmap);
	QMap<int, QList<QString>> GetZhiwuMap();
	void SetZhiwuMap(QMap<int, QList<QString>> renyuanmap);
	

	QHash<QString,QString> GetQmapRenyuanzhiwu();
	void SetQmapRenyuanzhiwu(QHash<QString, QString>);

	// 获取模板文件信息
	QString GetTempalteFile(QString fileName) ;
	void AddTemplateFiles(QString fileName, QString xmlFile) ;
	QStringList GetTemAllFileNames() ;
	// 保存当前计划方案文件信息
	void SavePlanFileInfo(PlanFileInfo info) ;
	PlanFileInfo & GetPlanFileInfo() ;

	void AddEditingChapter(QString chapter) ;             // 添加正在编辑的章节
	void DeleteEditingChapter(QString chapter) ;          // 删除正在编辑的章节 
	void DeleteEditingChapters() ;                        // 关闭计划方案文件的时候，进行清空
	QList<QString> GetEditingChapter() ;                  // 获取正在编辑的章节

	void SaveEditedChapter(QString chapter, QString content) ;
	QMap<QString, QString> & GetEditedChapters() ;
	void DeleteEditedChapters() ;



private:
	MemoryData data;
    static MemoryDataService *_instance;
};



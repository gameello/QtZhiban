#include "MemoryDataService.h"
//#include "Common/DatabaseService.h"
#include "Tip/MessageTipService.h"
#include "Common/SettingService.h"
#include "log/QtCLog.h"
#include "QLocale"
#include "QSqlRecord"
//#include <QxOrm_Impl.h>

MemoryDataService* MemoryDataService::_instance = nullptr;
MemoryDataService::MemoryDataService() {
}

MemoryDataService::~MemoryDataService() {
}

MemoryDataService* MemoryDataService::instance() {
    if (_instance == nullptr) {
        _instance = new MemoryDataService();
    }
    return _instance;
}

QDate MemoryDataService::GetcurrDate()
{
	return data.currDate;
}

void MemoryDataService::SetcurrDate(QDate currDate)
{
	data.currDate = currDate;
}

QList<loginStruct> MemoryDataService::GetSeatList()
{
	return data.loginList;
}

void MemoryDataService::AddSeatNameList(QList<loginStruct> namelist)
{
	data.loginList = namelist;
}

QMap<int, loginStruct> MemoryDataService::GetSeatMap()
{
	return data.loginMap;
}

void MemoryDataService::AddSeatNameMap(QMap<int, loginStruct> nameMap)
{
	data.loginMap = nameMap;
}

int MemoryDataService::GetSeatName()
{
	return data.curSeatName;
}

void MemoryDataService::SetCurrSeatName(int currName)
{
	data.curSeatName = currName;
}

QMap<int, sqlConfig> MemoryDataService::Getsqlconfig()
{
	return data.sql_configs;
}

void MemoryDataService::SetSqlconfig(QMap<int, sqlConfig> map)
{
	data.sql_configs = map;
}

// QStringList MemoryDataService::GetZhiwuList()
// {
// 	return data.Zhiwulist;
// }
//
// void MemoryDataService::AddZhiwuList(QStringList zhiwuList)
// {
// 	data.Zhiwulist = zhiwuList;
// }

QMap<int, QList<contactUserStruct>> MemoryDataService::GetRenyuanStructs()
{
	return data.Renyuanstructs;
}

void MemoryDataService::SetRenyuanStructs(QMap<int, QList<contactUserStruct>> renmingList)
{
	data.Renyuanstructs = renmingList;
}

QMap<int, QList<QString>> MemoryDataService::GetRenyuanMap()
{
	return data.renyuanmap;
}

void MemoryDataService::SetRenyuanMap(QMap<int, QList<QString>> renyuanmap)
{
	data.renyuanmap = renyuanmap;
}

QMap<int, QList<QString>> MemoryDataService::GetZhiwuMap()
{
	return data.zhiwumap;
}

void MemoryDataService::SetZhiwuMap(QMap<int, QList<QString>> renyuanmap)
{
	data.zhiwumap = renyuanmap;
}

// QStringList MemoryDataService::GetRenyuanList()
// {
// 	return data.Renyuanlist;
// }
//
// void MemoryDataService::SetRenyuanList(QStringList renmingList)
// {
// 	data.Renyuanlist = renmingList;
// }

QHash<QString, QString> MemoryDataService::GetQmapRenyuanzhiwu()
{
	return data.Renyuanzhiwu;
}

void MemoryDataService::SetQmapRenyuanzhiwu(QHash<QString, QString> renyuanzhiwu)
{
	data.Renyuanzhiwu = renyuanzhiwu;
}

QString MemoryDataService::GetTempalteFile(QString fileName)
{
	QString xmlFile = NULL;
	if (data.templateFiles.contains(fileName))
	{
		xmlFile = data.templateFiles.value(fileName);
	}
	return xmlFile;
}

void MemoryDataService::AddTemplateFiles(QString fileName, QString xmlFile)
{
	if (!data.templateFiles.contains(fileName))
	{
		data.templateFiles.insert(fileName, xmlFile);
	}
	else
	{
		data.templateFiles[fileName] = xmlFile;
	}
}

QStringList MemoryDataService::GetTemAllFileNames()
{
	QStringList allFile = QStringList("");
	if (!data.templateFiles.isEmpty())
	{
		allFile = data.templateFiles.keys();
	}
	return allFile;
}

void MemoryDataService::SavePlanFileInfo(PlanFileInfo info)
{
	data.planFileInfo = info;
}

PlanFileInfo & MemoryDataService::GetPlanFileInfo()
{
	return data.planFileInfo;
}

void MemoryDataService::AddEditingChapter(QString chapter)
{
	data.chapters.push_back(chapter);
}

void MemoryDataService::DeleteEditingChapter(QString chapter)
{
	data.chapters.removeOne(chapter);
}

void MemoryDataService::DeleteEditingChapters()
{
	data.chapters.clear();
}

QList<QString> MemoryDataService::GetEditingChapter()
{
	return data.chapters;
}

void MemoryDataService::SaveEditedChapter(QString chapter, QString content)
{
	data.chapterContents.insert(chapter, content);
}

QMap<QString, QString> & MemoryDataService::GetEditedChapters()
{
	return data.chapterContents;
}

void MemoryDataService::DeleteEditedChapters()
{
	data.chapterContents.clear();
}

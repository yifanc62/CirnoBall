#ifndef __BLOCKBREAKER_DATAFILEIO_H__
#define __BLOCKBREAKER_DATAFILEIO_H__

#include "cocos2d.h"

USING_NS_CC;

//字符串工具类 用于按指定分隔符拆分字符串并拆成一组Value放进Vector
class StringUtil :public Ref {
public:
	static StringUtil* getInstance(); //获取一个新对象
	virtual bool init();
	ValueVector split(const char* srcStr, const char* sSep); //拆分函数 srcStr是要拆分的字符串 sSep是拆分根据的分隔符
private:
	static StringUtil* m_StringUtil;
};
//文件数据类 用于在内存中存储文件数据
class FileData :public Ref {
public:
	CREATE_FUNC(FileData);
	virtual bool init();
	void addLineData(ValueVector lineData); //给Vector添加一行数据
	void deleteEndLineData(); //删除Vector中结尾行的数据
	void insertLineData(int iLine, ValueVector lineData); //在Vector某行前添加一行数据
	ValueVector getLineData(int iLine); //获取某行的数据
	float getLineNum(); //获取Vector行数
private:
	ValueVector linesVec;
};
//文件IO类 用于在数据中获取所需的信息
class DataFileIO :public Ref {
public:
	static DataFileIO* getInstance(); //获取一个新对象
	virtual bool init();
	void loadFile(const char* sPath); //加载指定资源路径的一个文件至一个文件数据对象
	void writeFile(const char* sPath, FileData* data); //将文件内容写入文件
	float getLineNum(const char* sPath); //获取数据行数
	FileData* getData(const char* sPath); //获取整块数据
	int getBlockX(int iLine, const char* sPath); //在文件中读取砖块的x坐标
	int getBlockY(int iLine, const char* sPath); //在文件中读取砖块的y坐标
	int getBlockType(int iLine, const char* sPath); //在文件中读取砖块的类型
	int getBlockItem(int iLine, const char* sPath); //在文件中读取砖块包含的道具类型
	long getRecordScore(int iLine, const char* sPath); //在成绩存档中读取游戏成绩
	std::string getRecordName(int iLine, const char* sPath); //在成绩存档中读取玩家名
	int getRecordDifficulty(int iLine, const char* sPath); //在成绩存档中读取游戏难度
private:
	Map<std::string, FileData*> mFileMap;
	static DataFileIO* m_DataUtil;
};
#endif // __BLOCKBREAKER_DATAFILEIO_H__
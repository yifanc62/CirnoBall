#include "DataFileIO.h"

StringUtil* StringUtil::m_StringUtil = NULL;
StringUtil* StringUtil::getInstance() {
	if (m_StringUtil == NULL) {
		m_StringUtil = new StringUtil();
		if (m_StringUtil && m_StringUtil->init()) {
			m_StringUtil->autorelease();
			m_StringUtil->retain();
		}
		else {
			CC_SAFE_DELETE(m_StringUtil);
			m_StringUtil = NULL;
		}
	}
	return m_StringUtil;
}
bool StringUtil::init() {
	return true;
}
ValueVector StringUtil::split(const char* srcStr, const char* sSep) {
	ValueVector stringList;
	int size = strlen(srcStr);
	Value str = Value(srcStr);
	int startIndex = 0;
	int endIndex = 0;
	endIndex = str.asString().find(sSep);
	std::string lineStr;
	while (endIndex > 0) {
		lineStr = str.asString().substr(startIndex, endIndex);
		stringList.push_back(Value(lineStr));
		str = Value(str.asString().substr(endIndex + 1, size));
		endIndex = str.asString().find(sSep);
	}
	if (str.asString().compare("") != 0) {
		stringList.push_back(Value(str.asString()));
	}
	return stringList;
}

void FileData::addLineData(ValueVector lineData) {
	linesVec.push_back(Value(lineData));
}
void FileData::deleteEndLineData() {
	linesVec.erase(linesVec.end() - 1);
}
void FileData::insertLineData(int iLine, ValueVector lineData) {
	linesVec.insert(linesVec.begin() + iLine, Value(lineData));
}
ValueVector FileData::getLineData(int iLine) {
	return linesVec.at(iLine).asValueVector();
}
float FileData::getLineNum() {
	return linesVec.size();
}
bool FileData::init() {
	return true;
}

DataFileIO* DataFileIO::m_DataUtil = NULL;
DataFileIO* DataFileIO::getInstance() {
	if (m_DataUtil == NULL) {
		m_DataUtil = new DataFileIO();
		if (m_DataUtil && m_DataUtil->init()) {
			m_DataUtil->autorelease();
			m_DataUtil->retain();
		}
		else{
			CC_SAFE_DELETE(m_DataUtil);
			m_DataUtil = NULL;
		}
	}
	return m_DataUtil;
}
void DataFileIO::loadFile(const char* sPath) {
	FileData* data = FileData::create();
	std::string str = FileUtils::getInstance()->getStringFromFile(sPath);
	ValueVector linesVec = StringUtil::getInstance()->split(str.c_str(), "\n");
	for (auto value : linesVec) {
		ValueVector strOfSingleLineVec = StringUtil::getInstance()->split(value.asString().c_str(), ",");
		data->addLineData(strOfSingleLineVec);
	}
	mFileMap.insert(sPath, data);
}
void DataFileIO::writeFile(const char* sPath, FileData* data) {
	FILE* fp = fopen(sPath, "wt");
	if (fp) {
		for (int i = 0; i < data->getLineNum(); i++) {
			std::string str = data->getLineData(i).at(0).asString() + "," + data->getLineData(i).at(1).asString() + "," + data->getLineData(i).at(2).asString() + "\n";
			fputs(str.c_str(), fp);
		}
		log("record.bin append complete");
		fclose(fp);
	}
}
FileData* DataFileIO::getData(const char* sPath) {
	auto data = mFileMap.at(sPath);
	if (data == nullptr) {
		loadFile(sPath);
		data = mFileMap.at(sPath);
	}
	return data;
}
float DataFileIO::getLineNum(const char* sPath) {
	auto data = mFileMap.at(sPath);
	if (data == nullptr) {
		loadFile(sPath);
		data = mFileMap.at(sPath);
	}
	return data->getLineNum();
}
int DataFileIO::getBlockX(int iLine, const char* sPath) {
	auto data = mFileMap.at(sPath);
	if (data == nullptr) {
		loadFile(sPath);
		data = mFileMap.at(sPath);
	}
	return data->getLineData(iLine).at(0).asInt();
}
int DataFileIO::getBlockY(int iLine, const char* sPath) {
	auto data = mFileMap.at(sPath);
	if (data == nullptr) {
		loadFile(sPath);
		data = mFileMap.at(sPath);
	}
	return data->getLineData(iLine).at(1).asInt();
}
int DataFileIO::getBlockType(int iLine, const char* sPath) {
	auto data = mFileMap.at(sPath);
	if (data == nullptr) {
		loadFile(sPath);
		data = mFileMap.at(sPath);
	}
	return data->getLineData(iLine).at(2).asInt();
}
int DataFileIO::getBlockItem(int iLine, const char* sPath) {
	auto data = mFileMap.at(sPath);
	if (data == nullptr) {
		loadFile(sPath);
		data = mFileMap.at(sPath);
	}
	return data->getLineData(iLine).at(3).asInt();
}
long DataFileIO::getRecordScore(int iLine, const char* sPath) {
	auto data = mFileMap.at(sPath);
	if (data == nullptr) {
		loadFile(sPath);
		data = mFileMap.at(sPath);
	}
	return data->getLineData(iLine).at(0).asInt();
}
std::string DataFileIO::getRecordName(int iLine, const char* sPath) {
	auto data = mFileMap.at(sPath);
	if (data == nullptr) {
		loadFile(sPath);
		data = mFileMap.at(sPath);
	}
	return data->getLineData(iLine).at(1).asString();
}
int DataFileIO::getRecordDifficulty(int iLine, const char* sPath) {
	auto data = mFileMap.at(sPath);
	if (data == nullptr) {
		loadFile(sPath);
		data = mFileMap.at(sPath);
	}
	return data->getLineData(iLine).at(2).asInt();
}
bool DataFileIO::init() {
	return true;
}
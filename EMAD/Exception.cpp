#include "Exception.h"
#include <sstream>

BaseException::BaseException(int line, const std::string& file) noexcept
	:mLine(line)
	,mFile(file){}

const char* BaseException::what() const noexcept
{
	std::ostringstream oss;

	// 获取基础错误信息(行、文件)
	oss << getBaseErrorStr();
	// 从子类获取错误信息
	oss << "[Desc] ";
	oss << mOtherErrorStr;

	mWhatBuffer = oss.str();

	return mWhatBuffer.c_str();
}

std::string BaseException::getBaseErrorStr() const noexcept
{
	std::ostringstream oss;
	oss << "[File] " << mFile << std::endl
		<< "[Line] " << mLine << std::endl;
	return oss.str();
}

InfoException::InfoException(int line, const std::string& file, const std::string& info)
	:BaseException(line, file){
	mOtherErrorStr += info;
	mOtherErrorStr += '\n';
}

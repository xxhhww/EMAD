#include "Exception.h"
#include <sstream>

BaseException::BaseException(int line, const std::string& file) noexcept
	:mLine(line)
	,mFile(file){}

const char* BaseException::what() const noexcept
{
	std::ostringstream oss;
	//�������ȡ������Ϣ
	oss << mOtherErrorStr;
	//��ȡ����������Ϣ(�С��ļ�)
	oss << getBaseErrorStr();

	mWhatBuffer = oss.str();

	return mWhatBuffer.c_str();
}

std::string BaseException::getBaseErrorStr() const noexcept
{
	std::ostringstream oss;
	oss << "[File] " << mFile << std::endl
		<< "[Line] " << mLine;
	return oss.str();
}

InfoException::InfoException(int line, const std::string& file, const std::string& info)
	:BaseException(line, file){
	mOtherErrorStr += info;
	mOtherErrorStr += '\n';
}

#pragma once

#include <string>
#include <exception>

//基础异常类，只管理错误代码所在行与所在文件
class BaseException : public std::exception {
public:
	BaseException(int line, const std::string& file) noexcept;

	const char* what() const noexcept override;
	std::string getBaseErrorStr() const noexcept;

	int getLine() const noexcept { return mLine; }
	const std::string& getFile() const noexcept { return mFile; }

protected:
	std::string mOtherErrorStr; //用于子类填充错误信息
private:
	int mLine;
	std::string mFile;
	mutable std::string mWhatBuffer; //暂存错误信息
};

//输出info
class InfoException : public BaseException {
public:
	InfoException(int line, const std::string& file, const std::string& info);
};

#define THROW_INFO_EXCEPTION(info) throw InfoException(__LINE__, __FILE__, info)
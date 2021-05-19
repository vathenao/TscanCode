#include "gitblamemgr.h"
#include "path.h"
#include <windows.h>
#include <regex>
#include <sstream>

namespace
{
	std::string runCmd(const char* cmd) {
		char buffer[128];
		std::string result = "";
		FILE* pipe = _popen(cmd, "r");
		if (!pipe) throw std::runtime_error("popen() failed!");
		try {
			while (fgets(buffer, sizeof buffer, pipe) != NULL) {
				result += buffer;
			}
		}
		catch (...) {
			_pclose(pipe);
			throw;
		}
		_pclose(pipe);
		return result;
	}
}

/// <summary>
/// blame options class
/// </summary>
BlameOption::BlameOption()
{

}

BlameOption::~BlameOption()
{
}

void BlameOption::addOption(const std::string& key, const std::string& value)
{
	m_opts.insert(std::pair<std::string, std::string>(key, value));
}

const std::map<std::string, std::string>& BlameOption::options() const
{
	return m_opts;
}

/// <summary>
/// git blame command op parser
/// </summary>
GitBlameParser::GitBlameParser()
{

}

GitBlameParser::~GitBlameParser()
{

}

void GitBlameParser::blame(const std::string& fullPath, const BlameOption& option)
{
	std::string filePath = Path::getPathFromFilename(fullPath);
	wchar_t oldDirPath[MAX_PATH];
	if (::GetCurrentDirectory(MAX_PATH, oldDirPath) > 0)
	{
		::SetCurrentDirectoryA(filePath.c_str());
		std::string strCmd = combineCmd(fullPath, option);
		std::string strResult = runCmd(strCmd.c_str());
		std::istringstream iss(strResult);
		std::string line;
		while (std::getline(iss, line, '\n')) {
			if (line == "")
				continue;

			BlameLineInfo info;
			std::regex pattern("[^(]*[(]([^ ]*).*");
			std::smatch results;
			if (std::regex_match(line, results, pattern) && results.size() > 1)
				info.author = results[1].str();

			m_infos.emplace_back(info);
		}

		::SetCurrentDirectory(oldDirPath);
	}
	return;
}

std::vector<BlameLineInfo> GitBlameParser::getBlameInfos()
{
	return m_infos;
}

std::string GitBlameParser::combineCmd(const std::string& fullPath, const BlameOption& option) const
{
	std::string strCmd = "git blame -L 57,57 CVideoDlg.cpp";
	strCmd = "git blame";
	for (auto it = option.options().cbegin(); it != option.options().cend(); it++)
	{
		strCmd.append(" ");
		strCmd.append(it->first);
		strCmd.append(" ");
		strCmd.append(it->second);
	}

	strCmd.append(" ");
	strCmd.append(fullPath);

	return strCmd;
}

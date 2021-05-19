#pragma once

#include "config.h"
#include <vector>
#include <string>
#include <map>

class BlameOption
{
public:
	BlameOption();
	~BlameOption();

	void addOption(const std::string& key, const std::string& value);
	const std::map<std::string, std::string>& options() const;

private:
	std::map<std::string, std::string> m_opts;
};

struct BlameLineInfo
{
	BlameLineInfo() : timeStamp(0){}

	std::string sha1;
	std::string author;
	time_t timeStamp;
	std::string content;
};

class TSCANCODELIB GitBlameParser
{
public:
	GitBlameParser();
	~GitBlameParser();

	void blame(const std::string& fullPath, const BlameOption& option);
	std::vector<BlameLineInfo> getBlameInfos();

private:
	std::string combineCmd(const std::string& fullPath, const BlameOption& option) const;

private:
	std::vector<BlameLineInfo> m_infos;
};
#ifndef FILEREADER_H
#define FILEREADER_H

#include <boost/algorithm/string.hpp>
#include <iostream>     // std::ios, std::istream, std::cout
#include <fstream>  
#include <istream>
#include <string>
#include <vector>
#include "tbb/concurrent_vector.h"

class FileReader
{
public:
	FileReader(std::string filePath)
		:filePath(filePath)
	{
	}
	virtual ~FileReader()
	{}

	std::string GetFilePath() { return filePath; }
	
	tbb::concurrent_vector<std::string>& ReadAll()
	{
		std::string line;
		std::ifstream infile(filePath);

		while (std::getline(infile, line))
		{
			boost::algorithm::trim(line);
			lines.push_back(line);
		}
		infile.close();
		line.shrink_to_fit();
		return lines;
	}
	
	tbb::concurrent_vector<std::string> GetLines()
	{ return ReadAll(); }

private:
	std::string filePath;
	tbb::concurrent_vector<std::string> lines;
};

#endif
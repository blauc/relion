#ifndef __EXTRA_OUTPUT_H
#define __EXTRA_OUTPUT_H

#include <pthread.h>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include "src/filename.h"

static pthread_mutex_t extra_output_push_back = PTHREAD_MUTEX_INITIALIZER;

class ExtraOutput
{

private:
	const FileName outputFile_;
	std::vector<std::string> extraOutput_;
	int rank_;

public:
	ExtraOutput():
		outputFile_("extra_output.dat"), extraOutput_(), rank_(0)
	{}

	void add(const std::string & str)
	{
		pthread_mutex_lock(&extra_output_push_back);
		extraOutput_.push_back(str);
		pthread_mutex_unlock(&extra_output_push_back);
	}

	void add(const std::vector<std::string>& strs)
	{
		std::ostringstream oss;
		for (int i = 0; i < strs.size(); i ++)
			oss << strs[i] << " ";
		add(oss.str());
	}

	void setRank(int rank)
	{
		rank_ = rank;
	}

	void write(FileName fn_root, int iteration)
	{
		std::ofstream outputFileStream;
		std::ostringstream oss;
		oss << fn_root << "_it" << iteration << "_" << rank_ << outputFile_;
		outputFileStream.open(oss.str().c_str(), std::ios::out);
		for (std::vector<std::string>::iterator extraOutputLine = extraOutput_.begin(); extraOutputLine!=extraOutput_.end(); ++extraOutputLine)
		{
			outputFileStream << *extraOutputLine << std::endl;
		}
		outputFileStream.close();
		extraOutput_.clear();
	}
};

#endif // __EXTRA_OUTPUT_H

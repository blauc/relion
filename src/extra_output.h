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
public:

	FileName output_file;
	bool do_extra_output;
	std::vector<std::string> extra_output;

	ExtraOutput():
		output_file("extra_output.dat"), do_extra_output(true), extra_output(0)
	{}

	void add(std::string str)
	{
		if (!do_extra_output)
			return;

		pthread_mutex_lock(&extra_output_push_back);
		extra_output.push_back(str);
		pthread_mutex_unlock(&extra_output_push_back);
	}

	void add(std::vector<std::string> strs)
	{
		if (!do_extra_output)
			return;

		std::ostringstream oss;
		for (int i = 0; i < strs.size(); i ++)
			oss << strs[i] << " ";
		add(oss.str());
	}

	void write(FileName fn_root)
	{
		if (!do_extra_output || extra_output.size() == 0)
			return;

		std::ofstream f;
		f.open((fn_root + output_file).c_str());
		for (unsigned i = 0; i < extra_output.size(); i ++)
			f << extra_output[i] << std::endl;
		f.close();
	}
};

#endif // __EXTRA_OUTPUT_H

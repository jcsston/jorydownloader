#pragma once

struct prof_entry {
	unsigned __int64 total, count;
};

class CProfile
{
public:  
	CProfile(std::string const & _name);
	~CProfile();

	// The following function convert ticks to seconds
	static double SecsFromTicks(__int64 ticks);

private: 
	std::string name; 
	unsigned __int64 start;
};

class CProfiler
{
public:
	CProfiler();
	~CProfiler();

	static void Report(int _count = 0);	

	std::map<std::string, prof_entry> m_ProfileData;
};

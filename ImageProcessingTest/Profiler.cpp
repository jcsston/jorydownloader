#include "StdAfx.h"
#include "Profiler.h"

CProfiler g_Profiler;
std::map<std::string, prof_entry> &prof_data = g_Profiler.m_ProfileData;

CProfile::CProfile(std::string const & _name)    
	: name(_name), start() 
{
	_LARGE_INTEGER pc;   
	::QueryPerformanceCounter(&pc); 
	start = pc.QuadPart; 
}

CProfile::~CProfile() 
{  
	_LARGE_INTEGER pc;  
	::QueryPerformanceCounter(&pc); 
	prof_entry& pe = prof_data[name];   
	pe.total += pc.QuadPart - start; 
	pe.count++; 
}

// The following function convert ticks to seconds
double CProfile::SecsFromTicks(__int64 ticks)
{
	// Query Frecuency
	LARGE_INTEGER	m_QPFrequency;		// ticks/sec resolution
	QueryPerformanceFrequency(&m_QPFrequency);	
	return static_cast<double>(ticks) / static_cast<double>(m_QPFrequency.QuadPart);
}

CProfiler::CProfiler()
{

}

CProfiler::~CProfiler()
{

}

void CProfiler::Report(int _count)
{
	char buffer[256];
	for (std::map<std::string, prof_entry>::const_iterator iter = prof_data.begin();
				iter != prof_data.end();
				iter++)
	{		
		int count = ((int)iter->second.count) + _count;
		double total = CProfile::SecsFromTicks(iter->second.total);
		double average = total / count;
		_snprintf(buffer, 255, "Function: %s Total: %f Average: %f \n", iter->first.c_str(), total, average);
		printf(buffer);
	}
}

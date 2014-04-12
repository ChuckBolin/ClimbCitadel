//Log.cpp
#include "..\includes\Log.h"

//this is the file to write to...appended
//std::string Log::mFileName = "logfile.txt";

Log::Log(){
  mFileName = "logfile.txt";
}

//all functions call this function to write data to log file
void Log::WriteLine(std::ostringstream& oss)
{
	std::ofstream file;
	file.open( mFileName.c_str(), std::ios_base::app );
	if(file){
    file <<  oss.str();
	} 
	file.close();
}

//Miscellaneous overloaded Write(...) functions
void Log::Write(std::string a)
{
	std::ostringstream oss;
  oss << a << std::endl;
  WriteLine(oss);
  oss.str("");
} 

void Log::Write(std::string a, bool b)
{
	std::ostringstream oss;
  oss << a << ", " << b << std::endl;
  WriteLine(oss);
  oss.str("");
} 

void Log::Write(std::string a, int b)
{
	std::ostringstream oss;
  oss << a << ", " << b << std::endl;
  WriteLine(oss);
  oss.str("");
} 

void Log::Write(std::string a, float b)
{
	std::ostringstream oss;
  oss << a << ", " << b << std::endl;
  WriteLine(oss);
  oss.str("");
} 

void Log::Write(std::string a, long b)
{
	std::ostringstream oss;
  oss << a << ", " << b << std::endl;
  WriteLine(oss);
  oss.str("");
} 

void Log::Write(std::string a, double b)
{
	std::ostringstream oss;
  oss << a << ", " << b << std::endl;
  WriteLine(oss);
  oss.str("");
} 

void Log::Write(std::string a, unsigned int b)
{
	std::ostringstream oss;
  oss << a << ", " << b << std::endl;
  WriteLine(oss);
  oss.str("");
} 

void Log::Write(std::string a, unsigned long b)
{
	std::ostringstream oss;
  oss << a << ", " << b << std::endl;
  WriteLine(oss);
  oss.str("");
} 

void Log::Write(std::string a, std::string b)
{
	std::ostringstream oss;
  oss << a << ", " << b << std::endl;
  WriteLine(oss);
  oss.str("");
}  

void Log::Write(int a)
{
	std::ostringstream oss;
  oss << a << std::endl;
  WriteLine(oss);
  oss.str("");
} 

void Log::Write(long a)
{
	std::ostringstream oss;
  oss << a << std::endl;
  WriteLine(oss);
  oss.str("");
} 

void Log::Write(float a)
{
	std::ostringstream oss;
  oss << a << std::endl;
  WriteLine(oss);
  oss.str("");
} 

void Log::Write(double a)
{
	std::ostringstream oss;
  oss << a << std::endl;
  WriteLine(oss);
  oss.str("");
} 

void Log::WriteTime()
{
	std::ostringstream oss;
	std::ofstream file;
	SYSTEMTIME st;
	::GetSystemTime(&st);
  
	file.open( mFileName.c_str(), std::ios_base::app );
	if(file){
    oss.str(""); 
		oss << "Time: " << st.wHour << ":" << st.wMinute << ":" << st.wSecond << std::endl;
    file <<  oss.str();
	} 
	file.close();
} 

void Log::WriteDate()
{
	std::ostringstream oss;
	std::ofstream file;
	SYSTEMTIME st;
	::GetSystemTime(&st);
  
	file.open( mFileName.c_str(), std::ios_base::app );
	if(file){
    oss.str(""); 
		oss << "Date: " << st.wMonth << "/" << st.wDay << "/" << st.wYear << std::endl;
    file <<  oss.str();
	} 
	file.close();
}

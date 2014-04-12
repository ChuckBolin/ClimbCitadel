//Utility.cpp
#include "..\includes\utility.h"

//returns std::string for current directory
std::string GetCurrDir(){

  TCHAR Buffer[MAX_PATH];
  DWORD dwRet;
  dwRet = GetCurrentDirectory(MAX_PATH, Buffer);
  std::ostringstream oss;
  oss << Buffer;
  std::string s = oss.str();
  return s;
}

//fileName for logfile

//Logs time to logfile.txt
void LogTime()
{
	std::ostringstream oss;
	std::ofstream file;
	SYSTEMTIME st;
	::GetSystemTime(&st);
  std::string fileName = "log.txt";
  
	file.open( fileName.c_str(), std::ios_base::app );
	if(file){
    oss.str(""); 
		oss << "Time: " << st.wHour << ":" << st.wMinute << ":" << st.wSecond << std::endl;
    file <<  oss.str();
	} 
	file.close();
}

//Logs date to file
void LogDate()
{
	//std::string fileName = "logfile.txt";
	std::ostringstream oss;
	std::ofstream file;
	SYSTEMTIME st;
	::GetSystemTime(&st);
std::string fileName = "log.txt";
  
	file.open( fileName.c_str(), std::ios_base::app );
	if(file){
    oss.str(""); 
		oss << "Date: " << st.wMonth << "/" << st.wDay << "/" << st.wYear << std::endl;
    file <<  oss.str();
	} 
	file.close();
}

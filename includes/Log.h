//Log.
#ifndef LOG_H
#define LOG_H

#include <sstream>
#include <string>
#include <fstream>
#include <windows.h>

class Log
{
public:
   Log();
   void Write(std::string a);
   void Write(std::string a, bool b);
   void Write(std::string a, int b);
   void Write(std::string a, float b);
   void Write(std::string a, long b);
   void Write(std::string a, double b);
   void Write(std::string a, unsigned int b);
   void Write(std::string a, unsigned long b);
   void Write(std::string a, std::string b);
   void Write(int a);
   void Write(long a);
   void Write(float a);
   void Write(double a);
   void WriteTime();
   void WriteDate();
  
private:
   void WriteLine(std::ostringstream &oss);
   std::string mFileName;
   
};

#endif //LOG_H
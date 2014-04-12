#include "..\includes\CINIReader.h"

CINIReader::CINIReader(){
}

CINIReader::CINIReader(std::string filename){
  m_Filename = filename;
  std::string line;
  m_NumberOfLines = 0;
  
  std::ifstream file(filename.c_str());
  if(file.is_open()){
    m_bFileExists = true;

    while(!file.eof()){
      getline (file, line);
      line = RemoveSpaces(line);
      line = RemoveComments(line);

      if(line.size() != 0){
        m_NumberOfLines++;
        m_Lines.push_back(line);
      }
    }

    file.close();
  }
  else
    m_bFileExists = false;

}

//removes spaces and comments
void CINIReader::LoadFile(std::string filename){
  m_Filename = filename;
  std::string line;
  m_NumberOfLines = 0;
  m_Lines.clear();
  int equalCount = 0;
  std::ifstream file(filename.c_str());
  
  if(file.is_open()){
    m_bFileExists = true;

    while(!file.eof()){
      getline (file, line);
      line = RemoveSpaces(line);
      line = RemoveComments(line);
      equalCount = CountEqualSigns(line);
      if(line.size() != 0){// && equalCount == 1){
        m_NumberOfLines++;
        m_Lines.push_back(line);
      }
    }

    file.close();
  }
  else
    m_bFileExists = false;

}

//returns number of lines...without comments, blank lines
int CINIReader::GetNumberOfLines(){
  return m_NumberOfLines;
}


//returns number of lines in the file
std::string CINIReader::GetLineFromFile(int i){
  if(i < 0 || i > m_NumberOfLines)
    return "";

  return m_Lines[i];
}

//returns number of terms in a line in the m_Lines vector
int CINIReader::GetNumberOfTermsFromLine(int i){
  if(i < 0 || i > m_NumberOfLines)
    return 0;

  return CountEqualSigns(m_Lines[i]);
}


//returns term in a equal delimited
std::string CINIReader::GetTerm(std::string line, int term){
  int equals = CountEqualSigns(line);
  if (equals < 1 || equals > 1)
    return ""; 

  int pos = line.find("=");

  if(term == 1)
    return line.substr(0, pos);
  else if(term == 2)
    return line.substr(pos + 1, line.size() - pos + 1);

  return "";
}

//***************************************** PRIVATE METHODS **********************
//removes all spaces from a string
std::string CINIReader::RemoveSpaces(std::string in){
  std::string temp;

  for(int i = 0; i < in.size(); i++){
    if(in.substr(i, 1) != " "){
      temp = temp + in.substr(i,1);      
    }
  }

  return temp;
}


//removes all comments from a string //
std::string CINIReader::RemoveComments(std::string in){
  std::string temp;

  for(int i = 0; i < in.size(); i ++){
    if(in.substr(i,2) != "//")
      temp = temp + in.substr(i, 1);
    else
      return temp;
  }
  return temp;
}

//returns the number of commas in a string
int CINIReader::CountCommas(std::string in){
  int count = 0;

  for(int i = 0; i < in.size(); i++){
    if(in.substr(i, 1) == ",")
      count++;

  }
  
  return count;
}

//counts number of equal signs in string
int CINIReader::CountEqualSigns(std::string in){
  int count = 0;

  for(int i = 0; i < in.size(); i++){
    if(in.substr(i, 1) == "=")
      count++;
  }
  
  return count;
}

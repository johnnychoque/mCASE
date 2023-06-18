#include <vector>
#include <string>
#include <fstream>
using namespace std;

#include "configFile.h"

#define DBG_LEVEL 1
#include "debug.h"

configFile::configFile() {

  DBG_IN("configFile::configFile");

  DBG_OUT("configFile::configFile");
}

configFile::~configFile()
{
  DBG_IN("configFile::~configFile");

  DBG_OUT("configFile::~configFile");
}

int configFile::suppressSpaces(char *line)
{
  int len;
  int i,j;

  DBG_IN("suppressSpaces");

  i=0;
  j=0;

  len = strlen(line);
  if(len > 0) {
    while(isspace(line[i]) || line[i] == '\t') {
      i++;
    }
    while(i <= len) {
      line[j++] = line[i++];
    }

    i = strlen(line)-1;
    while(i>0  && (isspace(line[i]) || line[i] == '\t')) {
      line[i--] = '\0';
    }
    len = strlen (line);
  }

  DBG_OUT("suppressSpaces");
  return len;
}

int configFile::readLine(char *line, string &section, string &key, vector<string> &values)
{
  int len;
  int i = 0, j = 0;
  string value;
  string key_;
  int separator;
  int ret;

  DBG_IN("configFile::readLine");

  section = key = "";
  len = strlen(line);

  if(line[0] == CONF_SECTION_BEGIN && line[len-1] == CONF_SECTION_END) {
    section = line+1;
    section.erase(section.size()-1,1);
    ret = SECTION;
  }
  else {
    key_ = line;
    separator = key_.find(CONF_KEY_ASSIGN,0);
    DBG_INFO("Line (%s), Separator = %d", line, separator);
    if(separator != -1) {       // Pair key_value
      key = key_.substr(0,separator);
      value = key_.substr(separator+1,key_.size()-separator);
      values.push_back(value);
      ret = KEY;
    }
    else {                // List
      while(i<=len) {
	while(!isspace(line[i]) && line[i] != '\t' && i<len) {
	  i++;
	}
	value = (line + j);
	if(i < len) {
	  value.erase(i-j,value.size()-(i-j));
	}
	values.push_back(value);
	j=++i;
      }
      key = values[0];
      values.erase(values.begin(), values.begin() + 1);
      ret = LIST;
    }
  }

  DBG_OUT("configFile::readLine");
  return ret;
}

int configFile::getListValues(string section_, string key_, vector<string> &value) 
{
  entries *entry;
  
  DBG_IN("configFile::getListValues");

  if(sectionEntries.count(section_)) {
    entry = sectionEntries[section_];
    if(entry->listEntry.count(key_)) {
      value = entry->listEntry[key_];
    }
    else {
      DBG_ERR("Unknown key (%s)",key_.c_str());
      DBG_OUT("configFile::getListValues");
      return -1;
    }
  } else {
   DBG_ERR("Unknown Section %s", section_.c_str());
   DBG_OUT("configFile::getListValues");
   return -1;
  }

  DBG_OUT("configFile::getListValues");
  return 0;
}

int configFile::getKeyValue(string section_, string key_, string &value) 
{
  entries *entry;

  DBG_IN("configFile::getKeyValue");

  DBG_INFO("Section = %s, Key = %s",section_.c_str(),key_.c_str());
	
  if(sectionEntries.count(section_)) {
    entry = sectionEntries[section_];
    if(entry->keyEntry.count(key_)) {
      value = entry->keyEntry[key_];
    }
    else {
      DBG_ERR("Unknown key (%s)",key_.c_str());
      DBG_OUT("configFile::getKeyValue");
      return -1;
    }
  }
  else {
    DBG_ERR("Unknown section");
    DBG_OUT("configFile::getKeyValue");
    return -1;
  }
  DBG_OUT("configFile::getKeyValue");
  return 0;
}

int configFile::loadConfig(string fileName) 
{
  fstream confFile;
  vector<string> lines;
  vector<string> values;
  char line[MAXLINE];
  unsigned int i,j;
  string sectionName, key;
  entries *sectionEntry = NULL;
  section::iterator iter1;
  keyEntries::iterator iter2;
  listEntries::iterator iter3;
 
  DBG_IN("configFile::loadConfig");

  confFile.open(fileName.c_str(), ios::in);
  if(!confFile) {
    DBG_ERR("An error happened openning the configuration file");
    DBG_OUT("configFile::LoadConfig");
    return(-1);
  }
  
  while(confFile.getline(line, MAXLINE)) {
    if(suppressSpaces(line) > 0 && line[0] != CONF_COMMENT) {
      lines.push_back(line);
    }
  }
  
  for(i=0;i<lines.size();i++) {
    switch(readLine((char *)lines[i].c_str(),sectionName, key, values)) {
    case SECTION:
      sectionEntry = new entries;
      sectionEntries[sectionName] = sectionEntry;
      values.clear();
      break;
    case KEY:      
      sectionEntry->keyEntry[key] = values[0];
      values.clear();
      break;
    case LIST:
      sectionEntry->listEntry[key] = values;
      values.clear();
      break;
    }     
  }

  for(iter1 = sectionEntries.begin() ; iter1 != sectionEntries.end() ; ++iter1) {
    DBG_INFO("Section %s",iter1->first.c_str());
    for(iter2 = iter1->second->keyEntry.begin() ; iter2 != iter1->second->keyEntry.end(); ++iter2) {
      DBG_INFO("\tkey = %s, value = %s", iter2->first.c_str(), iter2->second.c_str());
    }
    for(iter3 = iter1->second->listEntry.begin() ; iter3 != iter1->second->listEntry.end() ; ++iter3) {
      DBG_INFO("\tlist entry - key = %s", iter3->first.c_str());
      for(j=0;j<(iter3->second).size() ; j++) {
				DBG_INFO("\t Element(%d) = %s",j,(iter3->second)[j].c_str());
      }
    }
  }
  
  confFile.clear();

  DBG_OUT("configFile::loadConfig");
  return 0;
}

void configFile::flushConfig(void) 
{
	section::iterator iter1;
	listEntries::iterator iter3;

	for(iter1 = sectionEntries.begin() ; iter1 != sectionEntries.end() ; ++iter1) {
		DBG_INFO("Section %s",iter1->first.c_str());
		(iter1->second)->keyEntry.clear();
		for(iter3 = iter1->second->listEntry.begin() ; iter3 != iter1->second->listEntry.end() ; ++iter3) {
			(iter3->second).clear();
		}
		delete(iter1->second);
	}
	sectionEntries.clear();
}

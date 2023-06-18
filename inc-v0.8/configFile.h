#ifndef _INC_CONFIGFILE_H
#define _INC_CONFIGFILE_H

#include <string>
#include <map>
#include <vector>

using namespace std;

#define MAXLINE 200

#define CONF_COMMENT '#'
#define CONF_SECTION_BEGIN '['
#define CONF_SECTION_END ']'
#define CONF_KEY_ASSIGN '='

enum {
	SECTION,
	KEY,
	LIST,
};

typedef vector<string> listEntry_;
typedef map<string,listEntry_> listEntries;
typedef map<string,string> keyEntries;
typedef struct {
	keyEntries keyEntry;
	listEntries listEntry;
} entries;
typedef map<string,entries *> section;

class configFile;

class configFile {
	public:
		configFile();
		~configFile();

		int loadConfig(string fileName);
		int getListValues(string section_, string key_, vector<string> &value);
		int getKeyValue(string section, string key, string &value);
		void flushConfig(void);

	private:
		int suppressSpaces(char *line);
		int readLine(char *line, string &section, string &key, vector<string> &values);
		section sectionEntries;
};

#endif // _INC_CONFIGFILE_H
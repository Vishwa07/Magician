#include<map>
#include<vector>
#include<ctime>
#include<iostream>
#include<sstream>
#include<string.h>
using namespace std;
enum FileAction
{
	Idle,
	Read,
	Write,
	Delete
};
class Directory
{
static Directory *Instance;
std::map<string,unsigned long> URLDetails;
std::map<string,string> IDToURL;
Directory();
public:
static Directory* getInstance();
string AssignTokenToWrite(string ImageID, unsigned long ImageSize);
string GetTokenToRead(string imageid);
};

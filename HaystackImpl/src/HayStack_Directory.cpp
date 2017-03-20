// HayStack_Directory.cpp : Defines the entry point for the console application.
//

#include"HayStack_Directory.h"
using namespace std;

Directory::Directory()
{
	string url1= "http://127.0.0.1:8002";
	URLDetails[url1] = 1073741824; //1 gb;
	url1 = "http://127.0.0.1:8003";
	URLDetails[url1] = 1073741824 ;//1 gb;

}


//static member
Directory *Directory::Instance = NULL;


Directory* Directory::getInstance()
{
	if(Instance == NULL)
		Instance = new Directory();
	return Instance;
}

string Directory::AssignTokenToWrite(string ImageID, unsigned long ImageSize)
{
	string result;
	int vindex=0;
  for (std::map<string,unsigned long>::iterator it=URLDetails.begin(); it!=URLDetails.end(); ++it) {	
	   ++vindex;
	   	cout<< it->first << "\n";

		if( it->second  >= ImageSize)
		{
			URLDetails[it->first] -= ImageSize;
			IDToURL[ImageID] = it->first;// what if image names repeat??
			stringstream convert;
			convert << vindex;
			result = (convert.str()+","+ImageID);
			return it->first+ "/" + result;
		}

	}

	return "0";//No writable volumes found. All are full
}
string Directory::GetTokenToRead(string Imageid)
{	
	std::map<string,string>::iterator it;
	it = IDToURL.find(Imageid);
	string result;
	if(it != IDToURL.end())
	{
		return it->second;
	}
	else
		return "Image not found";//Image ID not found in the map.
}

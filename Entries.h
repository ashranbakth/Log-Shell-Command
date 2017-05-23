#ifndef ENTRIES_H_
#define ENTRIES_H_

#include <string>
#include <unordered_map>

using namespace std; 

struct Entry{
	string category;
	string comment;
	long long timestamp; 
	int EntryID; 
	
	Entry(string category1, string comment1, long long timestamp1, int entry); 
};

void StoreMasterLog(vector<Entry> & MasterLog, const string &file);

long long Convert2Timestamp(string & timestamp);

void TimeStampSearch(vector<Entry*> & MasterlogPointer, vector<Entry*> & RecentSearch); 

string Convert2Lower(string & category); 

void CategorySearch(vector<Entry*> & RecentSearch, unordered_map<string, vector <Entry*>> &Search);

void StoreKeyWords(vector<Entry*> & MasterLogPointer, unordered_map<string, vector<Entry*>> & Keywords); 

void AppendLogEntry(vector <Entry> & MasterLog, vector<Entry*> & ExcerptList); 

void AppendSearchResults(vector <Entry*> & RecentSearch, vector <Entry*> & ExcerptList); 

void DeleteLogEntry(vector<Entry*> & ExcerptList);

string Timestamp2String(const long long & timestamp); 

void PrintRecentSearch(vector <Entry*> & RecentSearch); 

void PrintExcerptList(vector <Entry*> & ExcerptList); 

void MoveToBeginning(vector<Entry*> & ExcerptList); 

void MoveToEnd(vector <Entry*> & ExcerptList); 

void KeyWordSearch(unordered_map<string, vector<Entry*>> & Keywords); 

#endif 

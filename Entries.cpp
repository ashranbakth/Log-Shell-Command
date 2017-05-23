#include <iostream>
#include <vector>
#include <sstream>
#include <cstdlib>
#include <string>
#include <fstream>
#include <algorithm>
#include <cctype>
#include "Entries.h"

using namespace std; 


Entry::Entry(string category1, string comment1, long long timestamp1, int entry) :
category(category1), comment(comment1), timestamp(timestamp1), EntryID(entry) {}


//Comparators
struct Lowerbound{
	bool operator() (const Entry* a, long long timestamp){
		return a->timestamp < timestamp; 
	}
};

//struct Upperbound{
//	bool operator() (long long timestamp, const Entry* a){
//		return timestamp < a->timestamp; 
//	}
//};



void StoreMasterLog(vector<Entry> & MasterLog, const string & file){
	
	string time; 
	string category1; 
	string comment1; 
	string s; 
	long long timestamp = 0; 
	int entryid = 0;

	ifstream log; 
	log.open(file); 
	while (getline(log, s)){

		int firstpos = s.find("|"); 
		time = s.substr(0,firstpos); 
		//cout << time << endl; 
		istringstream iss(time); 
		//Parsing the timestamp (month, day, hour, minute, seconds)
		timestamp = Convert2Timestamp(time); 

		int secondpos = s.find("|", firstpos + 1); 
		comment1 = s.substr(secondpos + 1);
		int difference = secondpos - firstpos; 
		category1 = s.substr(firstpos + 1, difference - 1);
		//cout << "Second position: " << secondpos << endl; 
		//cout << "Size: " << s.size() << endl; 
		int limit1 = static_cast<int>(s.size()) - secondpos; 
		//cout << "Limit: " << limit1 << endl; 
		string rest = s.substr(firstpos, limit1); 
		//cout << rest << endl; 
		 
		////Converting months, days, hours, and minutes to seconds and add them all up. 
		//month = month * 100 * 100 * 100 * 100; 
		//day = day * 100 * 100 * 100; 
		//hour = hour * 100 * 100; 
		//minute = minute * 100; 
		//second = month + day + hour + minute; 

		//Initializing an entry and pushing it back into the Masterlog vector. 
		//Entry entry(category1, comment1, timestamp, entryid); 
		MasterLog.push_back(Entry(category1, comment1, timestamp, entryid)); 
		++entryid; 
	}

	log.close(); 
}


long long Convert2Timestamp(string & timestamp){ //Convert the timestamp string into a long long

	timestamp = timestamp.erase(2, 1); 
	timestamp = timestamp.erase(4, 1); 
	timestamp = timestamp.erase(6, 1); 
	timestamp = timestamp.erase(8, 1); 

	long long time = stoll(timestamp); 

	return time;
}

void TimeStampSearch(vector<Entry*> & MasterLogPointer, vector<Entry*> & RecentSearch){ // 't'

	string lowerbound, higherbound, times; 
	cin.ignore();
	getline(cin, times); 
	lowerbound = times.substr(0, times.find('|')); 
	higherbound = times.substr(times.find('|') + 1); 

	if (lowerbound.size() != 14 || higherbound.size() != 14){
		cerr << "Error: Invalid command" << endl;
		return; 
	}
	
	if (!RecentSearch.empty()){
		RecentSearch.clear(); 
	}
	//Turn each (month, day, hour, minute, and seconds) into a long long integer. 
	long long lower = Convert2Timestamp(lowerbound); 
	long long higher = Convert2Timestamp(higherbound); 

	

	auto loweritr = lower_bound(MasterLogPointer.begin(), MasterLogPointer.end(), lower, Lowerbound());
	auto higheritr = lower_bound(MasterLogPointer.begin(), MasterLogPointer.end(), higher, Lowerbound());

	//cout << (*loweritr)->timestamp << endl; 
	//cout << (*higheritr)->timestamp << endl;

	//Store the found entries into the recent search.
	
	int found = 0;
	
		for (auto it = loweritr; it < higheritr && it != MasterLogPointer.end(); ++it){
			RecentSearch.push_back(*it); 
			++found; 
		}
	
	

	cout << found << " entries found\n"; 
}

string Convert2Lower(string & category){ //Convert a string to lowercase letters characters
	string newcategory(category); 
	for (int i = 0; i < static_cast<int>(category.size()); ++i){
		newcategory[i] = tolower(newcategory[i]); 
	}
	return newcategory; 
}

void CategorySearch(vector<Entry*> & RecentSearch, unordered_map<string, vector <Entry*>> &Search){ // 'c'
	string category; 
	cin.ignore();
	getline(cin, category); 
	
	string lowercateg = Convert2Lower(category); 

	int found = Search[lowercateg].size(); 

	cout << found << " entries found\n"; 

	for (int i = 0; i < found; ++i){
		RecentSearch.push_back(Search[lowercateg][i]); 
	}
}

void StoreKeyWords(vector<Entry*> & MasterLogPointer, unordered_map<string, vector<Entry*>> & Keywords){ //Stores the keywords of categories and comments.
	string category;
	string comment; 

	for (int i = 0; i < static_cast<int>(MasterLogPointer.size()); ++i){ //Looping through the MasterLogPointer
		category = MasterLogPointer[i]->category; 
		comment = MasterLogPointer[i]->comment; 

		for (int j = 0; j < static_cast<int>(category.size()); ++j){//Looping through the category word
			if (!isalnum(category[j])){
				category[j] = ' '; 
			}
		}
		for (int h = 0; h < static_cast<int>(comment.size()); ++h){//Looping through the comment word
			if (!isalnum(comment[h])){
				comment[h] = ' '; 
			}
		}
		istringstream iss(category);//Putting the category words into the unordered map "KeyWords"
		string word;
		while (iss >> word){ 
			word = Convert2Lower(word); 
			if ( (!Keywords[word].empty() && Keywords[word].back()->EntryID != MasterLogPointer[i]->EntryID)
					|| (Keywords[word].empty() && isalnum(word[0])) ){
				Keywords[word].push_back(MasterLogPointer[i]); 
			}
		}
		istringstream is(comment);
		string comment1;
		while (is >> comment1){ 
			comment1 = Convert2Lower(comment1); 
			if ( (!Keywords[comment1].empty() && Keywords[comment1].back()->EntryID != MasterLogPointer[i]->EntryID) 
					|| (Keywords[comment1].empty()  && isalnum(comment1[0])) ){
				Keywords[comment1].push_back(MasterLogPointer[i]); 
			}
		}

	}
}

void AppendLogEntry(vector <Entry> & MasterLog, vector<Entry*> & ExcerptList){ // 'a'
	int entryID = 0; 
	cin >> entryID; 

	if (entryID > (static_cast<int>(MasterLog.size()) -1) || entryID < 0){
		cerr << "Error: Invalid command" << endl; 
		return; 
	}

	ExcerptList.push_back(&MasterLog[entryID]); 

	cout << "log entry " << entryID << " appended\n"; 
}

void AppendSearchResults(vector <Entry*> & RecentSearch, vector <Entry*> & ExcerptList){ // 'r'
	/*if (RecentSearch.empty()){
		cerr << "Error: Invalid command(r)" << endl; 
		return; 
	}*/

	for (int i = 0; i < static_cast<int>(RecentSearch.size()); ++i){
		ExcerptList.push_back(RecentSearch[i]); 
	}
	cout << RecentSearch.size() << " log entries appended\n"; 
}

void DeleteLogEntry(vector<Entry*> & ExcerptList){ // 'd'
	int deletenum = 0; 
	cin >> deletenum; 

	if (deletenum < 0 || deletenum > (static_cast<int>(ExcerptList.size()) - 1 )){
		cerr << "Error: Invalid command" << endl;
		return; 
	}
	if (ExcerptList.empty()){
		//cerr << "The ExcerptList is empty!" << endl;
		return; 
	}
	ExcerptList.erase(ExcerptList.begin() + deletenum);
	cout << "excerpt list entry " << deletenum << " deleted\n"; 
}


string Timestamp2String(const long long & timestamp){ //Converting Timestamp long long to a string

	string timestring = to_string(timestamp); 
	/*for (int i = 0; i < static_cast<int>(timestring.size()); ++i){
		cout << i << ": " << timestring[i] << endl; 
	}*/
	string mainstring(14, '0'); 
	mainstring[2] = ':'; 
	mainstring[5] = ':'; 
	mainstring[8] = ':'; 
	mainstring[11] = ':'; 

	int mainindex = static_cast<int>(mainstring.size()) - 1;
	int timeindex = static_cast<int>(timestring.size()) - 1; 
	//cout << mainstring << endl; 
	
	while (timeindex >= 0){
		if (mainstring[mainindex] == '0'){
			mainstring[mainindex] = timestring[timeindex]; 
			--mainindex; 
			--timeindex; 
		}
		else{
			--mainindex; 
		}
	}

	return mainstring; 
}

void PrintRecentSearch(vector <Entry*> & RecentSearch){ // 'g'
	
	if (!RecentSearch.empty()){
		string timestr;
		stringstream ss; 
		for (int i = 0; i < static_cast<int>(RecentSearch.size()); ++i){
			timestr = Timestamp2String(RecentSearch[i]->timestamp);
			ss << RecentSearch[i]->EntryID << "|" << timestr << "|"; 
			ss << RecentSearch[i]->category << "|" << RecentSearch[i]->comment << '\n'; 
		}
		cout << ss.str(); 
		return; 
	}
	else{ 
		return; 
	}
}

void PrintExcerptList(vector <Entry*> & ExcerptList){ // 'p'

	if (!ExcerptList.empty()){
		string timestr; 
		stringstream ss; 
		for (int i = 0; i < static_cast<int>(ExcerptList.size()); ++i){
			timestr = Timestamp2String(ExcerptList[i]->timestamp); 
			ss << i << "|" << ExcerptList[i]->EntryID << "|"; 
			ss << timestr << "|" << ExcerptList[i]->category << "|"; 
			ss << ExcerptList[i]->comment << '\n'; 
		}
		cout << ss.str(); 
		return; 
	}
	else{
		return; 
	}
}


void MoveToBeginning(vector<Entry*> & ExcerptList){ // 'b'
	int excerptentry; 
	cin >> excerptentry; 
	if (excerptentry < 0 || excerptentry >(static_cast<int>(ExcerptList.size()) - 1)){
		cerr << "Error: Invalid command" << endl; 
		return; 
	}
	if (!ExcerptList.empty()){
		Entry *a = ExcerptList[excerptentry];
		ExcerptList.erase(ExcerptList.begin() + excerptentry); 
		ExcerptList.insert(ExcerptList.begin(), a); 
		cout << "excerpt list entry " << excerptentry << " moved\n"; 
	}
	return; 
}

void MoveToEnd(vector <Entry*> & ExcerptList){ // 'e'
	int excerptentry; 
	cin >> excerptentry; 
	if (excerptentry < 0 || excerptentry >(static_cast<int>(ExcerptList.size()) - 1)){
		cerr << "Error: Invalid command" << endl;
		return;
	}
	if (!ExcerptList.empty()){
		Entry *a = ExcerptList[excerptentry]; 
		ExcerptList.erase(ExcerptList.begin() + excerptentry); 
		ExcerptList.push_back(a); 
		cout << "excerpt list entry " << excerptentry << " moved\n"; 
		return; 
	}
}



#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <sstream>
#include "Entries.h"

using namespace std;

struct Timestampsort{
	bool operator() (const Entry *a, const Entry* b){
		if (a->timestamp != b->timestamp){
			return a->timestamp < b->timestamp;
		}
		else if (a->category != b->category){
			return a->category < b->category; 
		}
		else{
			return a->EntryID < b->EntryID; 
		}
	}
};

int main(int, char **argv){

	ios_base::sync_with_stdio(false);

	//Initializing containers
	vector<Entry> MasterLog;
	vector<Entry*> MasterLogPointers;
	vector<Entry*> RecentSearch; 
	unordered_map<string, vector <Entry*>> Category; 
	unordered_map<string, vector<Entry*>> Keywords; 
	vector<Entry*> ExcerptList; 

	//The filename of the MasterLog
	string file = argv[1]; 

	//Store the whole masterlog in the MasterLog vector. 
	StoreMasterLog(MasterLog, file);

	cout << MasterLog.size() << " entries read\n"; 
	

	//Make a vector of pointers pointing to entries in the MasterLog
	for (int i = 0; i < static_cast<int>(MasterLog.size()); ++i){
		MasterLogPointers.push_back(& MasterLog[i]);
	}

	//Sort the MasterLogPointers by timestamp (then category, then EntryID). 
	sort(MasterLogPointers.begin(), MasterLogPointers.end(), Timestampsort());


	//Put all the string categories into the unordered map "Category"
	/*string category; 
	for (int i = 0; i < static_cast<int>(MasterLogPointers.size()); ++i){
		category = Convert2Lower(MasterLogPointers[i]->category);
		//cout << category << endl; //REMOVE
		Category[category].push_back(MasterLogPointers[i]);
	}*/

	//StoreKeyWords(MasterLogPointers, Keywords);

	//*************Get the commands***********************
	
	bool tflag = true, cflag = true, kflag = true; 
	char i = '0'; 

	while (i != 'q'){

		cout << "% "; 
		cin >> i;

		if (i == 't'){
			tflag = false; 
			TimeStampSearch(MasterLogPointers, RecentSearch);
		}
		else if (i == 'c'){
			if(cflag){
				cflag = false; 
				string category; 
				for (int i = 0; i < static_cast<int>(MasterLogPointers.size()); ++i){
					category = Convert2Lower(MasterLogPointers[i]->category);
					//cout << category << endl; //REMOVE
					Category[category].push_back(MasterLogPointers[i]);
				}
			}
			
			if (!RecentSearch.empty()){
				RecentSearch.clear(); 
			}

			CategorySearch(RecentSearch, Category); 
		}
		else if (i == 'k'){
		
			if(kflag){
				kflag = false; 
				StoreKeyWords(MasterLogPointers, Keywords);
			}
			
			if (!RecentSearch.empty()){
				RecentSearch.clear(); 
			}
			//KeyWordSearch(Keywords); 
			string searchwords;
			vector <string> a; 
			 
			getline(cin, searchwords); 

			for (int j = 0; j < static_cast<int>(searchwords.size()); ++j){//Put whitespace if the character is not alphanumeric
				if (!isalnum(searchwords[j])){
					searchwords[j] = ' ';
				}
			}
			istringstream iss(searchwords); //Parsing the input string and putting it in a vector
			string word;
			while (iss >> word){
				word = Convert2Lower(word);
				a.push_back(word);
				//cout << "word: " << word << endl;  //REMOVE
			}
			if(a.size() > 1){
				vector <Entry*> entry(Keywords[a[0]].begin(), Keywords[a[0]].end());
				for (int i = 1; i < static_cast<int>(a.size()); ++i){
					vector<Entry*> temp;
					set_intersection(entry.begin(), entry.end(), Keywords[a[i]].begin(), Keywords[a[i]].end(), back_inserter(temp), Timestampsort()); 
					entry.assign(temp.begin(), temp.end()); 
				}
				for (int i = 0; i < static_cast<int>(entry.size()); ++i){ 
					RecentSearch.push_back(entry[i]); 	
					//cout << entry[i]->timestamp <<  entry[i]->category << "|" << entry[i]->comment << '\n'; //REMOVE
				}
			}
			else{
				for (int i = 0; i < static_cast<int>(Keywords[word].size()); ++i){
					RecentSearch.push_back(Keywords[word][i]); 
				}
			}
			cout << RecentSearch.size() << " entries found\n"; 
		}
		else if (i == 'a'){
			AppendLogEntry(MasterLog, ExcerptList); 
		}
		else if (i == 'r'){
			if( tflag == true && cflag == true && kflag == true ){
					cerr << "Error: Invalid command" << endl;  
					continue; 
			}
			AppendSearchResults(RecentSearch, ExcerptList); 
		}
		else if (i == 'd'){
			DeleteLogEntry(ExcerptList); 
		}
		else if (i == 'b'){
			MoveToBeginning(ExcerptList); 
		}
		else if (i == 'e'){
			MoveToEnd(ExcerptList); 
		}
		else if (i == 's'){
			sort(ExcerptList.begin(), ExcerptList.end(), Timestampsort());
			cout << "excerpt list sorted\n";
		}
		else if (i == 'l'){
			ExcerptList.clear(); 
			cout << "excerpt list cleared\n"; 
		}
		else if (i == 'g'){
			if( tflag == true && cflag == true && kflag == true){
				cerr << "Error: Invalid command" << endl; 
				continue; 
			}
			PrintRecentSearch(RecentSearch); 
		}
		else if (i == 'p'){
			PrintExcerptList(ExcerptList); 
		}
		else if (i == 'q'){
			return 0; 
		}
		else if (i == '#'){
			string wtfisthis; 
			getline(cin,wtfisthis);  
		}
		else{
			cerr << "Error: Invalid command" << endl; 
		}
	}








	return 0;
}

#include "Header.h"

extern vector<string> AllWordList;
extern vector<string> HashList;

int main()
{

	HashTable Words2;
	Words2.initTable();
	Words2.countWords();
	cout << "Total Words: " << Words2.wordCount << "\nArray Elements Used: " << Words2.arrayCount << "\nCollisions: " << Words2.collisions << endl;
	cout << "Max Elements in a Bucket: " << Words2.maxBucket << "\nMax Bucket Depth: " << Words2.maxHeight << endl;
   cout<<endl;
 
	Words2.SearchWords();

	return 0;
}
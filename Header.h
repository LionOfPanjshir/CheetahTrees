#include <iostream>
#include <cmath>
#include <array>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <functional>

using namespace std;

//const int TABLE_SIZE = 5387; // Random large prime #; Used 1463 elements, 250 collisions, maxBucket = 4, maxDepth = 2.
const int TABLE_SIZE = 17576; //=pow(26, 3);      //3433; // Prime # > 2*elements(1713); Used 1356 elements, 357 collisions, maxBucket = 4, maxDepth = 2.
const int R = 5381; // Prime # less than table_size

struct docInfo
{
	string title;
	docInfo(string newtitle)
	{
		title = newtitle;
	}

	docInfo()
	{

	}
};

//Used to keep track of the frequency of each word in each document
struct wordInfo
{
	int docNum;
	int freq;

	wordInfo(int doc)
	{
		docNum = doc;
		freq = 1;
	}
};


//AVLTree node
class WordNode
{
public:
	string word;
	/*
	The height is kept at each node to calculate the 
	balance factor, instead of keeping the balance factor.
	*/

	WordNode* left;
	WordNode* right;
	int height;
	vector<wordInfo> nfo;

	WordNode(string newword, int docnum)
	{
		word = newword;
		left = NULL;
		right = NULL;
	}

	WordNode()
	{
		left = NULL;
		right = NULL;
	}

};

class AVLTree
{
public:
	WordNode* root;

	AVLTree() { root = NULL; }
	~AVLTree() { removeLeaves(root); }
	WordNode* insert(WordNode*& tree, string word1, int docnum);
	WordNode* remove(WordNode*& node, string word1);
	void removeLeaves(WordNode*& node);
	void addInfo(vector<wordInfo> &info, int docnum);
	void inorderPrint(WordNode* node);
	int nodeCount(WordNode* node);
   int minlength(string, string);

private:

	int Height(WordNode* node);
	void rwlChild(WordNode*& node);
	void rwrChild(WordNode*& node);
	void doublerwlChild(WordNode*& node);
	void doublerwrChild(WordNode*& node);
	int getBalFac(WordNode* node);
};

//comment section



class HashTable
{
public:

	AVLTree WordTable[TABLE_SIZE];
	docInfo DocTable[50];

	// The following int values are kept for analysis purposes.
	int collisions;
	int wordCount;
	int arrayCount;
	int maxHeight;
	int maxBucket;

	void initTable();
	void PrintTable();
	void insert(string newword, int doc);
	void countWords();
	void SearchWords();
	void SearchMenu();
	WordNode* SearchWords2(WordNode*&, string);
	void IncompleteSearchWords(WordNode*&, string);
	void AndOrSearch(string);
	void Report(WordNode*);

   void test();

	static int HashVal(string newword)
	{

      int len = newword.size(), value=0, totalvalue=0;
      const int dif=97;

      for (int i=0; i<len; ++i)
      {
         if (newword[i]=='*') 
            value=97;
         else
            value = tolower(static_cast<int>(newword[i]));
         if (i<3)
         {
            value = static_cast<int>((value-dif)*pow(26, 2-i));
            totalvalue += value;
         }
         else len=0;
      }

      return abs(totalvalue%TABLE_SIZE);
	}

   
   bool isStopWord(string);
   void importStopWords();


private:
    vector<string> stopwords;
};
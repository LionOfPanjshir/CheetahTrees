#include "Header.h"

void HashTable::initTable()
{
   importStopWords();

	int index = 1;
   //EDIT PATH SO THAT IT WORKS WITH YOUR COMPUTER
	string fname = "C:\\Users\\ahigb\\Documents\\documents\\cranfield00";
	collisions = 0;
	do
	{
		ifstream file;
		ostringstream convert;
		convert << index;
		string indexstr;
		if (index < 10)
		{
			indexstr = "0";
			indexstr.append(convert.str());
		}

		else indexstr = convert.str();

		string filepath = fname;
		filepath.append(indexstr);

		file.open(filepath);

		if (!file) cout << "Cannot find file: " << filepath << endl;

		else
		{
			while (file.good())
			{
				string wrd;
				char brac = '<';
				if (file.peek() == brac)
				{
					char pretitle[4];
					file.get(pretitle, 4);
				if (strcmp(pretitle, "<TI") == 0)
					{
						string newtitle;
						file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
						while (file.peek() != '<')
						{
							char c = file.get();
							if (c != '.') newtitle += c;
							if (isalnum(c))
							{
								wrd += c;
							}
							if (!isalnum(file.peek()))
							{
								if (wrd.size() > 1 && !isStopWord(wrd))
								{
									insert(wrd, index);
									wrd.clear();
								}
								else wrd.clear();
							}
						}
						DocTable[index - 1].title = newtitle;
					}

					else file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				}

				else
				{
					while (isalnum(file.peek()))
					{
						char c = file.get();
						wrd += c;
					}
					if (!isalnum(file.peek())) file.ignore(1);

					// Add a bool function to check if the word is in the list of stopwords
               if (!isStopWord(wrd) && wrd.size()>1) 
                   insert(wrd, index);
			   else wrd.clear();
				}
			}
		}
		file.close();
		index++;
	} while (index < 51);
}

// HashTable insert function; resorts to the AVL insert function for collisions
void HashTable::insert(string newword, int doc)
{
	int hashIndex = HashVal(newword);
   WordTable[hashIndex].root = WordTable[hashIndex].insert(WordTable[hashIndex].root, newword, doc);
}

//AVL Tree Insert function
WordNode* AVLTree::insert(WordNode*& node, string word1, int docnum)
{
	if (node == NULL)
	{
		node = new WordNode(word1, docnum);
      addInfo(node->nfo, docnum);
	}
	else
	{
      int i=0;
      bool done=false;
      int len = minlength(node->word, word1);
      
      if (node->word.compare(word1) == 0)
		{
			addInfo(node->nfo, docnum);
		}
		else if (node->word.compare(word1) < 0)
		{
			node->right = insert(node->right, word1, docnum);
			if (getBalFac(node) == -2)
			{
				if (node->right->word.compare(word1) < 0)
					rwrChild(node);
				else doublerwlChild(node);
			}
		}

		else if (node->word.compare(word1) > 0)
		{
			node->left = insert(node->left, word1, docnum);
			if (getBalFac(node) == 2)
			{
				if (node->left->word.compare(word1) > 0) rwlChild(node);
				else doublerwrChild(node);
			}
		}
	}

	node->height = Height(node);

	return node;
}


int AVLTree::minlength(string a, string b)
{
   int len=0;
   if (a.size()==b.size())
   {
      len = b.size();
   }
   else if (a.size()<b.size())
      len = a.size();
   else
      len = b.size();

   return len;
}


WordNode* AVLTree::remove(WordNode*& node, string word1)
{
	WordNode* tempnode;

	if (node == NULL) return 0;
	else if (node->word.compare(word1) < 0)
	{
		node->right = remove(node->right, word1);
		if (getBalFac(node) == 2)
		{
			if (getBalFac(node->left) >= 0) rwlChild(node);
			else doublerwrChild(node);
		}
	}

	else if (node->word.compare(word1) > 0)
	{
		node->left = remove(node->left, word1);
		if (getBalFac(node) == -2)
		{
			if (getBalFac(node->left) >= 0) rwrChild(node);
			else doublerwlChild(node);
		}
	}

	else
	{
		if (node->right != NULL)
		{
			tempnode = node->right;
			while (tempnode->left != NULL) tempnode = tempnode->left;
			node->word = tempnode->word;
			node->right = remove(node->right, tempnode->word);
			if (getBalFac(node) == 2)
			{
				if (getBalFac(node->left) >= 0) rwlChild(node);
				else doublerwrChild(node);
			}
		}
		else return node->left;
	}

	node->height = Height(node);
	return node;
}

void AVLTree::removeLeaves(WordNode*& node)
{
	if (node != NULL)
	{
		removeLeaves(node->left);
		removeLeaves(node->right);
		delete node;
	}
	node = NULL;
}

void AVLTree::addInfo(vector<wordInfo> &info, int docnum)
{
	bool found = false;
   int len = info.size();
	for (int i = 0; i < len; i++)
	{
		if (info[i].docNum == docnum)
		{
			found = true;
			info[i].freq++;
		}
	}
	if (!found)
	{
		wordInfo winfo(docnum);
		info.push_back(winfo);
	}
}

void AVLTree::rwrChild(WordNode*& node)
{
	WordNode* tempnode = node->right;
	node->right = tempnode->left;
	tempnode->left = node;
	node->height = Height(node);
	tempnode->height = Height(node);
	node = tempnode;
}

void AVLTree::rwlChild(WordNode*& node)
{
	WordNode* tempnode;
	tempnode = node->left;
	node->left = tempnode->right;
	tempnode->right = node;
	node->height = Height(node);
	tempnode->height = Height(node);
	node = tempnode;
}

void AVLTree::doublerwlChild(WordNode*& node)
{
	rwlChild(node->right);
	rwrChild(node);
}

void AVLTree::doublerwrChild(WordNode*& node)
{
	rwrChild(node->left);
	rwlChild(node);
}

int AVLTree::Height(WordNode* node)
{
	int HLeft, HRight;

	if (node == NULL) return 0;

	if (node->left == NULL) HLeft = 0;
	else HLeft = 1 + node->left->height;

	if (node->right == NULL) HRight = 0;
	else HRight = 1 + node->right->height;

	if (HLeft > HRight) return HLeft;
	else return HRight;

}

int AVLTree::getBalFac(WordNode* node)
{
	int HLeft, HRight;

	if (node == NULL) return 0;

	if (node->left == NULL) HLeft = 0;
	else HLeft = 1 + node->left->height;

	if (node->right == NULL) HRight = 0;

	else HRight = 1 + node->right->height;

	return HLeft - HRight;
}

// The HashTable print function calls in inorder AVL print function
void HashTable::PrintTable()
{
	for (int i = 0; i < TABLE_SIZE; i++)
	{
		if (WordTable[i].root != NULL)
		{
			cout << i << " ";
			WordTable[i].inorderPrint(WordTable[i].root);
		}
	}

}

//performs inorder print 
void AVLTree::inorderPrint(WordNode* node)
{
	if (node == NULL) return;
	inorderPrint(node->left);
	cout << node->word <<", nfo.size="; 
   cout<<node->nfo.size();
   cout<<endl;
	inorderPrint(node->right);
}


// The HashTable count function calls the AVL count function for each non-NULL node and adds them up
void HashTable::countWords()
{
	arrayCount = 0;
	wordCount = 0;
	maxBucket = 0;
	int tempMax = 0;
	for (int i = 0; i < TABLE_SIZE; i++)
	{
		if (WordTable[i].root != NULL)
		{
			arrayCount++;
			tempMax = WordTable[i].nodeCount(WordTable[i].root);
			wordCount += tempMax;
		}
		if (tempMax > maxBucket) maxBucket = tempMax;
	}

	collisions = wordCount - arrayCount;
	maxHeight = (int)(log(maxBucket)/log(2) + 0.5);
}

//return nodeCount
int AVLTree::nodeCount(WordNode* node)
{
	if (node == NULL) return 0;

	else
	{
		if (node->left == NULL && node->right == NULL) return 1;

		else return nodeCount(node->left) + nodeCount(node->right) + 1;
	}
}


//Recursive search function: main search function
WordNode* HashTable::SearchWords2(WordNode*& node, string word)
{

   if (node == NULL)
	{
      return NULL;
	}
   else if(node->word==word)
   {
      return node;
   }

   WordNode* returnnode;

   if (node->left!=NULL && node->word.compare(word) > 0)
   {
      returnnode = SearchWords2(node->left, word); 
   }
   else if (node->right!=NULL && node->word.compare(word) < 0)
   {
      returnnode = SearchWords2(node->right, word);
   }
   else
      returnnode = NULL;

   return returnnode;
}

//Recursive function used for incomplete words, ie "spe*" like searches
void HashTable::IncompleteSearchWords(WordNode*& node, string word)
{
   if (node == NULL)
      return;
   if (node->left!=NULL)
      IncompleteSearchWords(node->left, word);
   if (node->right!=NULL)
      IncompleteSearchWords(node->right, word);

   for (int i=word.size(); i>0; --i)
   {
      if(node->word.substr(0,i)==word)
      {
         int len=node->nfo.size();
         cout<<node->word<<":\n";
         for (int i = 0; i < len; i++)
         {
            cout<<"   Doc "<<node->nfo[i].docNum<<": "<<node->nfo[i].freq<<" time";
            if (node->nfo[i].freq>1)
               cout<<"s";
            cout<<endl;
         }
         return;
      }
   }

   return;
}

void HashTable::test()
{
   string word=" ";
   int value=0, len=0;
   cout << "Enter the word you wish to search for: ";
	cin >> word;
   if (word == "-1")
   {
      cout<<"Have a good day!\n";
      return;
   }
   transform(word.begin(), word.end(), word.begin(), ::tolower);
   value=HashVal(word);
   cout<<"value = "<<value<<endl;
   test();
}

//And or OR search function
void HashTable::AndOrSearch(string word)
{
   vector<string> words;
   int find=0, value=0;
   bool done=false, or=false, and=false;
   WordNode* node=NULL;

   //parses search string into a vector
   while(!done)
   {
      find = word.find(" ");
      words.push_back(word.substr(0, find));
      if (word.find(" ")!=-1)
          word = word.substr(find+1, word.length()-1);
      else
         done=true;
   }

   int size = words.size();
   for (int i=0; i<size; ++i)
   {
      if (words[i]=="OR")
         or=true;
      if (words[i]=="AND")
         and=true;
   }
   
   //if OR it does regular seaches all words in word vector
   if (or){
      cout<<"------------------------------------"<<endl;
      for (int i=0; i<size;++i){
         if (words[i]!="OR")
         {
            word = words[i];
            transform(word.begin(), word.end(), word.begin(), ::tolower);
            value = HashVal(word);     
            node  = SearchWords2(WordTable[value].root, word);
            cout<<word<<":\n";
            Report(node);
            cout<<"------------------------------------"<<endl;
         }
      }
   }
   
   //if AND search does regular search and adds 1 to docCount array. 
   if (and){
      int docCount[51];
      int numWords=0;
      fill_n(docCount, 51, 0);
      cout<<"------------------------------------"<<endl;
      for (int i=0; i<size;++i){
         if (words[i]!="AND")
         {
            word = words[i];
            transform(word.begin(), word.end(), word.begin(), ::tolower);
            value = HashVal(word);     
            node  = SearchWords2(WordTable[value].root, word);
            if (node!=NULL)
            {
               int nfosize = node->nfo.size();
               for (int j=0; j<nfosize; ++j)
               {
                  docCount[node->nfo[j].docNum]+=1;
               }             
            }
            numWords+=1;
         }
      }
      if (numWords==0) return;
 
      //If docCount element has the value of the number of words then the search words were found
      bool found=false;
      for (int i=0; i<52;++i){
         if (docCount[i]>=numWords)
         {
            if (found==false)
            {
               found = true;
               for (int j=0; j<size; ++j)
               {
                  if (j==0 && words[j]!="AND")
                     cout<<words[j];
                  else if (words[j]!="AND")
                     cout<<" and "<<words[j];
               }
               cout<<" are found in"<<endl;
            }
            cout<<"doc "<<i<<endl;
         }
      }
      if (!found)
         cout<<"All the words were not found in the same document\n";
      cout<<"------------------------------------"<<endl;
   }

   return;
}

//Gets search string from user and performs appropriate search
void HashTable::SearchWords()
{
	string word=" ";
   int value=0, len=0;
	bool found = false;   

	cout << "Enter the word(s) to search for (0 to exit): ";
	getline(cin, word);
   if (word == "0")
   {
      cout<<"Have a good day!\n";
      return;
   }
   len = word.size();

   //checks to see if word and AND or OR and '-'
   if (word.find("-")<100)
   {
      int dash = word.find("-");
      string addword = word.substr(dash+1);
      addword = word.substr(0, word.find("-"))+" AND " + addword;
      word = addword;
   }

   if (word.find("AND")<100 || word.find("OR")<100)
   {
      AndOrSearch(word);
      SearchWords();
      return;
   }

	transform(word.begin(), word.end(), word.begin(), ::tolower);
   value=HashVal(word);

   //checks for the '*' character to see if the word is an incomplete search
   if (word[word.length()-1]=='*')
   {
      word = word.substr(0,word.length()-1); 
      if (word.size()==1)
      {
         for (int j=0; j<26*26; ++j)
         {
            IncompleteSearchWords(WordTable[value+j].root, word);
         }
      }
      if (word.size()==2)
      {
         for (int j=0; j<26; ++j)
         {
            IncompleteSearchWords(WordTable[value+j].root, word);
         }
      }
      else
         IncompleteSearchWords(WordTable[value].root, word);
     SearchWords();
      return;
   }
   
   //If not incomplete or AND, OR search function does regular search
   WordNode* node = SearchWords2(WordTable[value].root, word);
   Report(node);
   cout<<"------------------------------------"<<endl;

   SearchWords();
   return;
}

void HashTable::Report(WordNode* node)
{
   int max=0, prevmax=10000;
   if (node==NULL)
      cout<<"Word not found\n";
   else
   {
      int nfosize = node->nfo.size();
      for (int i = 0; i < nfosize; i++)
	   {
         max=0;
         for (int j=0; j< nfosize; j++)
         {
            int nfofreq = node->nfo[j].freq;
            if (nfofreq>max && nfofreq<prevmax)
               max = node->nfo[j].freq;
         }
         prevmax = max;
         for (int j=0; j< nfosize; j++)
         {
            int nfofreq = node->nfo[j].freq;
            if (max==nfofreq)
            {
               cout<<"Doc "<<node->nfo[j].docNum<<": "<<node->nfo[j].freq<<" time";
               if (node->nfo[j].freq>1)
                  cout<<"s";
               cout<<endl;
            }
         }
      }
   }
   return;
}

//checks to see if the word being imported from the documents is a stop word
bool HashTable::isStopWord(string word)
{
   int len = stopwords.size();
   for (int i=0; i<len; ++i)
   {
      if (stopwords[i]==word)
         return true;
      if (stopwords[i]==word.substr(0, word.find(" ")))
         return true;
   }
   return false;
}

//imports stopwards to stopwords vector
void HashTable::importStopWords()
{
   ifstream file;
   string word;
   file.open("stopwords.txt");
   if (!file.is_open()){
      cout<<"ERROR OPENING stopwords.txt\n";
      return;
   }
   while(getline(file, word))
   {
      stopwords.push_back(word.substr(0, word.find(' ')));
   }
}
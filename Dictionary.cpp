// Dictionary.cpp

// This is a correct but horribly inefficient implementation of Dictionary
// functionality.  Your job is to change DictionaryImpl to a more efficient
// correct implementation.  You can change it any way you like, subject to
// restrictions in the spec (e.g., that the only C++ library container you
// are allowed to use are vector, list, stack, and queue (and string); if you
// want anything fancier, implement it yourself).

#include "Dictionary.h"
#include <string>
#include <list>
#include <cctype>
#include <utility>  // for swap
#include <algorithm> // for sort
#include <functional> // for hashing
using namespace std;

void removeNonLetters(string& s);
void generateNextPermutation(string& permutation);

  // This class does the real work of the implementation.

class DictionaryImpl
{
  public:
    DictionaryImpl(int maxBuckets);
    ~DictionaryImpl() {}
    void insert(string word);
    void lookup(string letters, void callback(string)) const;
  private:
    int m_maxBuckets; // # of buckets
    struct Bucket
    {
        list<string> originalWord; // the word itself to check the anagram
        list<string> sortedKey; // vector of possible permutation of sorted words
    };
    
    vector<Bucket> wordMap; // the hash table with allocated Buckets
    
};

DictionaryImpl::DictionaryImpl(int maxBuckets) : m_maxBuckets(maxBuckets) // initialize # of buckets
{
    wordMap.resize(m_maxBuckets); // set the size of the vector with the # of buckets
}

void DictionaryImpl::insert(string word)
{
    removeNonLetters(word);
    
    if (!word.empty())
    {
        string original = word; // temporarily store word
        sort(word.begin(), word.end()); // sort the word alphabetically (since every anagram will have the same sorted word)
        unsigned int keyValue = hash<string>()(word) % m_maxBuckets; // return a key value to reference later in vector of buckets
        wordMap[keyValue].originalWord.push_back(original); // push back the original word so we can use it later
        wordMap[keyValue].sortedKey.push_back(word); // push back the sorted word to reference later
    }
    else
        return;
}

void DictionaryImpl::lookup(string letters, void callback(string)) const
{
    if (callback == nullptr)
        return;

    removeNonLetters(letters);
    if (letters.empty())
        return;

    sort(letters.begin(), letters.end()); // sort the string that we are looking up (so we can match it later)
    unsigned int keyValue = hash<string>()(letters) % m_maxBuckets; // return a key value to reference in vector of buckets
    list<string>::const_iterator sorted = wordMap[keyValue].sortedKey.begin(); // iterator that points to the first value of the sorted list
    list<string>::const_iterator original = wordMap[keyValue].originalWord.begin(); // iterator that points to the first value of the original words list

    for (int i = 0; i < wordMap[keyValue].originalWord.size(); i++, sorted++, original++)
    {
        // since there may be collisions, if the sorted verison of the word we are looking up matches any of the sorted words in that same bucket, callback on the original words which are the anagrams
        if (letters == *sorted)
            callback(*original);
    }
}

void removeNonLetters(string& s)
{
    string::iterator to = s.begin();
    for (string::const_iterator from = s.begin(); from != s.end(); from++)
    {
        if (isalpha(*from))
        {
            *to = tolower(*from);
            to++;
        }
    }
    s.erase(to, s.end());  // chop everything off from "to" to end.
} 

  // Each successive call to this function generates the next permutation of the
  // characters in the parameter.  For example,
  //    string s = "eel";
  //    generateNextPermutation(s);  // now s == "ele"
  //    generateNextPermutation(s);  // now s == "lee"
  //    generateNextPermutation(s);  // now s == "eel"
  // You don't have to understand this function's implementation.
void generateNextPermutation(string& permutation)
{
    string::iterator last = permutation.end() - 1;
    string::iterator p;

    for (p = last; p != permutation.begin()  &&  *p <= *(p-1); p--)
        ;
    if (p != permutation.begin())
    {
        string::iterator q;
        for (q = p+1; q <= last  &&  *q > *(p-1); q++)
            ;
        swap(*(p-1), *(q-1));
    }
    for ( ; p < last; p++, last--)
        swap(*p, *last);
}

//******************** Dictionary functions ******************************

// These functions simply delegate to DictionaryImpl's functions
// You probably don't want to change any of this code

Dictionary::Dictionary(int maxBuckets)
{
    m_impl = new DictionaryImpl(maxBuckets);
}

Dictionary::~Dictionary()
{
    delete m_impl;
}

void Dictionary::insert(string word)
{
    m_impl->insert(word);
}

void Dictionary::lookup(string letters, void callback(string)) const
{
    m_impl->lookup(letters,callback);
}

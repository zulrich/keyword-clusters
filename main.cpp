//
//  main.cpp
//  Clustering
//
//  Created by Zack Ulrich on 7/30/13.
//  Copyright (c) 2013 Zack Ulrich. All rights reserved.
//

#include <iostream>
#include <string>
#include <fstream>
#include <stdlib.h>
#include <vector>
#include <algorithm>
#include <iterator>
#include <sstream>


using namespace std;

void printClusters(std::vector < vector < string> > clusters)
{
    ofstream myfile;
    myfile.open ("result.txt");
    
    for (std::vector<vector<string>>::iterator it3 = clusters.begin(); it3 != clusters.end(); ++it3)
    {
        myfile << "----------------------- \n";
        
        for (std::vector<string>::iterator it = it3->begin(); it != it3->end(); ++it)
        {
            myfile << *it << endl;
        }
    }
    
    myfile.close();
}



bool comparePairs(vector<string> pair1, vector<string> pair2)
{
    //compares the vectors of pairs to see if same
    
    if(pair1.size() != pair2.size())
        return false;
    
    for (unsigned int i = 0; i < pair1.size(); i++)
    {
        string p1, p2;
        p1 = pair1.at(i);
        p2 = pair2.at(i);
        
        if(p1.compare(p2) != 0)
            return false;
    }
    
    return true;
    
}

vector<string> letterPairs(string str)
{
    //creates an array of letter pairs in the string
    unsigned long numPairs = str.length() - 1;
    vector<string> pairs = *new vector<string>;
    
    for (unsigned int i = 0; i<numPairs; i++) {
        pairs.push_back(str.substr(i,i+2));
        
    }
    return pairs;
    
}

vector<vector <string> > wordLetterPairs(string str)
{
    vector< vector <string> > allPairs = *new vector< vector <string> >;
    // Tokenize the string and put the tokens/words into an array
    
    
    //breaks line into words removes whitespace
    istringstream iss(str);
    vector<string> words;
    copy(istream_iterator<string>(iss),
         istream_iterator<string>(),
         back_inserter<vector<string> >(words));
    
    // For each word
    for (unsigned int w = 0; w < words.size(); w++) {
        // Find the pairs of characters
        vector<string> pairsInWord = letterPairs(words[w]);
        for (unsigned int p = 0; p < pairsInWord.size(); p++)
        {
            vector<string>temp;
            temp.push_back(pairsInWord.at(p));
            allPairs.push_back(temp);
        }
    }
    return allPairs;
}

double compareStrings(string str1, string str2)
{
    //return similarity between to strings from 0 - 1
    
    //make all strings in uppercase
    
    std::transform(str1.begin(), str1.end(),str1.begin(), ::toupper);
    std::transform(str2.begin(), str2.end(),str2.begin(), ::toupper);
    vector<vector<string> > pairs1 = wordLetterPairs(str1);
    vector<vector<string> > pairs2 = wordLetterPairs(str2);
    int intersection = 0;
    unsigned long unionVal = pairs1.size() + pairs2.size();
    for (unsigned int i = 0; i<pairs1.size(); i++) {
        for(unsigned int j = 0; j<pairs2.size(); j++) {
            if (comparePairs(pairs1.at(i), pairs2.at(j))) //compare wordLetterPairs
            {
                intersection++;
                pairs2.erase(pairs2.begin() + j); //remove the index to avoid duplicate matches
                break;
            }
        }
    }
    return (2.0*intersection)/unionVal;
}

int main(int argc, const char * argv[])
{
    
    string keywordsFilePath = "";
    string testFilePath = "";
    ifstream keywordFile, stopwordFile;
    vector<string> keywordVec, stopwordVec, testVec;
    
    if(argc == 2)
    {
        keywordsFilePath = argv[1];
        keywordFile.open(keywordsFilePath.c_str());
        
    }
    
    string line;
    
    if (keywordFile.is_open())
    {
        while (!keywordFile.eof())
        {
            getline(keywordFile,line);
            
            if (!line.empty()) //don't add blank lines
            {
                keywordVec.push_back(line);
                
            }
            
        }
    }
    
    int count = 0;
    vector<vector<string>> clusters, finalClusters;
    
    
    for(int i = 0; i < keywordVec.size(); i++)
    {
        vector<string> cluster;
        
        string firstVal = keywordVec.at(i);
        
        cluster.push_back(firstVal);
        
        for (int j = keywordVec.size() - 1; j > i; j--)
        {
            
            string secondVal = keywordVec.at(j);
            if (compareStrings(firstVal, secondVal) > .85)
            {
                cluster.push_back(secondVal);
                
            std:vector<string>::iterator it = keywordVec.begin() + j;
                
                keywordVec.erase(it);
                
            }
        }
        cout << "loop " << i << endl;
        clusters.push_back(cluster);
        
    }
    
    cout << "done looping \n";
    
    for (std::vector<vector<string>>::iterator it3 = clusters.begin(); it3 != clusters.end(); ++it3)
    {
        
        if (it3->size() > 30) //35
        {
            //need to split up
            vector<string> top, bottom;
            
            string first = it3->at(0);
            
            //cout << "first split " << first << endl;
            top.push_back(first);
            
            
            for (int i = 1; i < it3->size() - 1; i++)
            {
                
                string second = it3->at(i);
                
                if(compareStrings(first, second) > .9)
                {
                    top.push_back(second);
                }
                
                else
                {
                    bottom.push_back(second);
                }
                
            }
            
            if (top.size() > 0)
            {
                finalClusters.push_back(top);
            }
            
            if (bottom.size() > 0)
            {
                finalClusters.push_back(bottom);
            }
            
        }
        
        else if( it3 ->size() < 5) //25
        {
            // need to add
        }
        
        else
        {
            
            finalClusters.push_back(*it3);
            
        }
        
    }
    
    printClusters(finalClusters);
    
    cout << count << " DONE \n";
    
    return 0;
}



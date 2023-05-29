#pragma once
#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <string>
#include <vector>
#include <utility>
#include <QString>
#include "global.h"

using namespace std;

// 词法分析
class Lexer
{
public:
     Lexer();
     void run(string  str) ;

     bool isSeparater(char ch);
     bool isOperator(char ch);
     bool isReserved(string ch);
     bool isBlank(char ch);
     bool isLetter(char ch);
     bool isDigit(char ch);


     void lexicalAnalyse(FILE* fp);
     WORD getReservedNum(string s);
     string getString(int lexNum);
     void push_tokenList_vec(int line, string s, LexType lexstr);
     void Count_();

};



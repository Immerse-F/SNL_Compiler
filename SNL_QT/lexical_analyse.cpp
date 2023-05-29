#pragma once

#include "lexical_analyse.h"

// 保留字查找表
WORD reservedWords[21] =
{
    {"program",LexType::PROGRAM},
    {"type",LexType::TYPE},
    {"var",LexType::VAR},
    {"procedure",LexType::PROCEDURE},
    {"begin",LexType::BEGIN},
    {"end",LexType::END1},
    {"array",LexType::ARRAY},
    {"of",LexType::OF},
    {"record",LexType::RECORD},
    {"if",LexType::IF},
    {"then",LexType::THEN},
    {"else",LexType::ELSE},
    {"fi",LexType::FI},
    {"while",LexType::WHILE},
    {"do",LexType::DO},
    {"endwh",LexType::ENDWH},
    {"read",LexType::READ},
    {"write",LexType::WRITE},
    {"return",LexType::RETURN1},
    {"integer",LexType::INTEGER},
    {"char",LexType::CHAR1}
};

Lexer::Lexer(){}

void Lexer::run(string str){
    str="D:/SNL/edit.txt";
    //cout<<"112323"<<endl;
    //cout<<str<<endl;'D:/SNL/source.txt'
    for (int i = 0; i < 1024; i++)
    {
        tokenList[i].index = i;
    }
    FILE* fp;
    fp = fopen(str.c_str(), "r");
    lexicalAnalyse(fp);
    Count_();

}

bool Lexer::isDigit(char ch)
{
    return (ch >= '0' && ch <= '9');
}

bool Lexer::isLetter(char ch)
{
    return ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'));
}

bool Lexer::isBlank(char ch)
{
    return (ch == ' ' || ch == '\n' || ch == '\t' || ch == '\r');
}

//保留字
bool Lexer::isReserved(string ch)
{
    for (int i = 0; i < 21; i++)
        if (ch == reservedWords[i].str)
            return true;
    return false;
}

WORD Lexer::getReservedNum(string s)
{
    for (int i = 0; i < 21; i++)
    {
        if (reservedWords[i].str == s)
            return reservedWords[i];
    }
}


bool Lexer::isOperator(char ch)
{
    return (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '<' || ch == '=');

}

bool Lexer::isSeparater(char ch) {
    switch (ch) {
        case ';':
        case ',':
        case '{':
        case '}':
        case '[':
        case ']':
        case '(':
        case ')':
        case '.':
        case '\'':
        case ':':
            return true;
        default:
            return false;
    }
}

void Lexer::Count_()
{

    int i = 0;
    while (tokenList[i].wd.tok != LexType::ENDFILE)
    {
        i++;
    }
    INDEX = i;
}


string Lexer::getString(int lexNum) {
    switch (lexNum) {
        case 0:
            return "ENDFILE";
        case 1:
            return "ERROR";
        case 2:
            return "PROGRAM";
        case 3:
            return "PROCEDURE";
        case 4:
            return "TYPE";
        case 5:
            return "VAR";
        case 6:
            return "IF";
        case 7:
            return "THEN";
        case 8:
            return "ELSE";
        case 9:
            return "FI";
        case 10:
            return "WHILE";
        case 11:
            return "DO";
        case 12:
            return "ENDWH";
        case 13:
            return "BEGIN";
        case 14:
            return "END";
        case 15:
            return "READ";
        case 16:
            return "WRITE";
        case 17:
            return "ARRAY";
        case 18:
            return "OF";
        case 19:
            return "RECORD";
        case 20:
            return "RETURN";
        case 21:
            return "INTEGER";
        case 22:
            return "CHAR";
        case 23:
            return "ID";
        case 24:
            return "INTC";
        case 25:
            return "CHARC";
        case 26:
            return "ASSIGN";
        case 27:
            return "EQ";
        case 28:
            return "LT";
        case 29:
            return "PLUS";
        case 30:
            return "MINUS";
        case 31:
            return "TIMES";
        case 32:
            return "OVER";
        case 33:
            return "LPAREN";
        case 34:
            return "RPAREN";
        case 35:
            return "DOT";
        case 36:
            return "COLON";
        case 37:
            return "SEMI";
        case 38:
            return "COMMA";
        case 39:
            return "LMIDPAREN";
        case 40:
            return "RMIDPAREN";
        case 41:
            return "UNDERANGE";
        default:
            return "";
    }
}


void Lexer::push_tokenList_vec(int line, string s, LexType lexstr) {
    struct Token t ;
    t.line= line;
    t.wd=make_pair(s,lexstr);
    tokenList_vec.push_back(t);
}

void Lexer::lexicalAnalyse(FILE* fp)
{
    int lineNum = 1;

    int index = 0;

    char ch = fgetc(fp);

    string record = "";

    while (ch != EOF)
    {

        if (isLetter(ch))
        {
            //INID
            record = ch;
            ch = fgetc(fp);
            while (isLetter(ch) || isDigit(ch))
            {
                record = record + ch;
                ch = fgetc(fp);
            }

            if (isReserved(record))
            {
                tokenList[index].line = lineNum;
                tokenList[index].wd.str = getReservedNum(record).str;
                tokenList[index].wd.tok = getReservedNum(record).tok;
                push_tokenList_vec(lineNum, getReservedNum(record).str, getReservedNum(record).tok);
            }
            else
            {
                tokenList[index].line = lineNum;
                tokenList[index].wd.str = record;
                tokenList[index].wd.tok = LexType::ID;
                push_tokenList_vec(lineNum, record, LexType::ID);
            }
            index++;
        }

        else if (isDigit(ch))
        {
            //INNUM
            record = ch;
            ch = fgetc(fp);

            while (isDigit(ch))
            {
                record += ch;
                ch = fgetc(fp);
            }

            tokenList[index].wd.tok = LexType::INTC;
            push_tokenList_vec(lineNum, record, LexType::INTC);

            tokenList[index].line = lineNum;
            tokenList[index].wd.str = record;
            index++;
        }

        else if (isSeparater(ch))
        {
            record = "";
            //INCOMMENT
            if (ch == '{')
            {
                while (ch != '}')
                {
                    ch = fgetc(fp);
                    if (ch == '\n')
                        lineNum += 1;
                }
                ch = fgetc(fp);
            }

            else if (ch == '.')
            {
                record += ch;
                if ((ch = fgetc(fp)) == '.')
                {
                    record += ch;
                    tokenList[index].line = lineNum;
                    tokenList[index].wd.str = record;
                    tokenList[index].wd.tok = LexType::UNDERANGE;
                    push_tokenList_vec(lineNum, record, LexType::UNDERANGE);
                    index++;
                    ch = fgetc(fp);
                }
                else // '.'
                {
                    tokenList[index].line = lineNum;
                    tokenList[index].wd.str = record;
                    tokenList[index].wd.tok = LexType::DOT;
                    push_tokenList_vec(lineNum, record, LexType::DOT);
                    index++;

                }
            }


            else if (ch == '\'')
            {

                //INCHAR

                tokenList[index].line = lineNum;
                string temp = "";
                temp = temp + ch;
                tokenList[index].wd.tok = LexType::CHARC;
                record += (ch = fgetc(fp));
                int flag_c = 0;
                while (ch != EOF&&(ch = fgetc(fp)) != '\'')
                {
                    record += ch;
                    flag_c++;
                }
                if(flag_c){
                    string temp_str = to_string(lineNum) + "\t行" + record + "非法字符";
                    errorList.push_back(QString::fromStdString(temp_str));
                    push_tokenList_vec(lineNum, record, LexType::ERROR);
                }
                else{
                    push_tokenList_vec(lineNum, record, LexType::CHARC);
                }
                tokenList[index].wd.str = record;
                index++;
                ch = fgetc(fp);

            }

            else if (ch == ':')
            {
                record += ch;
                if ((ch = fgetc(fp)) == '=')
                {
                    //INASSIGN
                    record += ch;
                    tokenList[index].line = lineNum;
                    tokenList[index].wd.str = record;
                    tokenList[index].wd.tok = LexType::ASSIGN;
                    push_tokenList_vec(lineNum, record, LexType::ASSIGN);
                    index++;
                    ch = fgetc(fp);
                }
                else
                {
                    tokenList[index].line = lineNum;
                    tokenList[index].wd.str = record;
                    tokenList[index].wd.tok = LexType::COLON;
                    push_tokenList_vec(lineNum, record, LexType::COLON);
                    index++;
                }
            }
            else if(ch == '}'){


                tokenList[index].line = lineNum;
                string temp = "";
                temp = temp + ch;
                tokenList[index].wd.tok = LexType::ERROR;
                record =ch;


                string temp_str = to_string(lineNum) + "\t行" + record + "非法字符";
                errorList.push_back(QString::fromStdString(temp_str));
                push_tokenList_vec(lineNum, record, LexType::ERROR);
                tokenList[index].wd.str = record;
                index++;
                ch = fgetc(fp);

            }
            //
            else
            {
                tokenList[index].line = lineNum;
                string temp = "";
                temp = temp + ch;
                tokenList[index].wd.str = temp;

                if (temp == "(") {
                    tokenList[index].wd.tok = LexType::LPAREN;
                    push_tokenList_vec(lineNum, temp, LexType::LPAREN);
                }
                else if (temp == ")") {
                    tokenList[index].wd.tok = LexType::RPAREN;
                    push_tokenList_vec(lineNum, temp, LexType::RPAREN);
                }
                else if (temp == "[") {
                    tokenList[index].wd.tok = LexType::LMIDPAREN;
                    push_tokenList_vec(lineNum, temp, LexType::LMIDPAREN);
                }
                else if (temp == "]") {
                    tokenList[index].wd.tok = LexType::RMIDPAREN;
                    push_tokenList_vec(lineNum, temp, LexType::RMIDPAREN);
                }
                else if (temp == ";") {
                    tokenList[index].wd.tok = LexType::SEMI;
                    push_tokenList_vec(lineNum, temp, LexType::SEMI);
                }
                else if (temp == ",") {
                    tokenList[index].wd.tok = LexType::COMMA;
                    push_tokenList_vec(lineNum, temp, LexType::COMMA);
                }
                index++;
                ch = fgetc(fp);
            }
        }

        else if (isOperator(ch))
        {
            string temp = "";
            temp = ch;
            ch = fgetc(fp);

            if (temp == "+") {
                tokenList[index].wd.tok = LexType::PLUS;
                push_tokenList_vec(lineNum, temp, LexType::PLUS);
            }
            else if (temp == "-") {
                tokenList[index].wd.tok = LexType::MINUS;
                push_tokenList_vec(lineNum, temp, LexType::MINUS);
            }
            else if (temp == "*") {
                tokenList[index].wd.tok = LexType::TIMES;
                push_tokenList_vec(lineNum, temp, LexType::TIMES);
            }
            else if (temp == "/") {
                tokenList[index].wd.tok = LexType::OVER;
                push_tokenList_vec(lineNum, temp, LexType::OVER);
            }

            else if (temp == "<") {
                tokenList[index].wd.tok = LexType::LT;
                push_tokenList_vec(lineNum, temp, LexType::LT);
            }
            else if (temp == "=") {
                tokenList[index].wd.tok = LexType::EQ;
                push_tokenList_vec(lineNum, temp, LexType::EQ);
            }

            tokenList[index].wd.str = temp;
            tokenList[index].line = lineNum;


            index++;

        }


        else if (isBlank(ch))
        {
            if (ch == '\n') lineNum += 1;//行号+1
            ch = fgetc(fp);
        }


        else
        {

            int a =1 ;
            string temp_str = to_string(lineNum) + "\t行" + ch + "未知标识符";
            errorList.push_back(QString::fromStdString(temp_str));
            ch = fgetc(fp);
        }
    }
    //DONE

    tokenList[index].line = lineNum;
    tokenList[index].wd.str = ch;
    tokenList[index].wd.tok = LexType::ENDFILE;
    string tempStr = "" + ch;
    push_tokenList_vec(lineNum, tempStr, LexType::ENDFILE);
}




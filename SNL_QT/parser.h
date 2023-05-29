#pragma once
#include "tree.h"
#include "global.h"
//语法分析器
class Parser
{
public:
    Parser();
    virtual ~Parser() = default;
    virtual SyntaxTree* run(vector<struct Token> tokenList) = 0; //run函数，输入tokenList，得到语法树根节点
};

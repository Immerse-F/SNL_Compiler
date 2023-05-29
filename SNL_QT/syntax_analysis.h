#pragma once
#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <string>
#include <stack>
#include <unordered_map>
#include <QDebug>
#include "global.h"
#include "tree.h"
#include "parser.h"
using namespace std;

class ParserLL1: public Parser
{
public:
    ParserLL1(bool IS_CALL);
    virtual SyntaxTree* run(vector<struct Token> tokenList) override;

private:
    //根据产生式左部，找到First或Follow
    unordered_map<LexType, set<LexType>> First_mp;
    unordered_map<LexType, set<LexType>> Follow_mp;
    //产生式编号[1,104] index[0,103]
    vector<pair<LexType, vector<LexType>>> Production;
    //通过左部找到编号
    unordered_map<LexType, vector<int>> mp;
    //通过右部(的非终极符)找到编号
    unordered_map<LexType, vector<int>> reverse_mp;
    map<pair<LexType, LexType>, int> table;
    stack<LexType> symbal_stack; //符号栈
    stack<TreeNode**> syntaxtree_stack; //语法树栈
    stack<TreeNode*> op_stack;
    stack<TreeNode*> num_stack;

    TreeNode* current_treenode; //currentP
    DecKind* temp;
    TreeNode* saveP;
    bool getExpResult = true;
    int expflag = 0;
    int getExpResult2 = false;
public:
    static int nowLine;
    static int head;
    static TreeNode* newSpecNode(NodeKind kind);
    static TreeNode* newStmtNode(StmtKind kind);
    static TreeNode* newExpNode(ExpKind kind);
    //void InitLL1Table();//初始化全局变量LL1Table
    int getPriosity(LexType op);
    void process(int id);//使用tokenList全局变量
    //104产生式
    void initProduction();
    //mp和reverse_mp映射维护
    void get_mp();

    void First(LexType t, set<LexType>& s);
    void getFirst();

    void Follow(LexType t, set<LexType>& s, int cnt);
    void getFollow();

    void getPredict();
};

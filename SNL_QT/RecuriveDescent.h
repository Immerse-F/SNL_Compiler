#pragma once
#include <QDebug>
#include <QMessageBox>
#include "parser.h"
#include "tree.h"
#include "global.h"

//递归下降语法分析器
class ParserRecursiveDescent : public Parser
{
private:
    vector<struct Token> tokenList;
    int cur; //tokenList的下标
    string tempName; //分析函数中使用的存储变量名的临时变量

public:
    ParserRecursiveDescent();
    virtual SyntaxTree* run(vector<struct Token> tokenList) override;

private:
    bool match(LexType lexType);

    //55个递归下降程序
    TreeNode* program();
    TreeNode* programHead();
    void programName(TreeNode* t);
    TreeNode* declarePart();
    TreeNode* typeDecpart();
    TreeNode* typeDec();
    TreeNode* typeDecList();
    TreeNode* typeDecMore();
    void typeID(TreeNode* t);
    void typeDef(TreeNode* t);
    void baseType(TreeNode* t);
    void structureType(TreeNode* t);
    void arrayType(TreeNode* t);
    void low(TreeNode* t);
    void top(TreeNode* t);
    void recType(TreeNode* t);
    TreeNode* fieldDecList();
    TreeNode* fieldDecMore();
    void idList(TreeNode* t);
    void idMore(TreeNode* t);
    TreeNode* varDecpart();
    TreeNode* varDec();
    TreeNode* varDecList();
    TreeNode* varDecMore();
    void varIDList(TreeNode* t);
    void varIDMore(TreeNode* t);
    TreeNode* procDecpart();
    TreeNode* procDec();
    TreeNode* procDecMore();
    void procName(TreeNode* t);
    void paramList(TreeNode* t);
    TreeNode* paramDecList();
    TreeNode* paramMore();
    TreeNode* param();
    void formList(TreeNode* t);
    void fidMore(TreeNode* t);
    TreeNode* procDecPart();
    TreeNode* procBody();
    TreeNode* programBody();
    TreeNode* stmList();
    TreeNode* stmMore();
    TreeNode* stm();
    TreeNode* assCall();
    TreeNode* assignmentRest();
    TreeNode* conditionalStm();
    TreeNode* loopStm();
    TreeNode* inputStm();
    void inVar(TreeNode* t);
    TreeNode* outputStm();
    TreeNode* returnStm();
    TreeNode* callStmRest();
    TreeNode* actParamList();
    TreeNode* actParamMore();
    TreeNode* relExp();
    TreeNode* exp();
    TreeNode* term();
    TreeNode* factor();
    TreeNode* variable();
    void variMore(TreeNode* t);
    TreeNode* fieldVar();
    void fieldVarMore(TreeNode* t);
    void cmpOp(TreeNode* t);
    void addOp(TreeNode* t);
    void multOp(TreeNode* t);

    void syntaxError(string error);
};


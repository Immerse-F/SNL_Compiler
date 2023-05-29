#pragma once
#include"semDS.h"

//为了在该cpp文件访问ui
#include "mainwindow.h"
#include"ui_mainwindow.h"

struct typeIR;
class TreeNode;
class Symtable;
enum class DecKind;
enum class SEM_ERROR;

//
class MianWindow;


class SemAnalyzer {
public://类型的内部表示
    typeIR* intptr;
    typeIR* charptr;
    typeIR* boolptr;
public://
    Symtable* root;//符号表管理类

//
public:
    SemAnalyzer ();
    SemAnalyzer(Ui::MainWindow * parent);
    Ui::MainWindow * mainwidow;
   void SetUI();

public:
    void init();
    symbtable* Analyze(TreeNode*);
public:
    typeIR* nameType(TreeNode*);
    typeIR* arrayType(TreeNode*);
    typeIR* recordType(TreeNode*);
    typeIR* TypeProcess(TreeNode*, DecKind);
public:
    void TypeDecPart(TreeNode*);
    void VarDecList(TreeNode*);
    void ProcDecPart(TreeNode*);
public:
    symbtable* HeadProcess(TreeNode*);
    ParamTable* ParaDecList(TreeNode*);
public:
    void Body(TreeNode*);
    void statement(TreeNode*);
    typeIR* Expr(TreeNode*, AccessKind*);
    typeIR* arrayVar(TreeNode*);
    typeIR* recordVar(TreeNode*);
public:
    void returnstatement(TreeNode*);
    void assignstatement(TreeNode*);
    void callstatement(TreeNode*);
    void ifstatment(TreeNode*);
    void whilestatement(TreeNode*);
    void readstatement(TreeNode*);
    void writestatement(TreeNode*);

};

//错误标志,语义分析用到
extern bool Lexical_Error;
extern bool Grammar_Error;
extern bool Semantic_Error;
extern string error_info;














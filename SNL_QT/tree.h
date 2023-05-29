#pragma once
#include <QVector>
#include "global.h"
#include "lexical_analyse.h"
#include"semDS.h"

using namespace std;
class TreeNode
{
public:
    TreeNode* child[3]; // 子结点指针
    TreeNode* sibling; // 兄弟结点指针
    int lineno; // 源程序行号
    NodeKind nodekind; // 结点类型
    Kind kind; // 结点的具体类型，为共用体

    int idnum; // 结点中标识符的个数,用于语义分析
    string name[20]; // 结点中标识符的名字

    vector<symbtable*>table;// 指针数组，数组成员是结点中各个标识符在符号表中的入口

    string type_name; // 记录类型名，当结点是声明类型，且类型是由类型标识符表示时有效
    struct
    {
        struct
        {
            int low; // 数组下界
            int up; // 数组上界
            DecKind childtype; // 记录数组的成员类型
        } ArrayAttr; // 数组类型属性
        struct
        {
            ParamType paramt; // 参数类型（值参或变参）
        } ProcAttr; // 过程属性
        struct
        {
            LexType op; // 运算符单词，为单词类型。
             /*
             当语法树结点为"关系运算表达式"对应结点时，取值LT、EQ
             当语法树结点为"加法运算简单表达式"对应结点时，取值PLUS、MINUS
             当语法树结点为"乘法运算项"对应结点时，取值TIMES、OVER
             */
            int val; // 记录语法树结点的数值，当语法树结点为"数字因子"对应的语法树结点时有效，为整数类型
            VarKind varkind;//记录变量的类别
            ExpType type; // 记录语法树结点的检查类型，取值Void, Integer, Boolean, 为类型检查ExpType类型
        } ExpAttr; // 记录表达式的属性
    } attr;

public:
    TreeNode() {}
    TreeNode(NodeKind nodekind, int lineNo);
    ~TreeNode();
};

class SyntaxTree
{
public:
    TreeNode* root;
public:
    SyntaxTree();
    ~SyntaxTree();
    void setRoot(TreeNode* node);
    TreeNode* getRoot();
};

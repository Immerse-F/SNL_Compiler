#pragma once
#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <QMap>
#include <QString>
#include<unordered_map>
#define str first //保留字字符串
#define tok second //标号
using namespace std;

//Token种类
enum class LexType
{
    //终极符
    /* 簿记单词符号 */
    ENDFILE, ERROR,
    /*保留字*/
    PROGRAM, PROCEDURE, TYPE, VAR, IF,
    THEN, ELSE, FI, WHILE, DO, ENDWH,
    BEGIN, END1, READ, WRITE, ARRAY, OF,
    RECORD, RETURN1,
    /*类型*/
    INTEGER, CHAR1,
    /*多字符单词符号*/
    ID, INTC, CHARC,
    /*特殊符号*/
    ASSIGN, EQ, LT, PLUS, MINUS,
    TIMES, OVER, LPAREN, RPAREN, DOT,
    COLON, SEMI, COMMA, LMIDPAREN, RMIDPAREN,
    UNDERANGE, DIVIDE,//加了一个DIVIDE

    // 非终极符，递归下降使用
    Program, ProgramHead, ProgramName, DeclarePart,
    TypeDec, TypeDeclaration, TypeDecList, TypeDecMore,
    TypeId, TypeName, BaseType, StructureType,
    ArrayType, Low, Top, RecType,
    FieldDecList, FieldDecMore, IdList, IdMore,
    VarDec, VarDeclaration, VarDecList, VarDecMore,
    VarIdList, VarIdMore, ProcDec, ProcDeclaration,
    ProcDecMore, ProcName, ParamList, ParamDecList,
    ParamMore, Param, FormList, FidMore,
    ProcDecPart, ProcBody, ProgramBody, StmList,
    StmMore, Stm, AssCall, AssignmentRest,
    ConditionalStm, StmL, LoopStm, InputStm,
    InVar, OutputStm, ReturnStm, CallStmRest,
    ActParamList, ActParamMore, RelExp, OtherRelE,
    Exp, OtherTerm, Term, OtherFactor,
    Factor, Variable, VariMore, FieldVar,
    FieldVarMore, CmpOp, AddOp, MultOp,
    Empty, Well
};

typedef enum
{
    Program, ProgramHead, ProgramName, DeclarePart,
    TypeDec, TypeDeclaration, TypeDecList, TypeDecMore,
    TypeId, TypeName, BaseType, StructureType,
    ArrayType, Low, Top, RecType,
    FieldDecList, FieldDecMore, IdList, IdMore,
    VarDec, VarDeclaration, VarDecList, VarDecMore,
    VarIdList, VarIdMore, ProcDec, ProcDeclaration,
    ProcDecMore, ProcName, ParamList, ParamDecList,
    ParamMore, Param, FormList, FidMore,
    ProcDecPart, ProcBody, ProgramBody, StmList,
    StmMore, Stm, AssCall, AssignmentRest,
    ConditionalStm, StmL, LoopStm, InputStm,
    InVar, OutputStm, ReturnStm, CallStmRest,
    ActParamList, ActParamMore, RelExp, OtherRelE,
    Exp, OtherTerm, Term, OtherFactor,
    Factor, Variable, VariMore, FieldVar,
    FieldVarMore, CmpOp, AddOp, MultOp
}NonTerminal; // 非终极符，LL1使用

enum class NodeKind
{
    /* 标志结点 */
    ProK, // 根标志结点
    PheadK, // 程序头标志结点
    TypeK, // 类型声明标志结点
    VarK, // 变量声明标志结点
    ProcDecK, // 函数声明标志结点
    StmLK, // 语句序列标志结点
    /* 具体结点 */
    DecK, // 声明结点
    StmtK, // 语句结点
    ExpK //表达式结点
};


enum class DecKind
{
    ArrayK, // 组类型
    CharK, // 字符类型
    IntegerK, // 整数类型
    RecordK, // 记录类型
    IdK // 类型标识符作为类型
};
enum class StmtKind
{
    IfK, // 判断语句类型
    WhileK, // 循环语句类型
    AssignK, // 赋值语句类型
    ReadK, // 读语句类型
    WriteK, // 写语句类型
    CallK, // 函数调用语句类型
    ReturnK // 返回语句类型
};
enum class ExpKind
{
    OpK, // 操作符类型 a+1
    ConstK, // 常整数类型 4
    VariK // 标识符类型 a
};

union Kind//语法树节点的具体类型
{
    DecKind dec;//声明 如int a=1；
    StmtKind stmt;//语句 如if(a==1)；
    ExpKind exp;//表达式 如a+1
};

enum class ParamType
{
    valparamType, // 表示过程的参数是值参
    varparamType // 表示过程的参数是变参
};
enum class VarKind
{
    IdV, // 变量是标识符变量
    ArrayMembV, // 变量是数组成员变量
    FieldMembV // 变量是域成员变量
};
enum class ExpType
{
    Void,
    Integer,
    Boolean
};


struct Symbtable;

//
typedef pair<string, LexType> WORD;

//extern int LL1Table[104][104]; // LL1表
struct Token {
    int line;
    WORD wd;
    int index = -1;
};

//====语义分析使用====
enum class TypeKind {//类型的种类（在类型的内部表示中使用）
    intTy,
    charTy,
    arrayTy,
    recordTy,
    boolTy
};
enum class IdKind {//标识符种类（在符号表的信息域中使用）
    typeKind,
    varKind,
    procKind,
    arrayNameKind
};
enum class AccessKind {//直接间接
    dir,
    indir
};
//==================

extern Token* currentToken; // 建立语法树时使用,扫描TokenList

extern Token tokenList[1024];// token表

extern int INDEX ; // tokenList_Id

extern vector<QString> errorList;
extern vector<struct Token> tokenList_vec;
//tokenList <==> tokenList_vec是等价的  可以二选一

extern set<LexType> TTSet;//LL1使用
extern set<LexType> NTSet;//LL1使用
extern QMap<NodeKind, QString> nodekindName;//LL1使用
extern QMap<LexType, QString> lexName;
extern QMap<QString, LexType> reservedWord; //
extern QMap<char, LexType> opWords;

























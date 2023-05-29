#pragma once
#include<unordered_map>
#include"semDS.h"

using namespace::std;

const int MaxLevel = 50;
enum class TypeKind;
enum class IdKind;


/*
Symtable是一个符号表管理类
struct symbtable是符号表里的单元的结构
*/
class Symtable {
public:
    int cur_level;
    int CurOff;//全局偏移，局部化区偏移
    symbtable* cur_symtable = nullptr;//当前符号表的单元
    symbtable* table[MaxLevel] = { 0 };
    symbtable* table_tail[MaxLevel] = { 0 };//存上一层符号表的尾

public:
    Symtable();
    void init();
    void CreatTable();//创建一个空符号表
    symbtable* DestroyTable();//撤销本层符号表
    bool SearchoneTable(string, int, symbtable*&);//在当前符号表中找Id
    bool FindEntry(string, bool, symbtable*&);//在整个符号表中查找Id
    bool FindField(string, fieldChain*, fieldChain*&);//在域表中查找域名
    bool Enter(string, AttributeIR*, symbtable*&, bool);//填符号表

public:
    static string  typeKindShow(typeIR*);
    static string  varKindShow(AttributeIR&);
    static string  procKindShow(AttributeIR&);
    void PrintSymbTable();
    static void print(symbtable* root, int level = 0);
};








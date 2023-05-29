#pragma once
#include<iostream>
#include"global.h"
using namespace std;
//==========此文件为语义分析所用数据结构==============
struct symbtable;
struct typeIR;
struct ParamTable;
struct fieldChain;

//1.符号表的信息域
struct AttributeIR {
    typeIR* idtype;				//指向标识符类型内部表示
    IdKind kind;				//标识符种类
    union {
        struct {				//变量标识符的其他内容，kind = IdKind::varKind有用
            AccessKind access;
            int level;
            int off;
        }VarAttr;
        struct {				//过函标识符的其他内容，kind = IdKind::procKind有用
            int level;
            ParamTable* param;
            int code;
            int size;
        }ProcAttr;
    }More;
};

//3.记录的域表
struct fieldChain {
    string idname;
    typeIR* unitType;  //指向类型内部表示
    int offset;
    fieldChain* next;

    bool operator==(const fieldChain& rhs) const {
        return (idname == rhs.idname) && (unitType == rhs.unitType) && (offset == rhs.offset) && (next == rhs.next);
    }
};


//2.类型内部表示
struct typeIR {
    int size;                   //类型大小
    TypeKind kind;              //五种基本类型
    union {
        struct {
            typeIR* indexTy;    //当kind为arrayTy时有效，指向数组的下标的类型
            typeIR* elemTy;     //指向数组的元素的类型
            int low, up;
        }ArrayAttr;
        fieldChain* vody;        //当kind为记录recordTy时有效，指向域成员链表
    }More;

    typeIR& operator=(const typeIR& rhs) {
        if (this == &rhs) {
            return *this;
        }

        size = rhs.size;
        kind = rhs.kind;
        if (kind == TypeKind::arrayTy) {
            More.ArrayAttr.indexTy = rhs.More.ArrayAttr.indexTy;
            More.ArrayAttr.elemTy = rhs.More.ArrayAttr.elemTy;
            More.ArrayAttr.low = rhs.More.ArrayAttr.low;
            More.ArrayAttr.up = rhs.More.ArrayAttr.up;
        } else if (kind == TypeKind::recordTy) {
            fieldChain* curr = rhs.More.vody;
            More.vody = new fieldChain{curr->idname, curr->unitType, curr->offset, nullptr};
            fieldChain* prev = More.vody;
            curr = curr->next;
            while (curr) {
                prev->next = new fieldChain{curr->idname, curr->unitType, curr->offset, nullptr};
                prev = prev->next;
                curr = curr->next;
            }
        }

        return *this;
    }

    bool operator==(const typeIR& rhs) const {
        if (size != rhs.size || kind != rhs.kind) {
            return false;
        }

        if (kind == TypeKind::arrayTy) {
            return (More.ArrayAttr.low == rhs.More.ArrayAttr.low) && (More.ArrayAttr.up == rhs.More.ArrayAttr.up) && (*More.ArrayAttr.indexTy == *(rhs.More.ArrayAttr.indexTy)) && (*More.ArrayAttr.elemTy == *(rhs.More.ArrayAttr.elemTy));
        } else if (kind == TypeKind::recordTy) {
            fieldChain* curr1 = More.vody;
            fieldChain* curr2 = rhs.More.vody;
            while (curr1 && curr2) {
                if (!(*curr1 == *curr2)) {
                    return false;
                }
                curr1 = curr1->next;
                curr2 = curr2->next;
            }
            return (curr1 == curr2);
        }

        return true;
    }
};




//4.形参信息表
struct ParamTable {
    symbtable* entry;			//在符号表中位置
    ParamTable* next;			//下一个
};

//5.符号表的表项，小写开头；符号表是大写Symbtable.table
struct symbtable {
    string name;				//变量名
    AttributeIR attrIR;			//属性
    symbtable* next = nullptr;
    symbtable* nextLevel = nullptr;//保存函数下一层的符号表
};




#pragma once
#include<unordered_map>
#include<string>
#include"global.h"
//在语义分析中，将枚举类型转为字符串类型用于打印
extern std::unordered_map<LexType, std::string> LexName;
extern std::unordered_map<NodeKind, std::string>  NodeKindName;
extern std::unordered_map<DecKind, std::string>  DecKindName;
extern std::unordered_map<StmtKind, std::string>  StmtKindName;
extern std::unordered_map<ExpKind, std::string>  ExpKindName;
extern std::unordered_map<ParamType, std::string>  ParamTypeName;
extern std::unordered_map<VarKind, std::string>  VarKindName;
extern std::unordered_map<ExpType, std::string>  ExpTypeName;
extern std::unordered_map<TypeKind, std::string> TypeKind2str;
extern std::unordered_map<IdKind, std::string> IdKind2str;


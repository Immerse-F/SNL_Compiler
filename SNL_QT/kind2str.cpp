#include"Kind2Str.h"
#include"global.h"
//在语义分析中，将枚举类型转为字符串类型用于打印
std::unordered_map<LexType, std::string> LexName
{
    // 簿记单词符号
    {LexType::ENDFILE,"ENDFILE"},   {LexType::ERROR,"ERROR"},
    // 保留字
    {LexType::PROGRAM,"PROGRAM"},   {LexType::PROCEDURE,"PROCEDURE"}, {LexType::TYPE,"TYPE"},  {LexType::VAR,"VAR"},    {LexType::IF,"IF"},
    {LexType::THEN,"THEN"},      {LexType::ELSE,"ELSE"},    {LexType::FI,"FI"},        {LexType::WHILE,"WHILE"}, {LexType::DO,"DO"},
    {LexType::ENDWH,"ENDWH"},     {LexType::BEGIN,"BEGIN"},    {LexType::END1,"END"},   {LexType::READ,"READ"},      {LexType::WRITE,"WRITE"},
   {LexType::ARRAY,"ARRAY"},     {LexType::OF,"OF"},      {LexType::RECORD,"RECORD"},    {LexType::RETURN1,"RETURN"},
    {LexType::INTEGER,"INTEGER"}, {LexType::CHARC,"CHAR"},
    // 多字符单词符号
    {LexType::ID,"ID"},       {LexType::CHARC,"CHARC"},{LexType::INTC,"INTC"},
    // 特殊符号
    {LexType::ASSIGN,":="},        {LexType::EQ,"="},        {LexType::LT,"<"},       {LexType::PLUS,"+"},     {LexType::MINUS,"-"},
    {LexType::TIMES,"TIMES"}, {LexType::DIVIDE,"/"}, {LexType::LPAREN,"("},    {LexType::RPAREN,")"},        {LexType::DOT,"."},
    {LexType::COLON,":"},     {LexType::SEMI,";"},      {LexType::COMMA,","}, {LexType::LMIDPAREN,"["}, {LexType::RMIDPAREN,"]"},
    {LexType::UNDERANGE,".."}
};
std::unordered_map<NodeKind, std::string>  NodeKindName =
{
    {NodeKind::ProK, "ProK"},
    {NodeKind::PheadK, "PheadK"},
    {NodeKind::TypeK, "TypeK"},
    {NodeKind::VarK, "VarK"},
    {NodeKind::ProcDecK, "ProcDecK"},
    {NodeKind::StmLK, "StmLK"},
    {NodeKind::DecK, "DecK"},
    {NodeKind::StmtK, "StmtK"},
    {NodeKind::ExpK, "ExpK"}
};
std::unordered_map<DecKind, std::string>  DecKindName = {
   {DecKind::IdK, "IdK"},
   {DecKind::CharK, "CharK"},
   {DecKind::ArrayK, "ArrayK"},
   {DecKind::RecordK, "RecordK"},
   {DecKind::IntegerK, "IntegerK"},
};
std::unordered_map<StmtKind, std::string>  StmtKindName = {
   {StmtKind::IfK, "IfK"},
   {StmtKind::CallK, "CallK"},
   {StmtKind::ReadK, "ReadK"},
   {StmtKind::WhileK, "WhileK"},
   {StmtKind::WriteK, "WriteK"},
   {StmtKind::AssignK, "AssignK"},
   {StmtKind::ReturnK, "ReturnK"},
};
std::unordered_map<ExpKind, std::string>  ExpKindName = {
   {ExpKind::OpK, "OpK"},
   {ExpKind::VariK, "VariK"},
   {ExpKind::ConstK, "ConstK"},
};
std::unordered_map<ParamType, std::string>  ParamTypeName =
{
    {ParamType::valparamType,"val"},
    {ParamType::varparamType,"var"},
};
std::unordered_map<VarKind, std::string>  VarKindName =
{
    {VarKind::IdV,"IdV"},
    {VarKind::ArrayMembV,"ArrayMembV"},
    {VarKind::FieldMembV,"FieldMembV"},
};
std::unordered_map<ExpType, std::string>  ExpTypeName =
{
    {ExpType::Void,"Void"},
    {ExpType::Boolean,"Boolean"},
    {ExpType::Integer,"Integer"},
};
std::unordered_map<TypeKind, std::string> TypeKind2str =
{
    {TypeKind::intTy ,"intTy"},
    {TypeKind::arrayTy ,"arrayTy"},
    {TypeKind::boolTy ,"boolTy"},
    {TypeKind::charTy ,"charTy"},
    {TypeKind::recordTy ,"recordTy"},
};
std::unordered_map<IdKind, std::string> IdKind2str
{
    {IdKind::typeKind, "typeKind"},
    {IdKind::procKind, "procKind"},
    {IdKind::varKind, "varKind"},
    {IdKind::arrayNameKind, "arrayNameKind"}
};

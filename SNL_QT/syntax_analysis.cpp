#pragma once

#include "syntax_analysis.h"

int ParserLL1::nowLine = 0;
int ParserLL1::head = 0;

TreeNode* ParserLL1::newSpecNode(NodeKind kind)
{
    auto t = new TreeNode;
    int i;
    for (i = 0; i < 3; i++)
    {
        t->child[i] = nullptr;
    }
    t->sibling = nullptr;
    t->nodekind = kind;
    //t->lineno = lineno;
    t->lineno = ParserLL1::nowLine;
    t->idnum = 0;
    for (i = 0; i < 10; i++)
    {
        //t->table[i] = nullptr;
        t->table.push_back(nullptr);
    }
    return t;
}

TreeNode* ParserLL1::newStmtNode(StmtKind kind)
{
    auto t = ParserLL1::newSpecNode(NodeKind::StmtK);
    t->kind.stmt = kind;
    return t;
}

TreeNode* ParserLL1::newExpNode(ExpKind kind)
{
    auto t = ParserLL1::newSpecNode(NodeKind::ExpK);
    t->kind.exp = kind;
    return t;
}

SyntaxTree* ParserLL1::run(vector<struct Token> tokenList){
//SyntaxTree* ParserLL1::parse() {
    SyntaxTree* syntaxTree = new SyntaxTree();
    ParserLL1::head = 0;
    ParserLL1::nowLine = tokenList[head].line;
    symbal_stack.push(LexType::Program);
    syntaxTree->setRoot(newSpecNode(NodeKind::ProK));
    syntaxtree_stack.push(&syntaxTree->getRoot()->child[2]);
    syntaxtree_stack.push(&syntaxTree->getRoot()->child[1]);
    syntaxtree_stack.push(&syntaxTree->getRoot()->child[0]);

    while (!symbal_stack.empty()) {
        ParserLL1::nowLine = tokenList[head].line;
        if(head > INDEX){
            qDebug() << "越界";
            break;
        }
        //非终极
        if (NTSet.find(symbal_stack.top()) != NTSet.end()) {
            LexType toplex = symbal_stack.top();
            symbal_stack.pop();
            map<pair<LexType, LexType>, int>::iterator it = table.find(pair<LexType, LexType>(toplex,tokenList[head].wd.tok));
            if (it != table.end()) {
                process(it->second);
            }
            else {
                QString tempStr =QString::number(nowLine)+"\t行 语法错误:"+lexName[tokenList[head].wd.second];
                errorList.push_back(tempStr);
                //qDebug()<<nowLine<<"行 语法错误:"<<lexName[tokenList[head].wd.second];

                head++;
            }
            continue;
        }
        if (TTSet.find(symbal_stack.top()) != TTSet.end()) {
            if (symbal_stack.top() == tokenList[head].wd.tok) {
                LexType toplex = symbal_stack.top();
                symbal_stack.pop();
                head++;
            }
            else {
                QString tempStr =QString::number(nowLine)+"\t行 语法错误:"+lexName[tokenList[head].wd.second];
                errorList.push_back(tempStr);
                //qDebug()<<nowLine<<"行 语法错误:"<<lexName[tokenList[head].wd.second];

                head++;
                //return syntaxTree;
                //return NULL;
            }
            continue;
        }

    }
    return syntaxTree;
}

int ParserLL1::getPriosity(LexType op)
{
    int pri = 0;
    switch (op)
    {
    case LexType::END1:
        pri = -1;//0?
        break; //栈底标识，优先级最低
    case LexType::LPAREN:
        pri = 0;
    case LexType::LT:
        ;
    case LexType::EQ:
        pri = 1;
        break;
    case LexType::PLUS:
    case LexType::MINUS:
        pri = 2;
        break;
    case LexType::TIMES:
    case LexType::OVER:
        pri = 3;
        break;
    default:
        pri = -1;
    }
    return pri;
}



void ParserLL1::process(int id)
{
    switch (id)
    {
    case 1:
    {
        symbal_stack.push(LexType::DOT);
        symbal_stack.push(LexType::ProgramBody);
        symbal_stack.push(LexType::DeclarePart);
        symbal_stack.push(LexType::ProgramHead);
        break;
    }
    case 2:
    {
        symbal_stack.push(LexType::ProgramName);
        symbal_stack.push(LexType::PROGRAM);
        current_treenode = ParserLL1::newSpecNode(NodeKind::PheadK);
        TreeNode ** item = syntaxtree_stack.top();
        syntaxtree_stack.pop();
        (*item) = current_treenode;
        break;
    }
    case 3:
    {
        symbal_stack.push(LexType::ID);
        //标识符name
        current_treenode->name[0] = tokenList[head].wd.str;
        ++current_treenode->idnum;
        break;
    }
    case 4:
    {
        symbal_stack.push(LexType::ProcDec);
        symbal_stack.push(LexType::VarDec);
        symbal_stack.push(LexType::TypeDec);
        break;
    }
    case 5:
    {
        break;
    }
    case 6:
    {
        symbal_stack.push(LexType::TypeDeclaration);
        break;
    }
    case 7:
    {
        symbal_stack.push(LexType::TypeDecList);
        symbal_stack.push(LexType::TYPE);

        auto t = syntaxtree_stack.top();
        syntaxtree_stack.pop();
        current_treenode = ParserLL1::newSpecNode(NodeKind::TypeK);

        (*t) = current_treenode;
        syntaxtree_stack.push(&(current_treenode->sibling));
        syntaxtree_stack.push(&(current_treenode->child[0]));

        break;
    }
    case 8:
    {

        symbal_stack.push(LexType::TypeDecMore);
        symbal_stack.push(LexType::SEMI);
        symbal_stack.push(LexType::TypeName);
        symbal_stack.push(LexType::EQ);
        symbal_stack.push(LexType::TypeId);

        TreeNode** t = syntaxtree_stack.top();
        syntaxtree_stack.pop();

        current_treenode = ParserLL1::newSpecNode(NodeKind::DecK);
        (*t) = current_treenode;

        syntaxtree_stack.push(&(current_treenode->sibling));
        break;
    }
    case 9:
    {
        syntaxtree_stack.pop();
        break;
    }
    case 10:
    {
        symbal_stack.push(LexType::TypeDecList);
        break;
    }
    case 11:
    {
        symbal_stack.push(LexType::ID);
        current_treenode->name[0] = tokenList[head].wd.str;
        current_treenode->idnum++;
        break;
    }
    case 12:
    {
        symbal_stack.push(LexType::BaseType);
        temp = &(current_treenode->kind.dec);
        break;
    }
    case 13:
    {
        symbal_stack.push(LexType::StructureType);
        break;
    }
    case 14:
    {
        symbal_stack.push(LexType::ID);
        current_treenode->kind.dec = DecKind::IdK;
        current_treenode->type_name = tokenList[head].wd.str;
        //是否要++
        //current_treenode->idnum++;
        break;
    }
    case 15:
    {
        symbal_stack.push(LexType::INTEGER);
        (*temp) = DecKind::IntegerK;
        break;
    }
    case 16:
    {
        symbal_stack.push(LexType::CHAR1);
        (*temp) = DecKind::CharK;
        break;
    }
    case 17:
    {
        symbal_stack.push(LexType::ArrayType);

        break;
    }
    case 18:
    {
        symbal_stack.push(LexType::RecType);
        break;
    }
    case 19:
    {
        symbal_stack.push(LexType::BaseType);
        symbal_stack.push(LexType::OF);
        symbal_stack.push(LexType::RMIDPAREN);
        symbal_stack.push(LexType::Top);
        symbal_stack.push(LexType::UNDERANGE);
        symbal_stack.push(LexType::Low);
        symbal_stack.push(LexType::LMIDPAREN);
        symbal_stack.push(LexType::ARRAY);

        current_treenode->kind.dec = DecKind::ArrayK;
        temp = &current_treenode->attr.ArrayAttr.childtype;
        break;
    }
    case 20:
    {
        symbal_stack.push(LexType::INTC);
        current_treenode->attr.ArrayAttr.low = stoi(tokenList[head].wd.str);
        break;
    }
    case 21:
    {
        symbal_stack.push(LexType::INTC);
        current_treenode->attr.ArrayAttr.up = stoi(tokenList[head].wd.str);
        break;
    }
    case 22:
    {
        symbal_stack.push(LexType::END1);
        symbal_stack.push(LexType::FieldDecList);
        symbal_stack.push(LexType::RECORD);
        current_treenode->kind.dec = DecKind::RecordK;
        saveP = current_treenode;
        syntaxtree_stack.push(&(current_treenode->child[0]));
        break;
    }
    case 23:
    {
        symbal_stack.push(LexType::FieldDecMore);
        symbal_stack.push(LexType::SEMI);
        symbal_stack.push(LexType::IdList);
        symbal_stack.push(LexType::BaseType);
        TreeNode** t = syntaxtree_stack.top();
        syntaxtree_stack.pop();
        current_treenode = ParserLL1::newSpecNode(NodeKind::DecK);
        temp = &current_treenode->kind.dec;
        (*t) = current_treenode;
        syntaxtree_stack.push(&current_treenode->sibling);
        break;
    }
    case 24:
    {
        symbal_stack.push(LexType::FieldDecMore);
        symbal_stack.push(LexType::SEMI);
        symbal_stack.push(LexType::IdList);
        symbal_stack.push(LexType::ArrayType);

        TreeNode** t = syntaxtree_stack.top();
        syntaxtree_stack.pop();
        current_treenode = ParserLL1::newSpecNode(NodeKind::DecK); // 生成记录类型的下一个域，不添任何信息
        (*t) = current_treenode; // 若是第一个，则是record类型的子结点指向当前结点，否则，是上一个纪录域声明的兄弟结点
        syntaxtree_stack.push(&((*current_treenode).sibling));

        break;
    }
    case 25:
    {
        // 后面没有记录类型的下一个域了，恢复当前纪录类型节点的指针
        syntaxtree_stack.pop();
        current_treenode = saveP;
        break;
    }

    case 26:
    {
        symbal_stack.push(LexType::FieldDecList);
        break;
    }
    case 27:
    {
        symbal_stack.push(LexType::IdMore);
        symbal_stack.push(LexType::ID);

        // 纪录一个域中各个变量的语义信息
        current_treenode->name[current_treenode->idnum] = tokenList[head].wd.str;
        current_treenode->idnum++;
        break;
    }
    case 28:
    {
        break;
    }

    case 29:
    {
        symbal_stack.push(LexType::IdList);
        symbal_stack.push(LexType::COMMA);

        break;
    }
    case 30:
    {
        break;
    }

    case 31:
    {
        symbal_stack.push(LexType::VarDeclaration);

        break;
    }
    case 32:
    {
        symbal_stack.push(LexType::VarDecList);
        symbal_stack.push(LexType::VAR);

        current_treenode = ParserLL1::newSpecNode(NodeKind::VarK); // 生成一个标志变量声明的节点
        TreeNode** t = syntaxtree_stack.top();
        syntaxtree_stack.pop();
        (*t) = current_treenode;
        syntaxtree_stack.push(&((*current_treenode).sibling)); // 压入指向函数声明的指针
        syntaxtree_stack.push(&((*current_treenode).child[0])); // 压入指向第一个变量声明节点的指针

        break;
    }
    case 33:
    {
        symbal_stack.push(LexType::VarDecMore);
        symbal_stack.push(LexType::SEMI);
        symbal_stack.push(LexType::VarIdList);
        symbal_stack.push(LexType::TypeName);

        TreeNode** t = syntaxtree_stack.top();
        syntaxtree_stack.pop();
        current_treenode = ParserLL1::newSpecNode(NodeKind::DecK); // 建立一个新的声明节点，这里表示变量声明
        (*t) = current_treenode; // 若是第一个节点，则变量声明的头指针指向它，否则它是前一个变量声明的后继
        syntaxtree_stack.push(&((*current_treenode).sibling));

        break;
    }
    case 34:
    {
        syntaxtree_stack.pop();
        break;
    }

    case 35:
    {
        symbal_stack.push(LexType::VarDecList);
        break;
    }
    case 36:
    {
        symbal_stack.push(LexType::VarIdMore);
        symbal_stack.push(LexType::ID);

        current_treenode->name[current_treenode->idnum] = tokenList[head].wd.str;
        current_treenode->idnum++;
        break;
    }
    case 37:
    {
        break;
    }

    case 38:
    {
        symbal_stack.push(LexType::VarIdList);
        symbal_stack.push(LexType::COMMA);

        break;
    }
    case 39:
    {
        break;
    }

    case 40:
    {
        symbal_stack.push(LexType::ProcDeclaration);

        break;
    }
    case 41:
    {
        symbal_stack.push(LexType::ProcDecMore);
        symbal_stack.push(LexType::ProcBody);
        symbal_stack.push(LexType::ProcDecPart);
        symbal_stack.push(LexType::SEMI);
        symbal_stack.push(LexType::RPAREN);
        symbal_stack.push(LexType::ParamList);
        symbal_stack.push(LexType::LPAREN);
        symbal_stack.push(LexType::ProcName);
        symbal_stack.push(LexType::PROCEDURE);

        current_treenode = ParserLL1::newSpecNode(NodeKind::ProcDecK);
        TreeNode** t = syntaxtree_stack.top();
        syntaxtree_stack.pop();
        (*t) = current_treenode;

        syntaxtree_stack.push(&(current_treenode->sibling));
        syntaxtree_stack.push(&(current_treenode->child[2])); // 语句序列
        syntaxtree_stack.push(&(current_treenode->child[1])); // 函数的声明部分
        syntaxtree_stack.push(&(current_treenode->child[0])); // 指向参数声明
        break;
    }

    case 42:
    {
        break;
    }

    case 43:
    {
        symbal_stack.push(LexType::ProcDeclaration);
        break;
    }

    case 44:
    {
        symbal_stack.push(LexType::ID);

        current_treenode->name[0] = tokenList[head].wd.str;
        current_treenode->idnum++;
        break;
    }

    case 45:
    {
        /*形参部分为空，弹出指向形参的指针*/
        syntaxtree_stack.pop();
        break;
    }

    case 46:
    {
        symbal_stack.push(LexType::ParamDecList);
        break;
    }
    case 47:
    {
        symbal_stack.push(LexType::ParamMore);
        symbal_stack.push(LexType::Param);
        break;
    }
    case 48:
    {
        syntaxtree_stack.pop();
        break;
    }

    case 49:
    {
        symbal_stack.push(LexType::ParamDecList);
        symbal_stack.push(LexType::SEMI);
        break;
    }
    case 50:
    {
        symbal_stack.push(LexType::FormList);
        symbal_stack.push(LexType::TypeName);

        TreeNode** t = syntaxtree_stack.top();
        syntaxtree_stack.pop();
        current_treenode = ParserLL1::newSpecNode(NodeKind::DecK);
        // 函数的参数类型是值类型
        current_treenode->attr.ProcAttr.paramt = ParamType::valparamType;
        (*t) = current_treenode;
        syntaxtree_stack.push(&(current_treenode->sibling));

        break;
    }
    case 51:
    {
        symbal_stack.push(LexType::FormList);
        symbal_stack.push(LexType::TypeName);
        symbal_stack.push(LexType::VAR);

        TreeNode** t = syntaxtree_stack.top();
        syntaxtree_stack.pop();
        current_treenode = ParserLL1::newSpecNode(NodeKind::DecK);
        // 函数的参数类型是变量类型
        current_treenode->attr.ProcAttr.paramt = ParamType::varparamType;
        (*t) = current_treenode;
        syntaxtree_stack.push(&(current_treenode->sibling));

        break;
    }
    case 52:
    {
        symbal_stack.push(LexType::FidMore);
        symbal_stack.push(LexType::ID);

        current_treenode->name[current_treenode->idnum] = tokenList[head].wd.str;
        current_treenode->idnum++;

        break;
    }
    case 53:
    {
        break;
    }

    case 54:
    {
        symbal_stack.push(LexType::FormList);
        symbal_stack.push(LexType::COMMA);
        break;
    }

    case 55:
    {
        symbal_stack.push(LexType::DeclarePart);
        break;
    }
    case 56:
    {
        symbal_stack.push(LexType::ProgramBody);
        break;
    }

    case 57:
    {
        symbal_stack.push(LexType::END1);
        symbal_stack.push(LexType::StmList);
        symbal_stack.push(LexType::BEGIN);

        /*注意，若没有声明部分，则弹出的是程序或过程根节点中指向
          声明部分的指针child[1];若有声明部分，则弹出的是语句序列前
          的最后一个声明标识节点的兄弟指针；不管是哪种情况，都正好
          需要弹出语法树栈中的一个指针*/
        syntaxtree_stack.pop();

        /*建立语句序列标识节点*/
        TreeNode** t = syntaxtree_stack.top();
        syntaxtree_stack.pop();
        current_treenode = ParserLL1::newSpecNode(NodeKind::StmLK);
        (*t) = current_treenode;
        syntaxtree_stack.push(&(current_treenode->child[0]));
        break;
    }
    case 58:
    {
        symbal_stack.push(LexType::StmMore);
        symbal_stack.push(LexType::Stm);
        break;
    }
    case 59:
    {
        syntaxtree_stack.pop();
        break;
    }

    case 60:
    {
        symbal_stack.push(LexType::StmList);
        symbal_stack.push(LexType::SEMI);
        break;
    }
    case 61:
    {
        symbal_stack.push(LexType::ConditionalStm);

        current_treenode = ParserLL1::newStmtNode(StmtKind::IfK);
        //current_treenode->kind.stmt=;

        TreeNode** t = syntaxtree_stack.top();
        syntaxtree_stack.pop();
        (*t) = current_treenode;
        syntaxtree_stack.push(&current_treenode->sibling);

        break;
    }
    case 62:
    {
        symbal_stack.push(LexType::LoopStm);

        current_treenode = ParserLL1::newStmtNode(StmtKind::WhileK);
        //current_treenode->kind.stmt=;

        TreeNode** t = syntaxtree_stack.top();
        syntaxtree_stack.pop();
        (*t) = current_treenode;
        syntaxtree_stack.push(&current_treenode->sibling);

        break;
    }

    case 63:
    {
        symbal_stack.push(LexType::InputStm);

        TreeNode** t = syntaxtree_stack.top();
        syntaxtree_stack.pop();
        current_treenode = ParserLL1::newStmtNode(StmtKind::ReadK);
        //current_treenode->kind.stmt=;
        (*t) = current_treenode;
        syntaxtree_stack.push(&current_treenode->sibling);

        break;
    }
    case 64:
    {
        symbal_stack.push(LexType::OutputStm);

        TreeNode** t = syntaxtree_stack.top();
        syntaxtree_stack.pop();
        current_treenode = ParserLL1::newStmtNode(StmtKind::WriteK);
        (*t) = current_treenode;
        syntaxtree_stack.push(&current_treenode->sibling);

        break;
    }
    case 65:
    {
        symbal_stack.push(LexType::ReturnStm);

        TreeNode** t = syntaxtree_stack.top();
        syntaxtree_stack.pop();
        current_treenode = ParserLL1::newStmtNode(StmtKind::ReturnK);
        (*t) = current_treenode;
        syntaxtree_stack.push(&current_treenode->sibling);

        break;
    }

    case 66:
    {
        symbal_stack.push(LexType::AssCall);
        symbal_stack.push(LexType::ID);

        current_treenode = ParserLL1::newStmtNode(StmtKind::AssignK);

        /*赋值语句左部变量节点*/
        TreeNode* t = ParserLL1::newExpNode(ExpKind::VariK);
        t->name[0] = tokenList[head].wd.str;
        t->idnum++;

        /*赋值语句的child[0]指向左部的变量节点*/
        current_treenode->child[0] = t;

        TreeNode** t1 = syntaxtree_stack.top();
        syntaxtree_stack.pop();
        (*t1) = current_treenode;
        syntaxtree_stack.push(&current_treenode->sibling);

        break;
    }
    case 67:
    {
        symbal_stack.push(LexType::AssignmentRest);

        current_treenode->kind.stmt = StmtKind::AssignK;
        break;
    }
    case 68:
    {
        symbal_stack.push(LexType::CallStmRest);
        /*过程调用语句左部是标识符变量，表示过程名*/
        current_treenode->child[0]->attr.ExpAttr.varkind = VarKind::IdV;

        current_treenode->kind.stmt = StmtKind::CallK;
        break;
    }
    case 69:
    {
        symbal_stack.push(LexType::Exp);
        symbal_stack.push(LexType::ASSIGN);
        symbal_stack.push(LexType::VariMore);
        /*压入指向赋值右部的指针*/
        syntaxtree_stack.push(&(current_treenode->child[1]));
        /*当前指针指向赋值左部*/
        current_treenode = current_treenode->child[0];

        TreeNode* t = ParserLL1::newExpNode(ExpKind::OpK);
        t->attr.ExpAttr.op = LexType::END1;
        op_stack.push(t); //操作符栈的栈底存入一个特殊的操作符作为标志
        break;
    }
    case 70:
    {
        symbal_stack.push(LexType::FI);
        symbal_stack.push(LexType::StmList);
        symbal_stack.push(LexType::ELSE);
        symbal_stack.push(LexType::StmList);
        symbal_stack.push(LexType::THEN);
        symbal_stack.push(LexType::RelExp);
        symbal_stack.push(LexType::IF);

        syntaxtree_stack.push(&(current_treenode->child[2]));
        syntaxtree_stack.push(&(current_treenode->child[1]));
        syntaxtree_stack.push(&(current_treenode->child[0]));

        break;
    }

    case 71:
    {
        symbal_stack.push(LexType::ENDWH);
        symbal_stack.push(LexType::StmList);
        symbal_stack.push(LexType::DO);
        symbal_stack.push(LexType::RelExp);
        symbal_stack.push(LexType::WHILE);

        syntaxtree_stack.push(&(current_treenode->child[1]));
        syntaxtree_stack.push(&(current_treenode->child[0]));

        break;
    }

    case 72:
    {
        symbal_stack.push(LexType::RPAREN);
        symbal_stack.push(LexType::InVar);
        symbal_stack.push(LexType::LPAREN);
        symbal_stack.push(LexType::READ);
        break;
    }
    case 73:
    {
        symbal_stack.push(LexType::ID);

        current_treenode->name[0] = tokenList[head].wd.str;
        current_treenode->idnum++;
        break;
    }
    case 74:
    {
        symbal_stack.push(LexType::RPAREN);
        symbal_stack.push(LexType::Exp);
        symbal_stack.push(LexType::LPAREN);
        symbal_stack.push(LexType::WRITE);

        syntaxtree_stack.push(&(current_treenode->child[0]));

        TreeNode* t = ParserLL1::newExpNode(ExpKind::OpK);
        t->attr.ExpAttr.op = LexType::END1;
        op_stack.push(t); //操作符栈的栈底存入一个特殊的操作符作为标志
        break;
    }
    case 75:
    {
        symbal_stack.push(LexType::RETURN1);
        break;
    }

    case 76:
    {
        symbal_stack.push(LexType::RPAREN);
        symbal_stack.push(LexType::ActParamList);
        symbal_stack.push(LexType::LPAREN);

        syntaxtree_stack.push(&(current_treenode->child[1]));
        break;
    }
    case 77:
    {
        syntaxtree_stack.pop();
        break;
    }
    case 78:
    {
        symbal_stack.push(LexType::ActParamMore);
        symbal_stack.push(LexType::Exp);

        TreeNode* t = ParserLL1::newExpNode(ExpKind::OpK);
        t->attr.ExpAttr.op = LexType::END1;
        op_stack.push(t); //操作符栈的栈底存入一个特殊的操作符作为标志
        break;
    }

    case 79:
    {
        break;
    }
    case 80:
    {
        symbal_stack.push(LexType::ActParamList);
        symbal_stack.push(LexType::COMMA);

        syntaxtree_stack.push(&(current_treenode->sibling));
        break;
    }

    case 81:
    {
        symbal_stack.push(LexType::OtherRelE);
        symbal_stack.push(LexType::Exp);

        TreeNode* t = ParserLL1::newExpNode(ExpKind::OpK);
        t->attr.ExpAttr.op = LexType::END1;
        op_stack.push(t); //操作符栈的栈底存入一个特殊的操作符作为标志

        getExpResult = false;
        break;
    }

    case 82:
    {
        symbal_stack.push(LexType::Exp);
        symbal_stack.push(LexType::CmpOp);

        TreeNode* current_treenode = ParserLL1::newExpNode(ExpKind::OpK);
        current_treenode->attr.ExpAttr.op = tokenList[head].wd.tok;

        LexType sTop = op_stack.top()->attr.ExpAttr.op;
        while (this->getPriosity(sTop) >= this->getPriosity(tokenList[head].wd.tok))
            /*如果操作符栈顶运算符的优先级高于或等于当前读到的操作符*/
        {
            TreeNode* t = op_stack.top();
            op_stack.pop();
            TreeNode* Rnum = num_stack.top();
            num_stack.pop();
            TreeNode* Lnum = num_stack.top();
            num_stack.pop();
            t->child[1] = Rnum;
            t->child[0] = Lnum;
            num_stack.push(t);

            sTop = op_stack.top()->attr.ExpAttr.op;
        }

        op_stack.push(current_treenode);
        /*处理完关系操作符右部的表达式时，要弹语法树栈，故
          设置getExpResult为真*/
        getExpResult = true;
        break;
    }

    case 83:
    {
        symbal_stack.push(LexType::OtherTerm);
        symbal_stack.push(LexType::Term);
        break;
    }

    case 84:
    {
        if ((tokenList[head].wd.tok == LexType::RPAREN) && (expflag != 0))
            //说明当前右括号是表达式中的一部分
        {
            while (op_stack.top()->attr.ExpAttr.op != LexType::LPAREN)
            {
                TreeNode* t = op_stack.top();
                op_stack.pop();
                TreeNode* Rnum = num_stack.top();
                num_stack.pop();
                TreeNode* Lnum = num_stack.top();
                num_stack.pop();

                t->child[1] = Rnum;
                t->child[0] = Lnum;
                num_stack.push(t);
            }
            op_stack.pop(); //弹出左括号
            expflag--;
        }
        else
        {
            if ((getExpResult) || (getExpResult2))
            {
                while (op_stack.top()->attr.ExpAttr.op != LexType::END1)
                {
                    TreeNode* t = op_stack.top();
                    op_stack.pop();
                    TreeNode* Rnum = num_stack.top();
                    num_stack.pop();
                    TreeNode* Lnum = num_stack.top();
                    num_stack.pop();

                    t->child[1] = Rnum;
                    t->child[0] = Lnum;
                    num_stack.push(t);
                }
                op_stack.pop(); //弹出栈底标志
                current_treenode = num_stack.top();
                num_stack.pop();

                TreeNode** t = syntaxtree_stack.top();
                syntaxtree_stack.pop();
                (*t) = current_treenode;

                /*处理完数组变量，标志恢复初始值假，
                  遇到下一个数组下标表达式时，再将其设置为真值*/
                if (getExpResult2 == true)
                    getExpResult2 = false;
            }
        }
        break;
    }

    case 85:
    {
        symbal_stack.push(LexType::Exp);
        symbal_stack.push(LexType::AddOp);

        TreeNode* current_treenode = ParserLL1::newExpNode(ExpKind::OpK);
        current_treenode->attr.ExpAttr.op = tokenList[head].wd.tok;
        LexType sTop = op_stack.top()->attr.ExpAttr.op;
        while (this->getPriosity(sTop) >= this->getPriosity(tokenList[head].wd.tok))
        {
            TreeNode* t = op_stack.top();
            op_stack.pop();
            TreeNode* Rnum = num_stack.top();
            num_stack.pop();
            TreeNode* Lnum = num_stack.top();
            num_stack.pop();
            t->child[1] = Rnum;
            t->child[0] = Lnum;
            num_stack.push(t);
            sTop = op_stack.top()->attr.ExpAttr.op;
        }
        op_stack.push(current_treenode);
        break;
    }

    case 86:
    {
        symbal_stack.push(LexType::OtherFactor);
        symbal_stack.push(LexType::Factor);
        break;
    }
    case 87:
    {
        break;
    }
    case 88:
    {
        symbal_stack.push(LexType::Term);
        symbal_stack.push(LexType::MultOp);

        TreeNode* current_treenode = ParserLL1::newExpNode(ExpKind::OpK);
        current_treenode->attr.ExpAttr.op = tokenList[head].wd.tok;

        LexType sTop = op_stack.top()->attr.ExpAttr.op;
        while (this->getPriosity(sTop) >= this->getPriosity(tokenList[head].wd.tok))
            /*如果操作符栈顶运算符的优先级高于或等于当前读到的操作符*/
        {
            TreeNode* t = op_stack.top();
            op_stack.pop();
            TreeNode* Rnum = num_stack.top();
            num_stack.pop();
            TreeNode* Lnum = num_stack.top();
            num_stack.pop();
            t->child[1] = Rnum;
            t->child[0] = Lnum;
            num_stack.push(t);

            sTop = op_stack.top()->attr.ExpAttr.op;
        }
        op_stack.push(current_treenode);
        break;
    }

    case 89:
    {
        symbal_stack.push(LexType::RPAREN);
        symbal_stack.push(LexType::Exp);
        symbal_stack.push(LexType::LPAREN);

        TreeNode* t = ParserLL1::newExpNode(ExpKind::OpK);
        t->attr.ExpAttr.op = tokenList[head].wd.tok; /*把左括号也压入栈中*/
        op_stack.push(t);
        expflag++;
        break;
    }

    case 90:
    {
        symbal_stack.push(LexType::INTC);

        TreeNode* t = ParserLL1::newExpNode(ExpKind::ConstK);

        t->attr.ExpAttr.val = stoi(tokenList[head].wd.str);
        //解决ExpK后不显示
        t->name[0] = tokenList[head].wd.str;

        /*常数节点入操作数栈*/
        num_stack.push(t);

        break;
    }

    case 91:
    {
        symbal_stack.push(LexType::Variable);
        break;
    }

    case 92:
    {
        symbal_stack.push(LexType::VariMore);
        symbal_stack.push(LexType::ID);

        current_treenode = ParserLL1::newExpNode(ExpKind::VariK);
        current_treenode->name[0] = tokenList[head].wd.str;
        current_treenode->idnum++;
        // 变量节点入操作数栈
        num_stack.push(current_treenode);

        break;
    }

    case 93:
    {
        // 标识符变量
        current_treenode->attr.ExpAttr.varkind = VarKind::IdV;
        break;
    }

    case 94:
    {
        symbal_stack.push(LexType::RMIDPAREN);
        symbal_stack.push(LexType::Exp);
        symbal_stack.push(LexType::LMIDPAREN);
        // 数组成员变量
        current_treenode->attr.ExpAttr.varkind = VarKind::ArrayMembV;
        syntaxtree_stack.push(&current_treenode->child[0]);

        /*要进入表达式处理，初始化操作符栈*/
        //操作符栈的栈底存入一个特殊的操作符作为标志
        TreeNode* t = ParserLL1::newExpNode(ExpKind::OpK);
        t->attr.ExpAttr.op = LexType::END1;
        op_stack.push(t);

        /*要进入数组下标表达式处理，在函数process84处理中，要
          操作语法树栈，故将标志getExpResult2设置为真值*/
        getExpResult2 = true;

        break;
    }

    case 95:
    {
        symbal_stack.push(LexType::FieldVar);
        symbal_stack.push(LexType::DOT);
        // 域成员变量
        current_treenode->attr.ExpAttr.varkind = VarKind::FieldMembV;
        syntaxtree_stack.push(&current_treenode->child[0]);
        break;
    }

    case 96:
    {
        symbal_stack.push(LexType::FieldVarMore);
        symbal_stack.push(LexType::ID);

        // 纪录域的成员
        current_treenode = ParserLL1::newExpNode(ExpKind::VariK);
        current_treenode->name[0] = tokenList[head].wd.str;
        current_treenode->idnum++;

        TreeNode** t = syntaxtree_stack.top();
        syntaxtree_stack.pop();
        (*t) = current_treenode;

        break;
    }

    case 97:
    {
        /*域成员是标识符变量*/
        current_treenode->attr.ExpAttr.varkind = VarKind::IdV;
        break;
    }

    case 98:
    {
        symbal_stack.push(LexType::RMIDPAREN);
        symbal_stack.push(LexType::Exp);
        symbal_stack.push(LexType::LMIDPAREN);
        // 域成员是数组变量
        current_treenode->attr.ExpAttr.varkind = VarKind::ArrayMembV;
        // 指向数组成员表达式
        syntaxtree_stack.push(&current_treenode->child[0]);

        // 操作符栈的栈底存入一个特殊的操作符作为标志
        TreeNode* t = ParserLL1::newExpNode(ExpKind::OpK);
        t->attr.ExpAttr.op = LexType::END1;
        op_stack.push(t);

        /*要进入数组下标表达式处理，在函数process84处理中，要
          操作语法树栈，故将标志getExpResult2设置为真值*/
        getExpResult2 = true;
        break;
    }
    case 99:
    {
        symbal_stack.push(LexType::LT);
        break;
    }

    case 100:
    {
        symbal_stack.push(LexType::EQ);
        break;
    }

    case 101:
    {
        symbal_stack.push(LexType::PLUS);
        break;
    }

    case 102:
    {
        symbal_stack.push(LexType::MINUS);
        break;
    }

    case 103:
    {
        symbal_stack.push(LexType::TIMES);
        break;
    }

    case 104:
    {
        symbal_stack.push(LexType::OVER);
        break;
    }
    }
}


ParserLL1::ParserLL1(bool IS_CALL )
{

    if(IS_CALL){
        initProduction();
        get_mp();
        getFirst();
        getFollow();
        getPredict();
    }

    else this->table =
    {
        { { LexType::Program, LexType::PROGRAM }, 1 },
        { { LexType::ProgramHead, LexType::PROGRAM }, 2 },
        { { LexType::ProgramName, LexType::ID }, 3 },
        { { LexType::DeclarePart, LexType::TYPE }, 4 },
        { { LexType::DeclarePart, LexType::VAR }, 4 },
        { { LexType::DeclarePart, LexType::PROCEDURE }, 4 },
        { { LexType::DeclarePart, LexType::BEGIN }, 4 },
        { { LexType::TypeDec, LexType::TYPE }, 6 },
        { { LexType::TypeDec, LexType::VAR }, 5 },
        { { LexType::TypeDec, LexType::PROCEDURE }, 5 },
        { { LexType::TypeDec, LexType::BEGIN }, 5 },
        { { LexType::TypeDeclaration, LexType::TYPE }, 7 },
        { { LexType::TypeDecList, LexType::ID }, 8 },
        { { LexType::TypeDecMore, LexType::VAR }, 9 },
        { { LexType::TypeDecMore, LexType::PROCEDURE }, 9 },
        { { LexType::TypeDecMore, LexType::BEGIN }, 9 },
        { { LexType::TypeDecMore, LexType::ID }, 10 },
        { { LexType::TypeId, LexType::ID }, 11 },
        { { LexType::TypeName, LexType::INTEGER }, 12 },
        { { LexType::TypeName, LexType::CHAR1 }, 12 },
        { { LexType::TypeName, LexType::ARRAY }, 13 },
        { { LexType::TypeName, LexType::RECORD }, 13 },
        { { LexType::TypeName, LexType::ID }, 14 },
        { { LexType::BaseType, LexType::INTEGER }, 15 },
        { { LexType::BaseType, LexType::CHAR1 }, 16 },
        { { LexType::StructureType, LexType::ARRAY }, 17 },
        { { LexType::StructureType, LexType::RECORD }, 18 },
        { { LexType::ArrayType, LexType::ARRAY }, 19 },
        { { LexType::Low, LexType::INTC}, 20 },
        { { LexType::Top, LexType::INTC }, 21 },
        { { LexType::RecType, LexType::RECORD }, 22 },
        { { LexType::FieldDecList, LexType::INTEGER }, 23 },
        { { LexType::FieldDecList, LexType::CHAR1 }, 23 },
        { { LexType::FieldDecList, LexType::ARRAY }, 24 },
        { { LexType::FieldDecMore, LexType::INTEGER }, 26 },
        { { LexType::FieldDecMore, LexType::CHAR1 }, 26 },
        { { LexType::FieldDecMore, LexType::ARRAY }, 26 },
        { { LexType::FieldDecMore, LexType::END1 }, 25 },
        { { LexType::IdList, LexType::ID }, 27 },
        { { LexType::IdMore, LexType::SEMI }, 28 },
        { { LexType::IdMore, LexType::COMMA }, 29 },
        { { LexType::VarDec, LexType::VAR }, 31 },
        { { LexType::VarDec, LexType::PROCEDURE }, 30 },
        { { LexType::VarDec, LexType::BEGIN }, 30 },
        { { LexType::VarDeclaration, LexType::VAR }, 32 },
        { { LexType::VarDecList, LexType::INTEGER }, 33 },
        { { LexType::VarDecList, LexType::CHAR1 }, 33 },
        { { LexType::VarDecList, LexType::ARRAY }, 33 },
        { { LexType::VarDecList, LexType::RECORD }, 33 },
        { { LexType::VarDecList, LexType::ID }, 33 },
        { { LexType::VarDecMore, LexType::INTEGER }, 35 },
        { { LexType::VarDecMore, LexType::CHAR1 }, 35 },
        { { LexType::VarDecMore, LexType::ARRAY }, 35 },
        { { LexType::VarDecMore, LexType::RECORD }, 35 },
        { { LexType::VarDecMore, LexType::PROCEDURE }, 34 },
        { { LexType::VarDecMore, LexType::BEGIN }, 34 },
        { { LexType::VarDecMore, LexType::ID }, 35 },
        { { LexType::VarIdList, LexType::ID }, 36 },
        { { LexType::VarIdMore, LexType::SEMI }, 37 },
        { { LexType::VarIdMore, LexType::COMMA }, 38 },
        { { LexType::ProcDec, LexType::PROCEDURE }, 40 },
        { { LexType::ProcDec, LexType::BEGIN }, 39 },
        { { LexType::ProcDeclaration, LexType::PROCEDURE }, 41 },
        { { LexType::ProcDecMore, LexType::PROCEDURE }, 43 },
        { { LexType::ProcDecMore, LexType::BEGIN }, 42 },
        { { LexType::ProcName, LexType::ID }, 44 },
        { { LexType::ParamList, LexType::INTEGER }, 46 },
        { { LexType::ParamList, LexType::CHAR1 }, 46 },
        { { LexType::ParamList, LexType::ARRAY }, 46 },
        { { LexType::ParamList, LexType::RECORD }, 46 },
        { { LexType::ParamList, LexType::VAR }, 46 },
        { { LexType::ParamList, LexType::ID }, 46 },
        { { LexType::ParamList, LexType::RMIDPAREN }, 45 },
        { { LexType::ParamDecList, LexType::INTEGER }, 47 },
        { { LexType::ParamDecList, LexType::CHAR1 }, 47 },
        { { LexType::ParamDecList, LexType::ARRAY }, 47 },
        { { LexType::ParamDecList, LexType::RECORD }, 47 },
        { { LexType::ParamDecList, LexType::VAR }, 47 },
        { { LexType::ParamDecList, LexType::ID }, 47 },
        { { LexType::ParamMore, LexType::SEMI }, 49 },
        { { LexType::ParamMore, LexType::RPAREN }, 48 },
        { { LexType::Param, LexType::INTEGER }, 50 },
        { { LexType::Param, LexType::CHAR1 }, 50 },
        { { LexType::Param, LexType::ARRAY }, 50 },
        { { LexType::Param, LexType::RECORD }, 50 },
        { { LexType::Param, LexType::ID }, 50 },
        { { LexType::Param, LexType::VAR }, 51 },
        { { LexType::FormList, LexType::ID }, 52 },
        { { LexType::FidMore, LexType::SEMI }, 53 },
        { { LexType::FidMore, LexType::COMMA }, 54 },
        { { LexType::FidMore, LexType::RPAREN }, 53 },
        { { LexType::ProcDecPart, LexType::TYPE }, 55 },
        { { LexType::ProcDecPart, LexType::VAR }, 55 },
        { { LexType::ProcDecPart, LexType::PROCEDURE }, 55 },
        { { LexType::ProcDecPart, LexType::BEGIN }, 55 },
        { { LexType::ProcBody, LexType::BEGIN }, 56 },
        { { LexType::ProgramBody, LexType::BEGIN }, 57 },
        { { LexType::StmList, LexType::IF }, 58 },
        { { LexType::StmList, LexType::WHILE }, 58 },
        { { LexType::StmList, LexType::READ }, 58 },
        { { LexType::StmList, LexType::WRITE }, 58 },
        { { LexType::StmList, LexType::RETURN1 }, 58 },
        { { LexType::StmList, LexType::ID }, 58 },
        { { LexType::StmMore, LexType::END1 }, 59 },
        { { LexType::StmMore, LexType::ELSE }, 59 },
        { { LexType::StmMore, LexType::FI }, 59 },
        { { LexType::StmMore, LexType::ENDWH }, 59 },
        { { LexType::StmMore, LexType::SEMI }, 60 },
        { { LexType::Stm, LexType::IF }, 61 },
        { { LexType::Stm, LexType::WHILE }, 62 },
        { { LexType::Stm, LexType::READ }, 63 },
        { { LexType::Stm, LexType::WRITE }, 64 },
        { { LexType::Stm, LexType::RETURN1 }, 65 },
        { { LexType::Stm, LexType::ID }, 66 },
        { { LexType::AssCall, LexType::ASSIGN }, 67 },
        { { LexType::AssCall, LexType::DOT }, 67 },
        { { LexType::AssCall, LexType::LMIDPAREN }, 67 },
        { { LexType::AssCall, LexType::LPAREN }, 68 },
        { { LexType::AssignmentRest, LexType::DOT }, 69 },
        { { LexType::AssignmentRest, LexType::LMIDPAREN }, 69 },
        { { LexType::AssignmentRest, LexType::ASSIGN }, 69 },
        { { LexType::ConditionalStm, LexType::IF }, 70 },
        { { LexType::LoopStm, LexType::WHILE }, 71 },
        { { LexType::InputStm, LexType::READ }, 72 },
        { { LexType::InVar, LexType::ID }, 73 },
        { { LexType::OutputStm, LexType::WRITE }, 74 },
        { { LexType::ReturnStm, LexType::RETURN1 }, 75 },
        { { LexType::CallStmRest, LexType::LPAREN }, 76 },
        { { LexType::ActParamList, LexType::INTC }, 78 },
        { { LexType::ActParamList, LexType::ID }, 78 },
        { { LexType::ActParamList, LexType::LPAREN }, 78 },
        { { LexType::ActParamList, LexType::RPAREN }, 77 },
        { { LexType::ActParamMore, LexType::COMMA }, 80 },
        { { LexType::ActParamMore, LexType::RPAREN }, 79 },
        { { LexType::RelExp, LexType::INTC }, 81 },
        { { LexType::RelExp, LexType::ID }, 81 },
        { { LexType::RelExp, LexType::LPAREN }, 81 },
        { { LexType::OtherRelE, LexType::LT }, 82 },
        { { LexType::OtherRelE, LexType::EQ }, 82 },
        { { LexType::Exp, LexType::INTC }, 83 },
        { { LexType::Exp, LexType::ID }, 83 },
        { { LexType::Exp, LexType::LPAREN }, 83 },
                { { LexType::OtherTerm, LexType::END1 }, 84 },
                { { LexType::OtherTerm, LexType::THEN }, 84 },
                { { LexType::OtherTerm, LexType::ELSE }, 84 },
                { { LexType::OtherTerm, LexType::FI }, 84 },
                { { LexType::OtherTerm, LexType::DO }, 84 },
                { { LexType::OtherTerm, LexType::ENDWH }, 84 },
                { { LexType::OtherTerm, LexType::SEMI }, 84 },
                { { LexType::OtherTerm, LexType::COMMA }, 84 },
                { { LexType::OtherTerm, LexType::RPAREN }, 84 },
                { { LexType::OtherTerm, LexType::RMIDPAREN }, 84 },
                { { LexType::OtherTerm, LexType::LT }, 84 },
                { { LexType::OtherTerm, LexType::EQ }, 84 },
                { { LexType::OtherTerm, LexType::PLUS }, 85 },
                { { LexType::OtherTerm, LexType::MINUS }, 85 },
                { { LexType::Term, LexType::LPAREN }, 86 },
                { { LexType::Term, LexType::INTC }, 86 },
                { { LexType::Term, LexType::ID }, 86 },
        { { LexType::OtherFactor, LexType::END1 }, 87 },
        { { LexType::OtherFactor, LexType::THEN }, 87 },
        { { LexType::OtherFactor, LexType::ELSE }, 87 },
        { { LexType::OtherFactor, LexType::FI }, 87 },
        { { LexType::OtherFactor, LexType::DO }, 87 },
        { { LexType::OtherFactor, LexType::ENDWH }, 87 },
        { { LexType::OtherFactor, LexType::SEMI }, 87 },
        { { LexType::OtherFactor, LexType::COMMA }, 87 },
        { { LexType::OtherFactor, LexType::RPAREN }, 87 },
        { { LexType::OtherFactor, LexType::RMIDPAREN }, 87 },
        { { LexType::OtherFactor, LexType::LT }, 87 },
        { { LexType::OtherFactor, LexType::EQ }, 87 },
        { { LexType::OtherFactor, LexType::PLUS }, 87 },
        { { LexType::OtherFactor, LexType::MINUS }, 87 },
        { { LexType::OtherFactor, LexType::TIMES }, 88 },
        { { LexType::OtherFactor, LexType::OVER }, 88 },
                { { LexType::Factor, LexType::LPAREN }, 89 },
        { { LexType::Factor, LexType::INTC }, 90 },
                { { LexType::Factor, LexType::ID }, 91 },
                { { LexType::Variable, LexType::ID }, 92 },
        { { LexType::VariMore, LexType::END1 }, 93 },
                { { LexType::VariMore, LexType::THEN }, 93 },
                { { LexType::VariMore, LexType::ELSE }, 93 },
                { { LexType::VariMore, LexType::FI }, 93 },
                { { LexType::VariMore, LexType::DO }, 93 },
                { { LexType::VariMore, LexType::ENDWH }, 93 },
                { { LexType::VariMore, LexType::SEMI }, 93 },
                { { LexType::VariMore, LexType::COMMA }, 93 },
                { { LexType::VariMore, LexType::RPAREN }, 93 },
                { { LexType::VariMore, LexType::RMIDPAREN }, 93 },
                { { LexType::VariMore, LexType::LT }, 93 },
        { { LexType::VariMore, LexType::EQ }, 93 },
                { { LexType::VariMore, LexType::PLUS }, 93 },
                { { LexType::VariMore, LexType::MINUS }, 93 },
                { { LexType::VariMore, LexType::TIMES }, 93 },
                { { LexType::VariMore, LexType::OVER }, 93 },
                { { LexType::VariMore, LexType::ASSIGN }, 93 },
                { { LexType::VariMore, LexType::LMIDPAREN }, 94 },
                { { LexType::VariMore, LexType::DOT }, 95 },
                { { LexType::FieldVar, LexType::ID }, 96 },
        { { LexType::FieldVarMore, LexType::END1 }, 97 },
        { { LexType::FieldVarMore, LexType::THEN }, 97 },
        { { LexType::FieldVarMore, LexType::ELSE }, 97 },
        { { LexType::FieldVarMore, LexType::FI }, 97 },
        { { LexType::FieldVarMore, LexType::DO }, 97 },
        { { LexType::FieldVarMore, LexType::ENDWH }, 97 },
        { { LexType::FieldVarMore, LexType::SEMI }, 97 },
        { { LexType::FieldVarMore, LexType::COMMA }, 97 },
        { { LexType::FieldVarMore, LexType::RPAREN }, 97 },
        { { LexType::FieldVarMore, LexType::RMIDPAREN }, 97 },
        { { LexType::FieldVarMore, LexType::LT }, 97 },
        { { LexType::FieldVarMore, LexType::EQ }, 97 },
        { { LexType::FieldVarMore, LexType::PLUS }, 97 },
        { { LexType::FieldVarMore, LexType::MINUS }, 97 },
        { { LexType::FieldVarMore, LexType::TIMES }, 97 },
        { { LexType::FieldVarMore, LexType::OVER }, 97 },
        { { LexType::FieldVarMore, LexType::ASSIGN }, 97 },
        { { LexType::FieldVarMore, LexType::LMIDPAREN }, 98 },
        { { LexType::CmpOp, LexType::LT }, 99 },
        { { LexType::CmpOp, LexType::EQ }, 100 },
        { { LexType::AddOp, LexType::PLUS }, 101 },
        { { LexType::AddOp, LexType::MINUS }, 102 },
        { { LexType::MultOp, LexType::TIMES }, 103 },
        { { LexType::MultOp, LexType::OVER }, 104 },
    };
}
void ParserLL1::initProduction() {
    Production.push_back({ LexType::Program, std::vector<LexType>{LexType::ProgramHead, LexType::DeclarePart, LexType::ProgramBody, LexType::DOT} });
    Production.push_back({ LexType::ProgramHead, std::vector<LexType>{LexType::PROGRAM, LexType::ProgramName} });
    Production.push_back({ LexType::ProgramName, std::vector<LexType>{LexType::ID} });
    Production.push_back({ LexType::DeclarePart, std::vector<LexType>{LexType::TypeDec,LexType::VarDec,LexType::ProcDec} });
    Production.push_back({ LexType::TypeDec, std::vector<LexType>{LexType::Empty} });
    Production.push_back({ LexType::TypeDec, std::vector<LexType>{LexType::TypeDeclaration} });
    Production.push_back({ LexType::TypeDeclaration, std::vector<LexType>{LexType::TYPE,LexType::TypeDecList} });
    Production.push_back({ LexType::TypeDecList, std::vector<LexType>{LexType::TypeId,LexType::EQ,LexType::SEMI,LexType::TypeDecMore} });
    Production.push_back({ LexType::TypeDecMore, std::vector<LexType>{LexType::Empty} });
    Production.push_back({ LexType::TypeDecMore, std::vector<LexType>{LexType::TypeDecList} });
    Production.push_back({ LexType::TypeId, std::vector<LexType>{LexType::ID} });
    Production.push_back({ LexType::TypeName, std::vector<LexType>{LexType::BaseType} });
    Production.push_back({ LexType::TypeName, std::vector<LexType>{LexType::StructureType} });
    Production.push_back({ LexType::TypeName, std::vector<LexType>{LexType::ID} });
    Production.push_back({ LexType::BaseType, std::vector<LexType>{LexType::INTEGER} });
    Production.push_back({ LexType::BaseType, std::vector<LexType>{LexType::CHAR1} });
    Production.push_back({ LexType::StructureType, std::vector<LexType>{LexType::ArrayType} });
    Production.push_back({ LexType::StructureType, std::vector<LexType>{LexType::RecType} });
    Production.push_back({ LexType::ArrayType, std::vector<LexType>{LexType::ARRAY,LexType::LMIDPAREN,LexType::Low,LexType::UNDERANGE,LexType::Top,LexType::RMIDPAREN,LexType::OF,LexType::BaseType} });
    Production.push_back({ LexType::Low, std::vector<LexType>{LexType::INTC} });
    Production.push_back({ LexType::Top, std::vector<LexType>{LexType::INTC} });
    Production.push_back({ LexType::RecType, std::vector<LexType>{LexType::RECORD,LexType::FieldDecList,LexType::END1} });
    Production.push_back({ LexType::FieldDecList, std::vector<LexType>{LexType::BaseType,LexType::IdList,LexType::SEMI,LexType::FieldDecMore} });
    Production.push_back({ LexType::FieldDecList, std::vector<LexType>{LexType::ArrayType,LexType::IdList,LexType::SEMI,LexType::FieldDecMore} });
    Production.push_back({ LexType::FieldDecMore, std::vector<LexType>{LexType::Empty} });
    Production.push_back({ LexType::FieldDecMore, std::vector<LexType>{LexType::FieldDecList} });
    Production.push_back({ LexType::IdList, std::vector<LexType>{LexType::ID ,LexType::IdMore} });
    Production.push_back({ LexType::IdMore	, std::vector<LexType>{LexType::Empty} });
    Production.push_back({ LexType::IdMore	, std::vector<LexType>{LexType::COMMA,LexType::IdList} });
    Production.push_back({ LexType::VarDec	, std::vector<LexType>{LexType::Empty} });
    Production.push_back({ LexType::VarDec	, std::vector<LexType>{LexType::VarDeclaration} });
    Production.push_back({ LexType::VarDeclaration	, std::vector<LexType>{LexType::VAR,LexType::VarDecList} });
    Production.push_back({ LexType::VarDecList	, std::vector<LexType>{LexType::TypeName,LexType::VarIdList,LexType::SEMI,LexType::VarDecMore} });
    Production.push_back({ LexType::VarDecMore	, std::vector<LexType>{LexType::Empty} });
    Production.push_back({ LexType::VarDecMore	, std::vector<LexType>{LexType::VarDecList} });
    Production.push_back({ LexType::VarIdList	, std::vector<LexType>{LexType::ID,LexType::VarIdMore} });
    Production.push_back({ LexType::VarIdMore	, std::vector<LexType>{LexType::Empty} });
    Production.push_back({ LexType::VarIdMore	, std::vector<LexType>{LexType::COMMA,LexType::VarIdList} });
    Production.push_back({ LexType::ProcDec	, std::vector<LexType>{LexType::Empty} });
    Production.push_back({ LexType::ProcDec	, std::vector<LexType>{LexType::ProcDeclaration	} });
    Production.push_back({ LexType::ProcDeclaration	, std::vector<LexType>{LexType::PROCEDURE,LexType::ProcName,LexType::LPAREN,LexType::ParamList,LexType::RPAREN,LexType::SEMI,LexType::ProcDecPart,LexType::ProcBody,LexType::ProcDecMore} });
    Production.push_back({ LexType::ProcDecMore	, std::vector<LexType>{LexType::Empty} });
    Production.push_back({ LexType::ProcDecMore	, std::vector<LexType>{LexType::ProcDeclaration} });
    Production.push_back({ LexType::ProcName, std::vector<LexType>{LexType::ID} });
    Production.push_back({ LexType::ParamList	, std::vector<LexType>{LexType::Empty} });
    Production.push_back({ LexType::ParamList	, std::vector<LexType>{LexType::ParamDecList} });
    Production.push_back({ LexType::ParamDecList	, std::vector<LexType>{LexType::Param,LexType::ParamMore} });
    Production.push_back({ LexType::ParamMore	, std::vector<LexType>{LexType::Empty} });
    Production.push_back({ LexType::ParamMore	, std::vector<LexType>{LexType::SEMI,LexType::ParamDecList} });
    Production.push_back({ LexType::Param	, std::vector<LexType>{LexType::TypeName,LexType::FormList} });
    Production.push_back({ LexType::Param	, std::vector<LexType>{LexType::VAR,LexType::TypeName,LexType::FormList} });
    Production.push_back({ LexType::FormList, std::vector<LexType>{LexType::ID,LexType::FidMore} });
    Production.push_back({ LexType::FidMore, std::vector<LexType>{LexType::Empty} });
    Production.push_back({ LexType::FidMore, std::vector<LexType>{LexType::COMMA,LexType::FormList} });
    Production.push_back({ LexType::ProcDecPart, std::vector<LexType>{LexType::DeclarePart} });
    Production.push_back({ LexType::ProcBody, std::vector<LexType>{LexType::ProgramBody} });
    Production.push_back({ LexType::ProgramBody, std::vector<LexType>{LexType::BEGIN,LexType::StmList,LexType::END1} });
    Production.push_back({ LexType::StmList, std::vector<LexType>{LexType::Stm,LexType::StmMore} });
    Production.push_back({ LexType::StmMore, std::vector<LexType>{LexType::Empty} });
    Production.push_back({ LexType::StmMore, std::vector<LexType>{LexType::SEMI,LexType::StmList} });
    Production.push_back({ LexType::Stm, std::vector<LexType>{LexType::ConditionalStm} });
    Production.push_back({ LexType::Stm, std::vector<LexType>{LexType::LoopStm} });
    Production.push_back({ LexType::Stm, std::vector<LexType>{LexType::InputStm} });
    Production.push_back({ LexType::Stm, std::vector<LexType>{LexType::OutputStm} });
    Production.push_back({ LexType::Stm, std::vector<LexType>{LexType::ReturnStm} });
    Production.push_back({ LexType::Stm, std::vector<LexType>{LexType::ID,LexType::AssCall} });
    Production.push_back({ LexType::AssCall, std::vector<LexType>{LexType::AssignmentRest} });
    Production.push_back({ LexType::AssCall, std::vector<LexType>{LexType::CallStmRest} });
    Production.push_back({ LexType::AssignmentRest, std::vector<LexType>{LexType::VariMore,LexType::ASSIGN,LexType::Exp} });
    Production.push_back({ LexType::ConditionalStm, std::vector<LexType>{LexType::IF,LexType::RelExp,LexType::THEN,LexType::StmList,LexType::ELSE,LexType::StmList,LexType::FI} });
    Production.push_back({ LexType::LoopStm, std::vector<LexType>{LexType::WHILE,LexType::RelExp,LexType::DO,LexType::StmList,LexType::ENDWH} });
    Production.push_back({ LexType::InputStm, std::vector<LexType>{LexType::READ,LexType::LPAREN,LexType::InVar,LexType::RPAREN} });
    Production.push_back({ LexType::InVar, std::vector<LexType>{LexType::ID} });
    Production.push_back({ LexType::OutputStm, std::vector<LexType>{LexType::WRITE,LexType::LPAREN,LexType::Exp,LexType::RPAREN} });
    Production.push_back({ LexType::ReturnStm, std::vector<LexType>{LexType::RETURN1} });
    Production.push_back({ LexType::CallStmRest, std::vector<LexType>{LexType::LPAREN,LexType::ActParamList,LexType::RPAREN} });
    Production.push_back({ LexType::ActParamList, std::vector<LexType>{LexType::Empty} });
    Production.push_back({ LexType::ActParamList, std::vector<LexType>{LexType::Exp,LexType::ActParamMore} });
    Production.push_back({ LexType::ActParamMore, std::vector<LexType>{LexType::Empty} });
    Production.push_back({ LexType::ActParamMore, std::vector<LexType>{LexType::COMMA,LexType::ActParamList} });
    Production.push_back({ LexType::RelExp, std::vector<LexType>{LexType::Exp,LexType::OtherRelE} });
    Production.push_back({ LexType::OtherRelE, std::vector<LexType>{LexType::CmpOp,LexType::Exp} });
    Production.push_back({ LexType::Exp, std::vector<LexType>{LexType::Term,LexType::OtherTerm} });
    Production.push_back({ LexType::OtherTerm, std::vector<LexType>{LexType::Empty} });
    Production.push_back({ LexType::OtherTerm, std::vector<LexType>{LexType::AddOp,LexType::Exp} });
    Production.push_back({ LexType::Term, std::vector<LexType>{LexType::Factor,LexType::OtherFactor} });
    Production.push_back({ LexType::OtherFactor, std::vector<LexType>{LexType::Empty} });
    Production.push_back({ LexType::OtherFactor, std::vector<LexType>{LexType::MultOp,LexType::Term} });
    Production.push_back({ LexType::Factor, std::vector<LexType>{LexType::LPAREN,LexType::Exp,LexType::RPAREN} });
    Production.push_back({ LexType::Factor, std::vector<LexType>{LexType::INTC} });
    Production.push_back({ LexType::Factor, std::vector<LexType>{LexType::Variable} });
    Production.push_back({ LexType::Variable, std::vector<LexType>{LexType::ID,LexType::VariMore} });
    Production.push_back({ LexType::VariMore, std::vector<LexType>{LexType::Empty} });
    Production.push_back({ LexType::VariMore, std::vector<LexType>{LexType::LMIDPAREN,LexType::Exp,LexType::RMIDPAREN} });
    Production.push_back({ LexType::VariMore, std::vector<LexType>{LexType::DOT,LexType::FieldVar} });
    Production.push_back({ LexType::FieldVar, std::vector<LexType>{LexType::ID,LexType::FieldVarMore} });
    Production.push_back({ LexType::FieldVarMore, std::vector<LexType>{LexType::Empty} });
    Production.push_back({ LexType::FieldVarMore, std::vector<LexType>{LexType::LMIDPAREN,LexType::Exp,LexType::RMIDPAREN} });
    Production.push_back({ LexType::CmpOp, std::vector<LexType>{LexType::LT} });
    Production.push_back({ LexType::CmpOp, std::vector<LexType>{LexType::EQ} });
    Production.push_back({ LexType::AddOp, std::vector<LexType>{LexType::PLUS} });
    Production.push_back({ LexType::AddOp, std::vector<LexType>{LexType::MINUS} });
    Production.push_back({ LexType::MultOp, std::vector<LexType>{LexType::TIMES} });
    Production.push_back({ LexType::MultOp, std::vector<LexType>{LexType::OVER} });


}

void ParserLL1::get_mp() {
    //cout << Production.size() << endl;
    for (auto i = 0; i < Production.size(); i++) {
        mp[Production[i].first].push_back(i + 1);
        vector<LexType> tempVec = Production[i].second;
        for (int j = 0; j < tempVec.size(); j++)
        {
            if (NTSet.find(tempVec[j]) == NTSet.end()) continue;
            reverse_mp[tempVec[j]].push_back(i + 1);
        }
    }

}
//t为产生式左部
void ParserLL1::First(LexType t, set<LexType>& s) {
    if (TTSet.find(t) != TTSet.end()) {
        s.insert(t);
        return;
    }

    vector<int> ProductionId = mp[t];

    for (int i = 0; i < ProductionId.size(); i++) {
        int id = ProductionId[i] - 1; //104->>103
        LexType LEFT = Production[id].first;
        vector<LexType> RIGHT = Production[id].second;
        //每条产生式的LexType
        for (int rid = 0; rid < RIGHT.size(); rid++) {
            set<LexType> temppSet;
            First(RIGHT[rid], temppSet);

            auto t_it = temppSet.find(LexType::Empty);
            bool flag = false;//是否找完了
            if (rid != RIGHT.size() - 1 && t_it != temppSet.end()) {
                temppSet.erase(t_it);
            }
            else flag = true;//没有空，找完了

            //union
            s.insert(temppSet.begin(), temppSet.end());
            if (flag) break;
        }

    }
}

void ParserLL1::getFirst() {

    for (int i = 0, j = 0; i < Production.size();) {
        while (j < Production.size() && Production[i].first == Production[j].first)  j++;

        //[i,j)条产生式
        for (int k = i; k < j; k++) {
            //cout << k << endl;
            LexType LEFT = Production[k].first;
            vector<LexType> RIGHT = Production[k].second;
            set<LexType> tempSet;
            //每条产生式的LexType
            for (int rid = 0; rid < RIGHT.size(); rid++) {
                set<LexType> temppSet;
                First(RIGHT[rid], temppSet);
                auto t_it = temppSet.find(LexType::Empty);
                bool flag = false;//是否找完了
                if (rid != RIGHT.size() - 1 && t_it != temppSet.end()) {
                    temppSet.erase(t_it);
                }
                else flag = true;//没有空，找完了

                //union
                tempSet.insert(temppSet.begin(), temppSet.end());
                if (flag) break;
            }

            First_mp[LEFT].insert(tempSet.begin(), tempSet.end());
        }

        i = j;
    }

}

void ParserLL1::Follow(LexType t, set<LexType>& s, int cnt) {
    if (cnt > 20) return;
    if (TTSet.find(t) != TTSet.end()) {
        return;
    }
    vector<int> ProductionId = reverse_mp[t];

    for (int i = 0; i < ProductionId.size(); i++) {
        int id = ProductionId[i] - 1;
        LexType LEFT = Production[id].first;
        vector<LexType> tempVec = Production[id].second;
        int j = 0;
        if (t == LexType::StmList && LEFT == LexType::ConditionalStm) s.insert(LexType::FI);
        while (j < tempVec.size() && tempVec[j] != t) j++;
        j++;
        if (j == tempVec.size()) {
            set<LexType> temppSet;
            Follow(LEFT, temppSet, cnt + 1);
            s.insert(temppSet.begin(), temppSet.end());
        }

        for (; j < tempVec.size(); j++) {
            if (TTSet.find(tempVec[j]) != TTSet.end()) {
                s.insert(tempVec[j]);
                break;
            }
            else {
                bool  flag = true;
                set<LexType> temppSet = First_mp[tempVec[j]];
                auto it_erase = temppSet.find(LexType::Empty);
                if (it_erase != temppSet.end()) {
                    temppSet.erase(it_erase);
                    flag = false;
                    s.insert(temppSet.begin(), temppSet.end());
                    if (j + 1 == tempVec.size()) {
                        set<LexType> tempppSet;
                        Follow(LEFT, tempppSet, cnt + 1);
                        s.insert(tempppSet.begin(), tempppSet.end());
                        break;
                    }
                }
                else s.insert(temppSet.begin(), temppSet.end());
                if (flag) break;
            }
        }
    }
}

void ParserLL1::getFollow() {
    Follow_mp[LexType::Program].insert(LexType::Well);

    for (auto it = reverse_mp.begin(); it != reverse_mp.end(); it++) {
        //cout << it->second.size()<<"+++" << endl;
        LexType NT = (LexType)it->first;
        //vector<int> ProductionId = it->second;//id [1,104]
        set<LexType> tempSet;
        Follow(NT, tempSet, 0);
        Follow_mp[NT].insert(tempSet.begin(), tempSet.end());
        //cout << "-----" << endl;
    }


}

void ParserLL1::getPredict() {

    for (int i = 0; i < Production.size(); i++) {

        LexType LEFT = Production[i].first;
        vector<LexType> RIGHT = Production[i].second;
        set<LexType> tempSet;
        for (int rid = 0; rid < RIGHT.size(); rid++) {

            if (RIGHT[rid] == LexType::Empty) {
                tempSet.insert(Follow_mp[LEFT].begin(), Follow_mp[LEFT].end());
                break;
            }


            if (TTSet.find(RIGHT[rid]) != TTSet.end()   )
            {
                tempSet.insert(RIGHT[rid]);
                break;
            }
            else {
                int flag = true;
                set<LexType> temppSet = First_mp[RIGHT[rid]];
                auto it_erase = temppSet.find(LexType::Empty);
                if ( it_erase != temppSet.end()) {
                    flag = false;
                    temppSet.erase(it_erase);
                    tempSet.insert(temppSet.begin(), temppSet.end());
                    if (rid + 1 == RIGHT.size()) {
                        tempSet.insert(Follow_mp[RIGHT[rid]].begin(), Follow_mp[RIGHT[rid]].end());
                        break;
                    }
                }
                else	tempSet.insert(temppSet.begin(), temppSet.end());
                if (flag) break;
            }

        }

        //tempSet 转table
        for (auto item : tempSet) {
            table[{LEFT, item} ]=i+1;
        }

    }

}

/*
void ParserLL1::InitLL1Table()
{
    int i, j;

    for (i = 0; i < 104; i++)
        for (j = 0; j < 104; j++)
            LL1Table[i][j] = 0;

    //这个LL1分析表应该修改了课表的几个错误
    LL1Table[Program][PROGRAM] = 1;

    LL1Table[ProgramHead][PROGRAM] = 2;

    LL1Table[ProgramName][ID] = 3;

    LL1Table[DeclarePart][TYPE] = 4;
    LL1Table[DeclarePart][VAR] = 4;
    LL1Table[DeclarePart][PROCEDURE] = 4;
    LL1Table[DeclarePart][BEGIN] = 4;

    LL1Table[TypeDec][VAR] = 5;
    LL1Table[TypeDec][PROCEDURE] = 5;
    LL1Table[TypeDec][BEGIN] = 5;

    LL1Table[TypeDec][TYPE] = 6;

    LL1Table[TypeDeclaration][TYPE] = 7;

    LL1Table[TypeDecList][ID] = 8;

    LL1Table[TypeDecMore][VAR] = 9;
    LL1Table[TypeDecMore][PROCEDURE] = 9;
    LL1Table[TypeDecMore][BEGIN] = 9;


    LL1Table[TypeDecMore][ID] = 10;

    LL1Table[TypeId][ID] = 11;

    LL1Table[TypeName][INTEGER] = 12;
    LL1Table[TypeName][CHAR1] = 12;

    LL1Table[TypeName][ARRAY] = 13;
    LL1Table[TypeName][RECORD] = 13;

    LL1Table[TypeName][ID] = 14;

    LL1Table[BaseType][INTEGER] = 15;

    LL1Table[BaseType][CHAR1] = 16;

    LL1Table[StructureType][ARRAY] = 17;

    LL1Table[StructureType][RECORD] = 18;

    LL1Table[ArrayType][ARRAY] = 19;

    LL1Table[Low][INTC] = 20;

    LL1Table[Top][INTC] = 21;

    LL1Table[RecType][RECORD] = 22;

    LL1Table[FieldDecList][INTEGER] = 23;
    LL1Table[FieldDecList][CHAR1] = 23;

    LL1Table[FieldDecList][ARRAY] = 24;

    LL1Table[FieldDecMore][END1] = 25;

    LL1Table[FieldDecMore][INTEGER] = 26;
    LL1Table[FieldDecMore][CHAR1] = 26;
    LL1Table[FieldDecMore][ARRAY] = 26;

    LL1Table[IdList][ID] = 27;

    LL1Table[IdMore][SEMI] = 28;

    LL1Table[IdMore][COMMA] = 29;

    LL1Table[VarDec][PROCEDURE] = 30;
    LL1Table[VarDec][BEGIN] = 30;

    LL1Table[VarDec][VAR] = 31;

    LL1Table[VarDeclaration][VAR] = 32;

    LL1Table[VarDecList][INTEGER] = 33;
    LL1Table[VarDecList][CHAR1] = 33;
    LL1Table[VarDecList][ARRAY] = 33;
    LL1Table[VarDecList][RECORD] = 33;
    LL1Table[VarDecList][ID] = 33;

    LL1Table[VarDecMore][PROCEDURE] = 34;
    LL1Table[VarDecMore][BEGIN] = 34;


    LL1Table[VarDecMore][INTEGER] = 35;
    LL1Table[VarDecMore][CHAR1] = 35;
    LL1Table[VarDecMore][ARRAY] = 35;
    LL1Table[VarDecMore][RECORD] = 35;
    LL1Table[VarDecMore][ID] = 35;

    LL1Table[VarIdList][ID] = 36;

    LL1Table[VarIdMore][SEMI] = 37;

    LL1Table[VarIdMore][COMMA] = 38;

    LL1Table[ProcDec][BEGIN] = 39;

    LL1Table[ProcDec][PROCEDURE] = 40;

    LL1Table[ProcDeclaration][PROCEDURE] = 41;

    LL1Table[ProcDecMore][BEGIN] = 42;

    LL1Table[ProcDecMore][PROCEDURE] = 43;

    LL1Table[ProcName][ID] = 44;

    LL1Table[ParamList][RPAREN] = 45;

    LL1Table[ParamList][INTEGER] = 46;
    LL1Table[ParamList][CHAR1] = 46;
    LL1Table[ParamList][ARRAY] = 46;
    LL1Table[ParamList][RECORD] = 46;
    LL1Table[ParamList][ID] = 46;
    LL1Table[ParamList][VAR] = 46;

    LL1Table[ParamDecList][INTEGER] = 47;
    LL1Table[ParamDecList][CHAR1] = 47;
    LL1Table[ParamDecList][ARRAY] = 47;
    LL1Table[ParamDecList][RECORD] = 47;
    LL1Table[ParamDecList][ID] = 47;
    LL1Table[ParamDecList][VAR] = 47;

    LL1Table[ParamMore][RPAREN] = 48;

    LL1Table[ParamMore][SEMI] = 49;

    LL1Table[Param][INTEGER] = 50;
    LL1Table[Param][CHAR1] = 50;
    LL1Table[Param][ARRAY] = 50;
    LL1Table[Param][RECORD] = 50;
    LL1Table[Param][ID] = 50;

    LL1Table[Param][VAR] = 51;

    LL1Table[FormList][ID] = 52;

    LL1Table[FidMore][SEMI] = 53;
    LL1Table[FidMore][RPAREN] = 53;

    LL1Table[FidMore][COMMA] = 54;

    LL1Table[ProcDecPart][TYPE] = 55;
    LL1Table[ProcDecPart][VAR] = 55;
    LL1Table[ProcDecPart][PROCEDURE] = 55;
    LL1Table[ProcDecPart][BEGIN] = 55;

    LL1Table[ProcBody][BEGIN] = 56;

    LL1Table[ProgramBody][BEGIN] = 57;

    LL1Table[StmList][ID] = 58;
    LL1Table[StmList][IF] = 58;
    LL1Table[StmList][WHILE] = 58;
    LL1Table[StmList][RETURN1] = 58;
    LL1Table[StmList][READ] = 58;
    LL1Table[StmList][WRITE] = 58;

    LL1Table[StmMore][END1] = 59;
    LL1Table[StmMore][ENDWH] = 59;
    LL1Table[StmMore][ELSE] = 59;
    LL1Table[StmMore][FI] = 59;

    LL1Table[StmMore][SEMI] = 60;

    LL1Table[Stm][IF] = 61;

    LL1Table[Stm][WHILE] = 62;

    LL1Table[Stm][READ] = 63;

    LL1Table[Stm][WRITE] = 64;

    LL1Table[Stm][RETURN1] = 65;

    LL1Table[Stm][ID] = 66;

    LL1Table[AssCall][ASSIGN] = 67;
    LL1Table[AssCall][LMIDPAREN] = 67;
    LL1Table[AssCall][DOT] = 67;


    LL1Table[AssCall][LPAREN] = 68;

    LL1Table[AssignmentRest][ASSIGN] = 69;
    LL1Table[AssignmentRest][LMIDPAREN] = 69;
    LL1Table[AssignmentRest][DOT] = 69;

    LL1Table[ConditionalStm][IF] = 70;


    LL1Table[LoopStm][WHILE] = 71;

    LL1Table[InputStm][READ] = 72;

    LL1Table[InVar][ID] = 73;

    LL1Table[OutputStm][WRITE] = 74;

    LL1Table[ReturnStm][RETURN1] = 75;

    LL1Table[CallStmRest][LPAREN] = 76;

    LL1Table[ActParamList][RPAREN] = 77;

    LL1Table[ActParamList][ID] = 78;
    LL1Table[ActParamList][INTC] = 78;
    LL1Table[ActParamList][LPAREN] = 78;

    LL1Table[ActParamMore][RPAREN] = 79;

    LL1Table[ActParamMore][COMMA] = 80;

    LL1Table[RelExp][LPAREN] = 81;
    LL1Table[RelExp][INTC] = 81;
    LL1Table[RelExp][ID] = 81;

    LL1Table[OtherRelE][LT] = 82;
    LL1Table[OtherRelE][EQ] = 82;

    LL1Table[Exp][LPAREN] = 83;
    LL1Table[Exp][INTC] = 83;
    LL1Table[Exp][ID] = 83;

    LL1Table[OtherTerm][LT] = 84;
    LL1Table[OtherTerm][EQ] = 84;
    LL1Table[OtherTerm][THEN] = 84;
    LL1Table[OtherTerm][DO] = 84;
    LL1Table[OtherTerm][RPAREN] = 84;
    LL1Table[OtherTerm][END1] = 84;
    LL1Table[OtherTerm][SEMI] = 84;
    LL1Table[OtherTerm][COMMA] = 84;
    LL1Table[OtherTerm][ENDWH] = 84;
    LL1Table[OtherTerm][ELSE] = 84;
    LL1Table[OtherTerm][FI] = 84;
    LL1Table[OtherTerm][RMIDPAREN] = 84;


    LL1Table[OtherTerm][PLUS] = 85;
    LL1Table[OtherTerm][MINUS] = 85;

    LL1Table[Term][LPAREN] = 86;
    LL1Table[Term][INTC] = 86;
    LL1Table[Term][ID] = 86;

    LL1Table[OtherFactor][PLUS] = 87;
    LL1Table[OtherFactor][MINUS] = 87;
    LL1Table[OtherFactor][LT] = 87;
    LL1Table[OtherFactor][EQ] = 87;
    LL1Table[OtherFactor][THEN] = 87;
    LL1Table[OtherFactor][ELSE] = 87;
    LL1Table[OtherFactor][FI] = 87;
    LL1Table[OtherFactor][DO] = 87;
    LL1Table[OtherFactor][ENDWH] = 87;
    LL1Table[OtherFactor][RPAREN] = 87;
    LL1Table[OtherFactor][END1] = 87;
    LL1Table[OtherFactor][SEMI] = 87;
    LL1Table[OtherFactor][COMMA] = 87;
    LL1Table[OtherFactor][RMIDPAREN] = 87;

    LL1Table[OtherFactor][TIMES] = 88;
    LL1Table[OtherFactor][OVER] = 88;

    LL1Table[Factor][LPAREN] = 89;

    LL1Table[Factor][INTC] = 90;

    LL1Table[Factor][ID] = 91;

    LL1Table[Variable][ID] = 92;

    LL1Table[VariMore][ASSIGN] = 93;
    LL1Table[VariMore][TIMES] = 93;
    LL1Table[VariMore][OVER] = 93;
    LL1Table[VariMore][PLUS] = 93;
    LL1Table[VariMore][MINUS] = 93;
    LL1Table[VariMore][LT] = 93;
    LL1Table[VariMore][EQ] = 93;
    LL1Table[VariMore][THEN] = 93;
    LL1Table[VariMore][ELSE] = 93;
    LL1Table[VariMore][FI] = 93;
    LL1Table[VariMore][DO] = 93;
    LL1Table[VariMore][ENDWH] = 93;
    LL1Table[VariMore][RPAREN] = 93;
    LL1Table[VariMore][END1] = 93;
    LL1Table[VariMore][SEMI] = 93;
    LL1Table[VariMore][COMMA] = 93;
    LL1Table[VariMore][RMIDPAREN] = 93;

    LL1Table[VariMore][LMIDPAREN] = 94;

    LL1Table[VariMore][DOT] = 95;

    LL1Table[FieldVar][ID] = 96;

    LL1Table[FieldVarMore][ASSIGN] = 97;
    LL1Table[FieldVarMore][TIMES] = 97;
    LL1Table[FieldVarMore][OVER] = 97;
    LL1Table[FieldVarMore][PLUS] = 97;
    LL1Table[FieldVarMore][MINUS] = 97;
    LL1Table[FieldVarMore][LT] = 97;
    LL1Table[FieldVarMore][EQ] = 97;
    LL1Table[FieldVarMore][THEN] = 97;
    LL1Table[FieldVarMore][ELSE] = 97;
    LL1Table[FieldVarMore][FI] = 97;
    LL1Table[FieldVarMore][DO] = 97;
    LL1Table[FieldVarMore][ENDWH] = 97;
    LL1Table[FieldVarMore][RPAREN] = 97;
    LL1Table[FieldVarMore][END1] = 97;
    LL1Table[FieldVarMore][SEMI] = 97;
    LL1Table[FieldVarMore][COMMA] = 97;

    LL1Table[FieldVarMore][LMIDPAREN] = 98;

    LL1Table[CmpOp][LT] = 99;

    LL1Table[CmpOp][EQ] = 100;

    LL1Table[AddOp][PLUS] = 101;

    LL1Table[AddOp][MINUS] = 102;

    LL1Table[MultOp][TIMES] = 103;

    LL1Table[MultOp][OVER] = 104;

}
*/

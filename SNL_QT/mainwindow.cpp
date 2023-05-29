#pragma once
#include <QFileDialog>
#include <QMessageBox>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "global.h"
#include "lexical_analyse.h"
#include "syntax_analysis.h"
#include "RecuriveDescent.h"
#include "tree.h"
#include <QDebug>
#include"semanalyzer.h"
#include"symtable.h"
#include<iostream>
#include"kind2str.h"
using namespace ::std;

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    //ui->textEdit->setReadOnly(true);
    syntaxTree = nullptr;
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete syntaxTree;
    delete ui;
}


void MainWindow::on_pushButton_clicked() // 从文件中读取
{
    //定义文件对话框类
    QFileDialog* fileDialog = new QFileDialog(this);
    //定义文件对话框标题
    fileDialog->setWindowTitle(QStringLiteral("选中文件"));
    //设置默认文件路径
    // fileDialog->setDirectory(".");
    //设置文件过滤器
    fileDialog->setNameFilter(tr("文本文件(*.txt)"));
    //设置可以选择多个文件,默认为只能选择一个文件QFileDialog::ExistingFiles
    fileDialog->setFileMode(QFileDialog::ExistingFiles);
    //设置视图模式
    fileDialog->setViewMode(QFileDialog::Detail);
    //打印所有选择的文件的路径
    QStringList fileNames;
    if (fileDialog->exec())
    {
        fileNames = fileDialog->selectedFiles();
        file_name=fileNames.back();//返回QString
    }
    if (fileNames.size() == 0)
    {
        return;
    }
    QFile file(fileNames.back());
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QByteArray t = file.readAll();
    ui->textEdit->setText(QString(t));
    //ui->plainTextEdit->setPlainText(QString(t));
    file.close();
    //ui->textEdit->setReadOnly(true);
    ui->listWidget_2->clear();
    ui->listWidget->clear();
    ui->listWidget_3->clear();

}

void MainWindow::on_pushButton_2_clicked() // 进行词法分析
{
    Lexer* lexer = new Lexer();

    tokenList_vec.clear();
    INDEX = 0;
    ui->listWidget->clear();
    ui->listWidget_2->clear();
    ui->listWidget_3->clear();
    ui->treeWidget->clear();
    errorList.clear();
    //写入Edit
    QString filePath = "D:/SNL/edit.txt";
    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream stream(&file);
        stream << ui->textEdit->toPlainText();
        file.close();
    }



    lexer->run(file_name.toStdString());
    delete lexer;
    this->printLexer(); // 显示词法分析的结果
    this->printError();
}

void MainWindow::on_pushButton_3_clicked() // 进行语法分析
{
    if (INDEX == 0)
    {
        QMessageBox::about(nullptr, "提示", "尚未进行词法分析，无法进行语法分析");
        return;
    }
    //cout<<"index:"<<INDEX<<endl;
    //else if(INDEX==1){
        int selectedID = ui->comboBox->currentIndex();
        Parser* parser = nullptr;
        if (selectedID == 0)
        {
            parser = new ParserLL1(true); // LL1

        }
        else
        {
            parser = new ParserRecursiveDescent(); // 递归下降
        }
        if (this->syntaxTree != nullptr)
        {
         delete this->syntaxTree;
            this->syntaxTree = nullptr;
        }
        this->syntaxTree = parser->run(tokenList_vec);
        this->printParser(); // 显示语法分析的结果
        this->printError();

}

void MainWindow::on_pushButton_4_clicked()// 点击进行语义分析
{

    if(errorList.size() > 0){
        QString msg = QString("语法树错误或结构不完整,无法进行语义分析。");
        QMessageBox::critical(nullptr, "错误", msg);
        //exit(-1);
        //cout << "第" << tokenList[cur].line << "行出现语法错误" << error << endl;
        //exit(-1);
    }
    else{
        this->printError();
       //SemAnalyzer asemAnalyzer();
        SemAnalyzer asemAnalyzer(this->ui);
        symbtable* table = asemAnalyzer.Analyze(this->syntaxTree->getRoot());
        //Symtable::print(table);
        this->printSem(table);
    }
}


void MainWindow::printLexer() // 显示词法分析的结果
{
    ui->listWidget->clear();
    int tokenSize = INDEX;
    for (int i = 0; i < tokenSize; i++)
    {
        QString lineStr = "Line " + QString::number(tokenList[i].line);
        QString lexStr = lexName[tokenList[i].wd.second];
        QString semStr = QString::fromStdString(tokenList[i].wd.first);

        // 计算每个字段的长度
        int lineStrWidth = 25; // 假设第一个字段的最大长度为8
        int lexStrWidth = 35; // 假设第二个字段的最大长度为10
        int semStrWidth = 15;

        // 格式化字符串，使用空格填充来实现对齐
        QString format = lineStr+"\t"+lexStr+"\t\t"+semStr;
        //format = format.arg(lineStr.leftJustified(lineStrWidth, ' '));
        //format = format.arg(lexStr.leftJustified(lexStrWidth, ' '));
        //format = format.arg(semStr).leftJustified(semStrWidth, ' ');

        // 添加到列表中
        ui->listWidget->addItem(format);
    }
}
void MainWindow::printError(){
    ui->listWidget_2->clear();
    for(int i = 0 ;i< errorList.size();i++){
        ui->listWidget_2->addItem(errorList[i]);
    }

}
void MainWindow::printParser() // 显示语法分析的结果
{
    ui->treeWidget->clear();
    TreeNode* root = this->syntaxTree->getRoot();
    QTreeWidgetItem* topItem = new QTreeWidgetItem(ui->treeWidget);
    topItem->setExpanded(true);

    QString str = nodekindName[root->nodekind];
    topItem->setText(0, str);
    for (int i = 0; i < 3; i++)
    {
        if (root->child[i] == nullptr)
        {
            continue;
        }
        this->preOrder(root->child[i], topItem);
    }

}

void MainWindow::printSem(symbtable* table,int level)//显示语义分析结果
{
        symbtable* head = table;

        while (head&& head->next)
        {
            AttributeIR& at = head->attrIR;
            //show
            QString nameStr = "[Id_Name] " + QString::fromStdString(head->name.c_str())+"\n";
            QString kdStr="[Id_Kind] " + QString::fromStdString(IdKind2str[at.kind].c_str());
            QString content1=nameStr+kdStr;
            ui->listWidget_3->addItem(content1);
            //QString format="%1\t%2\t%3";
            //format+=format.arg(nameStr);
            //format+=format.arg(kdStr);
            QString more(QString::null);
            if (at.kind == IdKind::typeKind) {
                //类型标识符
                //more=QString::fromStdString(typeKindShow(at.idtype));
                more=typeKindShow(at.idtype);
                ui->listWidget_3->addItem(more);
            }
            else if (at.kind == IdKind::varKind||at.kind == IdKind::arrayNameKind) {
                //变量标识符
                more= "[Level]: " +QString::fromStdString(to_string(at.More.VarAttr.level))+"\n";
                more+=varKindShow(at);
                ui->listWidget_3->addItem(more);
            }
            else {
                //过函标识符
                more="[Level]: " +QString::fromStdString(to_string(at.More.ProcAttr.level))+"\n\n";
                ui->listWidget_3->addItem(more);
                printSem(head->nextLevel);
            }

            //format+=more;

            //QString content=nameStr+"\n"+kdStr+"\n" +more+ "\n";

            // 添加到列表中
            //ui->listWidget_3->addItem(content);
            head = head->next;
        }
}

QString GetKind(TreeNode * node){
    QString res ="";
    if(node->nodekind ==NodeKind::DecK){
        if(node->kind.dec == DecKind::IdK) return "IdK";
        if(node->kind.dec == DecKind::CharK) return "CharK";
        if(node->kind.dec == DecKind::IntegerK) return "IntegerK";
        if(node->kind.dec == DecKind::RecordK) return "RecordK";
        if(node->kind.dec == DecKind::ArrayK) return "ArrayK";
    }
    else if(node->nodekind == NodeKind::StmtK){
        if(node->kind.stmt == StmtKind::IfK) return "IfK";
        if(node->kind.stmt == StmtKind::WhileK) return "WhileK";
        if(node->kind.stmt == StmtKind::AssignK) return "AssignK";
        if(node->kind.stmt == StmtKind::ReadK) return "ReadK";
        if(node->kind.stmt == StmtKind::WriteK) return "WriteK";
        if(node->kind.stmt == StmtKind::CallK) return "CallK";
        if(node->kind.stmt == StmtKind::ReturnK) return "ReturnK";

    }
    else if(node->nodekind == NodeKind::ExpK){
        if(node->kind.exp == ExpKind::OpK) return "OpK";
        if(node->kind.exp == ExpKind::ConstK) return "ConstK";
        if(node->kind.exp == ExpKind::VariK) return "VariK";
    }
    return res;
}


void MainWindow::preOrder(TreeNode* node, QTreeWidgetItem* parentItem)
{
    QTreeWidgetItem* nowItem = new QTreeWidgetItem(parentItem);
    nowItem->setExpanded(true);
    if (node->sibling != nullptr)
    {
        this->preOrder(node->sibling, parentItem);
    }
    QString str =nodekindName[node->nodekind]+" "+GetKind(node);

    // 以下用于显示标识符
    //if(str == "ExpK"&&node->name[0]=="") return ;
    for (int i = 0; i < 10; i++)
    {
        if (node->name[i] != "")
        {
            if (i != 0)
            {
                str += ",";
            }
            else
            {
                str += ":";
            }
            str += QString::fromStdString(node->name[i]);
        }
    }

    nowItem->setText(0, str);
    for (int i = 0; i < 3; i++)
    {
        if (node->child[i] == nullptr)
        {
            continue;
        }
        //if("ExpK"==nodekindName[node->child[i]->nodekind]&&node->child[i]->name[0]=="") continue;
        this->preOrder(node->child[i], nowItem);
    }
}


////语义分析需要的函数：
QString MainWindow::typeKindShow(typeIR* tr) {
    QString ans;
    if (!tr)
        return ans;
    ans =ans + "[TypeKind]: " + QString::fromStdString(TypeKind2str[tr->kind]) + "\n[TypeSize]: " + QString::fromStdString(to_string(tr->size)) + "\n";
    if (tr->kind == TypeKind::arrayTy) {
        //printf("[indexTy]:\n");
        ans += "[indexTy]:\n";
        ans += typeKindShow(tr->More.ArrayAttr.indexTy);
        //printf("[elemTy]:\n");
        ans += "[elemTy]:\n";
        ans += typeKindShow(tr->More.ArrayAttr.elemTy);
    }
    else if (tr->kind == TypeKind::recordTy) {
        fieldChain* head = tr->More.vody;
        while (head)
        {
            ans =ans + QString::fromStdString(head->idname) + ":";
            ans += typeKindShow(head->unitType);
            head = head->next;
        }
    }
    return ans;
}

QString MainWindow::varKindShow(AttributeIR& at) {
    QString ans;
    ans += "[AccessKind]:";
    if (at.More.VarAttr.access == AccessKind::dir)
        ans += "dir\n";
    else
        ans += "indir\n";
    ans+= "[Off]: " + QString::fromStdString(to_string(at.More.VarAttr.off)) + "\n";
    return ans;
}

QString MainWindow::procKindShow(AttributeIR& at) {
    QString ans="[Level]: " +QString::number(at.More.ProcAttr.level)+ "\n";
    ans+="[Level]: " + QString::number(at.More.ProcAttr.level)+ "\n";
    return ans;
}




























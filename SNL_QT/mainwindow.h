#pragma once
#include"symtable.h"
#include <QMainWindow>
#include<QString>

class SyntaxTree;
class TreeNode;
class QTreeWidgetItem;
class SemAnalyzer;

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    //语义分析会使用到
    //ui变为共有
    Ui::MainWindow* ui;
    SemAnalyzer* semanalyzer;
    QString typeKindShow(typeIR* tr);
    QString varKindShow(AttributeIR& at);
    QString procKindShow(AttributeIR& at);
    //

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();
    void printLexer();//打印词法分析结果
    void printParser();//打印语法分析结果
    void printError();//打印报错
    void printSem(symbtable* table,int level = 0);



private:
    QString file_name;
    SyntaxTree* syntaxTree;
    void preOrder(TreeNode* node, QTreeWidgetItem* parentItem);
};






































#define _CRT_SECURE_NO_WARNINGS
#pragma once
#include "mainwindow.h"
#include <QApplication>


#include "global.h"
#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <string>

#include "tree.h"
#include "global.h"
#include "lexical_analyse.h"
#include "syntax_analysis.h"
#include "semantic_analysis.h"
#include"RecuriveDescent.h"
using namespace std;



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}

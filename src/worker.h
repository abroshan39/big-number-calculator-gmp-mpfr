/*
    [Big Number Calculator]
    Publisher: Rosybit
    Url: http://www.rosybit.com
    GitHub: https://github.com/abroshan39/big-number-calculator-gmp-mpfr
    Author: Aboutaleb Roshan [ab.roshan39@gmail.com]
    License: MIT License
*/

#ifndef WORKER_H
#define WORKER_H

#include <QObject>
#include "common_functions.h"

class Worker : public QObject
{
    Q_OBJECT

public:
    Worker(AppMode appMode, NumType numType, OpType opType, const QString &n1Str, int n1Base, const QString &n2Str, int n2Base, int resultBase, long int precision, RoundMode roundMode);

signals:
    void finished(const QString &);

public slots:
    void process();

private:
    AppMode appMode;
    NumType numType;
    OpType opType;
    RoundMode roundMode;
    QString n1Str;
    QString n2Str;
    long int precision;
    int n1Base;
    int n2Base;
    int resultBase;
};
#endif // WORKER_H

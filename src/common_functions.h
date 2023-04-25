/*
    [Big Number Calculator]
    Publisher: Rosybit
    Url: http://www.rosybit.com
    GitHub: https://github.com/abroshan39/big-number-calculator-gmp-mpfr
    Author: Aboutaleb Roshan [ab.roshan39@gmail.com]
    License: MIT License
*/

#ifndef COMMON_FUNCTIONS_H
#define COMMON_FUNCTIONS_H

#include <cstdlib>
#include <cmath>

#include <QGuiApplication>
#include <QApplication>

#include <QDesktopServices>
#include <QStyle>
#include <QScreen>
#include <QPalette>
#include <QColor>

#include <QLabel>
#include <QTextDocumentFragment>
#include <QTextStream>
#if QT_VERSION >= 0x060000
#include <QStringConverter>
#else
#include <QTextCodec>
#endif
#include <QRegularExpression>
#include <QString>
#include <QSize>

#include <QMessageBox>
#include <QDebug>

enum AppMode
{
    Calc, BaseConv
};

enum NumType
{
    Integer, FloatingPoint
};

enum OpType
{
    Add, Sub, Mul, Div, Mod, Pow, Root
};

enum RoundMode
{
    RNDN, RNDD, RNDU, RNDZ, RNDA
};

QString &insert_dec_sep(QString &num, long int exp);
void linkLabelEnabled(QLabel *label, bool enabled, bool call_setEnabled_slot = true);
double calculateScreenRatio();

#endif // COMMON_FUNCTIONS_H

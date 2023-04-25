/*
    [Big Number Calculator]
    Publisher: Rosybit
    Url: http://www.rosybit.com
    GitHub: https://github.com/abroshan39/big-number-calculator-gmp-mpfr
    Author: Aboutaleb Roshan [ab.roshan39@gmail.com]
    License: MIT License
*/

#ifndef WIDGETSREIMPLEMENT_H
#define WIDGETSREIMPLEMENT_H

// Reimplementation of Widgets

#include <QTextEdit>
#include <QEvent>
#include <QKeyEvent>
#include <QDragEnterEvent>
#include <QDropEvent>

class MyTextEdit : public QTextEdit
{
    Q_OBJECT

public:
    MyTextEdit(QWidget *parent = nullptr) : QTextEdit(parent) {;}
    ~MyTextEdit() {;}

signals:
    void enterKeyPressedIgnored();
    void dragEntered(QDragEnterEvent *);
    void dragLeaved(QDragLeaveEvent *);
    void dropped(QDropEvent *);

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragLeaveEvent(QDragLeaveEvent *event) override;
    void dropEvent(QDropEvent *event) override;
};
#endif // WIDGETSREIMPLEMENT_H

/*
    [Big Number Calculator]
    Publisher: Rosybit
    Url: http://www.rosybit.com
    GitHub: https://github.com/abroshan39/big-number-calculator-gmp-mpfr
    Author: Aboutaleb Roshan [ab.roshan39@gmail.com]
    License: MIT License
*/

#include "widgets_reimplement.h"

void MyTextEdit::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter)
    {
        event->ignore();
        emit enterKeyPressedIgnored();
    }
    else
    {
        QTextEdit::keyPressEvent(event);
    }
}

void MyTextEdit::dragEnterEvent(QDragEnterEvent *event)
{
    QTextEdit::dragEnterEvent(event);
    emit dragEntered(event);
}

void MyTextEdit::dragLeaveEvent(QDragLeaveEvent *event)
{
    QTextEdit::dragLeaveEvent(event);
    emit dragLeaved(event);
}

void MyTextEdit::dropEvent(QDropEvent *event)
{
    QTextEdit::dropEvent(event);
    emit dropped(event);
}

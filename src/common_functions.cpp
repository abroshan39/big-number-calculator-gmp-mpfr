/*
    [Big Number Calculator]
    Publisher: Rosybit
    Url: http://www.rosybit.com
    GitHub: https://github.com/abroshan39/big-number-calculator-gmp-mpfr
    Author: Aboutaleb Roshan [ab.roshan39@gmail.com]
    License: MIT License
*/

#include "common_functions.h"

QString &insert_dec_sep(QString &num, long int exp)
{
    int first_digit_index = num.indexOf(QRegularExpression("[0-9A-Za-z]"));
    if(first_digit_index + exp >= num.size())
        return num;
    if(exp > 0)
        num.insert(first_digit_index + exp, ".");
    else
        num.insert(first_digit_index, QString("0.%1").arg(QString("0").repeated(abs((int)exp))));
    return num;
}

void linkLabelEnabled(QLabel *label, bool enabled, bool call_setEnabled_slot)
{
    QString str;
    QString text(QTextDocumentFragment::fromHtml(label->text()).toPlainText());
    QString colorStr;
    QColor color;

    str = "<!DOCTYPE html><html><head><style type=\"text/css\">\n";
    str += "a {Color: %1;}\n";
    str += "</style></head><body>\n";
    str += "<p><a href=\"null\">%2</a></p></body></html>";

    if(enabled)
        color = QLabel().palette().color(QPalette::Text);
    else
        color = QLabel().palette().color(QPalette::Disabled, QPalette::Text);
    colorStr = QString("rgb(%1,%2,%3)").arg(color.red()).arg(color.green()).arg(color.blue());

    label->setText(QString(str).arg(colorStr, text));
    if(call_setEnabled_slot)
        label->setEnabled(enabled);
}

double calculateScreenRatio()
{
    QSize s_size(QGuiApplication::primaryScreen()->size());
    double x, y, result;
    x = (double)s_size.width() / 1366;
    y = (double)s_size.height() / 768;
    result = x > y ? y : x;
    return result > 1 ? result : 1;
}

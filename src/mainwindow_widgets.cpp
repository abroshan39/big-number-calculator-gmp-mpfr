/*
    [Big Number Calculator]
    Publisher: Rosybit
    Url: http://www.rosybit.com
    GitHub: https://github.com/abroshan39/big-number-calculator-gmp-mpfr
    Author: Aboutaleb Roshan [ab.roshan39@gmail.com]
    License: MIT License
*/

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "common_functions.h"
#include "gmp.h"
#include "mpfr.h"

#include <QFile>
#include <QDir>
#include <QFileDialog>
#include <QMimeData>
#include <QUrl>

#include <QMessageBox>
#include <QDebug>

void MainWindow::on_btnCalculate_clicked()
{
    calculate();
}

void MainWindow::on_btnExit_clicked()
{
    QCoreApplication::quit();
}

void MainWindow::on_radioButtonCalculator_clicked()
{
    ui->frameOp->setEnabled(true);
    ui->labelN2->setEnabled(true);
    ui->labelB2->setEnabled(true);
    ui->labelN2Name->setEnabled(true);
    ui->spinBoxN2->setEnabled(true);
    ui->textEditN2->setEnabled(true);
    linkLabelEnabled(ui->labelN2File, true);
    linkLabelEnabled(ui->labelN2FileClear, true);
}

void MainWindow::on_radioButtonBaseConv_clicked()
{
    ui->frameOp->setEnabled(false);
    ui->labelN2->setEnabled(false);
    ui->labelB2->setEnabled(false);
    ui->labelN2Name->setEnabled(false);
    ui->spinBoxN2->setEnabled(false);
    ui->textEditN2->setEnabled(false);
    linkLabelEnabled(ui->labelN2File, false);
    linkLabelEnabled(ui->labelN2FileClear, false);
}

void MainWindow::on_radioButtonInteger_clicked()
{
    ui->groupBoxPrec->setEnabled(false);
    ui->groupBoxRounding->setEnabled(false);
    linkLabelEnabled(ui->labelPrecDefault, false, false);
    linkLabelEnabled(ui->labelPrecMin, false, false);
    linkLabelEnabled(ui->labelPrecMax, false, false);
}

void MainWindow::on_radioButtonFloat_clicked()
{
    ui->groupBoxPrec->setEnabled(true);
    ui->groupBoxRounding->setEnabled(true);
    linkLabelEnabled(ui->labelPrecDefault, true, false);
    linkLabelEnabled(ui->labelPrecMin, true, false);
    linkLabelEnabled(ui->labelPrecMax, true, false);
}

void MainWindow::on_labelPrecDefault_linkActivated(const QString &link)
{
    Q_UNUSED(link);  // (void)link;
    ui->lineEditPrec->setText(QString::number(1024));
}

void MainWindow::on_labelPrecMin_linkActivated(const QString &link)
{
    Q_UNUSED(link);  // (void)link;
    ui->lineEditPrec->setText(QString::number(MPFR_PREC_MIN));
}

void MainWindow::on_labelPrecMax_linkActivated(const QString &link)
{
    Q_UNUSED(link);  // (void)link;
    ui->lineEditPrec->setText(QString::number(MPFR_PREC_MAX));
}

void MainWindow::on_comboBoxFont_currentIndexChanged(int index)
{
    FontNameIndex fontNameIndex = static_cast<FontNameIndex>(ui->comboBoxFont->itemData(index, Qt::UserRole).toInt());
    if(fontNameIndex == OpenSans)
        setFont(QFont("Open Sans", ui->spinBoxFontSize->value(), QFont::Normal));
    if(fontNameIndex == OpenSansMedium)
        setFont(QFont("Open Sans Medium", ui->spinBoxFontSize->value(), QFont::Medium));
    else if(fontNameIndex == SourceCodePro)
        setFont(QFont("Source Code Pro", ui->spinBoxFontSize->value(), QFont::Normal));
}

void MainWindow::on_spinBoxFontSize_valueChanged(int arg1)
{
    FontNameIndex fontNameIndex = static_cast<FontNameIndex>(ui->comboBoxFont->currentData(Qt::UserRole).toInt());
    if(fontNameIndex == OpenSans)
        setFont(QFont("Open Sans", arg1, QFont::Normal));
    else if(fontNameIndex == OpenSansMedium)
        setFont(QFont("Open Sans Medium", arg1, QFont::Medium));
    else if(fontNameIndex == SourceCodePro)
        setFont(QFont("Source Code Pro", arg1, QFont::Normal));
}

void MainWindow::on_checkBoxResultFile_clicked(bool checked)
{
    ui->textBrowserR->setEnabled(!checked);
    ui->textBrowserR->clear();
}

void MainWindow::on_labelAbout_linkActivated(const QString &link)
{
    Q_UNUSED(link);  // (void)link;
    QMessageBox::information(this, "About", "Big Number Calculator v1.0\nWritten by Aboutaleb Roshan [Email: ab.roshan39@gmail.com]\nGitHub: https://github.com/abroshan39");
}

void MainWindow::on_spinBoxN1_valueChanged(int arg1)
{
    labelNumName(ui->labelN1Name, arg1);
    if(ui->radioButtonBaseConv->isChecked())
        on_btnCalculate_clicked();
}

void MainWindow::on_spinBoxN2_valueChanged(int arg1)
{
    labelNumName(ui->labelN2Name, arg1);
}

void MainWindow::on_spinBoxR_valueChanged(int arg1)
{
    checkBoxUppercaseShowHide(arg1);
    labelNumName(ui->labelRName, arg1);
    if(ui->radioButtonBaseConv->isChecked())
        on_btnCalculate_clicked();
}

void MainWindow::on_labelN1File_linkActivated(const QString &link)
{
    Q_UNUSED(link);  // (void)link;
    if(!isProcessing)
    {
        QString file_path = QFileDialog::getOpenFileName(this, "Open", QDir::homePath(), "Text files (*.txt);;All files (*)");
        if(!file_path.isEmpty())
        {
            n1_file_path = file_path;
            ui->textEditN1->setText(QString("File Path:\n%1").arg(QDir::toNativeSeparators(file_path)));
            ui->textEditN1->setReadOnly(true);
        }
    }
}

void MainWindow::on_labelN2File_linkActivated(const QString &link)
{
    Q_UNUSED(link);  // (void)link;
    if(!isProcessing && ui->textEditN2->isEnabled())
    {
        QString file_path = QFileDialog::getOpenFileName(this, "Open", QDir::homePath(), "Text files (*.txt);;All files (*)");
        if(!file_path.isEmpty())
        {
            n2_file_path = file_path;
            ui->textEditN2->setText(QString("File Path:\n%1").arg(QDir::toNativeSeparators(file_path)));
            ui->textEditN2->setReadOnly(true);
        }
    }
}

void MainWindow::on_labelN1FileClear_linkActivated(const QString &link)
{
    Q_UNUSED(link);  // (void)link;
    if(!isProcessing && !n1_file_path.isEmpty())
    {
        n1_file_path.clear();
        ui->textEditN1->clear();
        ui->textEditN1->setReadOnly(false);
    }
}

void MainWindow::on_labelN2FileClear_linkActivated(const QString &link)
{
    Q_UNUSED(link);  // (void)link;
    if(!isProcessing && ui->textEditN2->isEnabled() && !n2_file_path.isEmpty())
    {
        n2_file_path.clear();
        ui->textEditN2->clear();
        ui->textEditN2->setReadOnly(false);
    }
}

void MainWindow::on_textEditN1_dragEntered(QDragEnterEvent *e)
{
    if(!isProcessing)
    {
        ui->textEditN1->setReadOnly(false);
        if(e->mimeData()->hasUrls())
            e->acceptProposedAction();
    }
}

void MainWindow::on_textEditN1_dragLeaved(QDragLeaveEvent *e)
{
    Q_UNUSED(e);  // (void)e;
    if(!isProcessing)
    {
        if(!n1_file_path.isEmpty())
            ui->textEditN1->setReadOnly(true);
    }
}

void MainWindow::on_textEditN1_dropped(QDropEvent *e)
{
    QString dropFilePath;
    for(int i = 0; i < e->mimeData()->urls().count(); i++)
    {
        dropFilePath = e->mimeData()->urls().at(i).toLocalFile();
        break;  // Only first item
    }

    if(QFileInfo(dropFilePath).isFile())
    {
        n1_file_path = dropFilePath;
        ui->textEditN1->setText(QString("File Path:\n%1").arg(QDir::toNativeSeparators(dropFilePath)));
        ui->textEditN1->setReadOnly(true);
    }
}

void MainWindow::on_textEditN2_dragEntered(QDragEnterEvent *e)
{
    if(!isProcessing)
    {
        ui->textEditN2->setReadOnly(false);
        if(e->mimeData()->hasUrls())
            e->acceptProposedAction();
    }
}

void MainWindow::on_textEditN2_dragLeaved(QDragLeaveEvent *e)
{
    Q_UNUSED(e);  // (void)e;
    if(!isProcessing)
    {
        if(!n2_file_path.isEmpty())
            ui->textEditN2->setReadOnly(true);
    }
}

void MainWindow::on_textEditN2_dropped(QDropEvent *e)
{
    QString dropFilePath;
    for(int i = 0; i < e->mimeData()->urls().count(); i++)
    {
        dropFilePath = e->mimeData()->urls().at(i).toLocalFile();
        break;  // Only first item
    }

    if(QFileInfo(dropFilePath).isFile())
    {
        n2_file_path = dropFilePath;
        ui->textEditN2->setText(QString("File Path:\n%1").arg(QDir::toNativeSeparators(dropFilePath)));
        ui->textEditN2->setReadOnly(true);
    }
}

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

#include "worker.h"

#include <QRegularExpression>
#include <QKeyEvent>
#include <QThread>
#include <QFontDatabase>
#include <QFont>
#include <QIcon>
#include <QStyle>
#include <QScreen>
#include <QFile>
#include <QDir>
#include <QFileDialog>
#include <QDate>

#include <QMessageBox>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QFontDatabase::addApplicationFont(":/files/fonts/OpenSans-Regular.ttf");
    QFontDatabase::addApplicationFont(":/files/fonts/OpenSans-Medium.ttf");
    QFontDatabase::addApplicationFont(":/files/fonts/SourceCodePro-Regular.ttf");
    double screenRatio = calculateScreenRatio();
    resize((int)(900 * screenRatio), (int)(600 * screenRatio));
    setFont(QFont("Open Sans", ui->spinBoxFontSize->value(), QFont::Normal));
    setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, size(), QGuiApplication::primaryScreen()->availableGeometry()));
    setWindowTitle("Big Number Calculator");
    setWindowIcon(QIcon(":/files/images/hash-icon.svg"));
    setAcceptDrops(true);
    on_radioButtonInteger_clicked();

    ui->progressBar->hide();
    ui->comboBoxFont->addItem("Open Sans", OpenSans);
    ui->comboBoxFont->addItem("Open Sans Medium", OpenSansMedium);
    ui->comboBoxFont->addItem("Source Code Pro", SourceCodePro);

    checkBoxUppercaseShowHide(ui->spinBoxR->value());
    labelNumName(ui->labelN1Name, ui->spinBoxN1->value());
    labelNumName(ui->labelN2Name, ui->spinBoxN2->value());
    labelNumName(ui->labelRName, ui->spinBoxR->value());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent *e)
{
    if(e->key() == Qt::Key_Return || e->key() == Qt::Key_Enter)
        on_btnCalculate_clicked();
    else if(e->key() == Qt::Key_Escape)
        close();
}

void MainWindow::closeEvent(QCloseEvent *e)
{
    if(isProcessing)
    {
        QMessageBox::warning(this, "Warning", "Processing, please wait...");
        e->ignore();
        return;
    }

    QWidget::closeEvent(e);
}

void MainWindow::calculate()
{
    if(isProcessing)
    {
        QMessageBox::warning(this, "Warning", "Processing, please wait...");
        return;
    }

    AppMode appMode = AppMode::Calc;
    NumType numType = NumType::Integer;
    OpType opType = OpType::Add;
    RoundMode roundMode = RoundMode::RNDN;
    long int precision = 1024;

    if(ui->radioButtonCalculator->isChecked())
        appMode = AppMode::Calc;
    else if(ui->radioButtonBaseConv->isChecked())
        appMode = AppMode::BaseConv;

    if(ui->radioButtonInteger->isChecked())
        numType = NumType::Integer;
    else if(ui->radioButtonFloat->isChecked())
        numType = NumType::FloatingPoint;

    if(ui->radioButtonAdd->isChecked())
        opType = OpType::Add;
    else if(ui->radioButtonSub->isChecked())
        opType = OpType::Sub;
    else if(ui->radioButtonMul->isChecked())
        opType = OpType::Mul;
    else if(ui->radioButtonDiv->isChecked())
        opType = OpType::Div;
    else if(ui->radioButtonMod->isChecked())
        opType = OpType::Mod;
    else if(ui->radioButtonPow->isChecked())
        opType = OpType::Pow;
    else if(ui->radioButtonRoot->isChecked())
        opType = OpType::Root;

    if(ui->radioButtonRNDN->isChecked())
        roundMode = RoundMode::RNDN;
    else if(ui->radioButtonRNDD->isChecked())
        roundMode = RoundMode::RNDD;
    else if(ui->radioButtonRNDU->isChecked())
        roundMode = RoundMode::RNDU;
    else if(ui->radioButtonRNDZ->isChecked())
        roundMode = RoundMode::RNDZ;
    else if(ui->radioButtonRNDA->isChecked())
        roundMode = RoundMode::RNDA;

    if(ui->lineEditPrec->text().contains(QRegularExpression("[^0-9]")))
        ui->lineEditPrec->setText(QString::number(precision));
    else
        precision = ui->lineEditPrec->text().toLong();

    QString n1File, n2File;

    if(!n1_file_path.isEmpty())
    {
        QFile file(n1_file_path);
        file.open(QIODevice::ReadOnly);
        n1File = file.readAll();
        file.close();
    }

    if(!n2_file_path.isEmpty())
    {
        QFile file(n2_file_path);
        file.open(QIODevice::ReadOnly);
        n2File = file.readAll();
        file.close();
    }

    int resultBase = ui->spinBoxR->value();
    if(resultBase >= 11 && resultBase <= 36 && ui->checkBoxUppercase->isChecked())
        resultBase *= -1;

    Worker *worker = new Worker(appMode, numType, opType, (n1_file_path.isEmpty() ? ui->textEditN1->toPlainText() : n1File), ui->spinBoxN1->value(), (n2_file_path.isEmpty() ? ui->textEditN2->toPlainText() : n2File), ui->spinBoxN2->value(), resultBase, precision, roundMode);
    QThread *thread = new QThread;
    worker->moveToThread(thread);

    connect(thread, &QThread::started, worker, &Worker::process);
    connect(worker, &Worker::finished, this, &MainWindow::threadFinished);
    connect(worker, &Worker::finished, thread, &QThread::quit);
    connect(worker, &Worker::finished, worker, &Worker::deleteLater);
    connect(thread, &QThread::finished, thread, &QThread::deleteLater);

    isProcessing = true;
    widgetsDeactived(true);
    ui->progressBar->show();
    thread->start();
}

void MainWindow::threadFinished(const QString &str)
{
    isProcessing = false;
    widgetsDeactived(false);
    ui->progressBar->hide();

    if(ui->checkBoxResultFile->isChecked())
    {
        QString filter = "Text files (*.txt);;All files (*)";
        QString file_path = QFileDialog::getSaveFileName(this, "Save As", QDir::homePath() + "/BigNumCalc_" + QDate::currentDate().toString("yyyy-MM-dd") + "_" + QTime::currentTime().toString("HHmmss") + ".txt", filter);
        if(!file_path.isEmpty())
        {
            QFile file(file_path);
            QTextStream out(&file);
            file.open(QIODevice::WriteOnly | QIODevice::Truncate);
#if QT_VERSION >= 0x060000
            out.setEncoding(QStringConverter::Utf8);
#else
            out.setCodec(QTextCodec::codecForName("UTF-8"));
#endif
            out << str;
            file.close();
        }
    }
    else
    {
        ui->textBrowserR->setText(str);
    }
}

void MainWindow::widgetsDeactived(bool b)
{
    ui->spinBoxN1->setReadOnly(b);
    ui->spinBoxN2->setReadOnly(b);
    ui->spinBoxR->setReadOnly(b);
    ui->frameFontFile->setEnabled(!b);
    ui->groupBoxMode->setEnabled(!b);
    ui->groupBoxNumberType->setEnabled(!b);

    if(b)
    {
        ui->textEditN1->setReadOnly(true);
        ui->textEditN2->setReadOnly(true);
        ui->frameOp->setEnabled(false);
        ui->groupBoxPrec->setEnabled(false);
        ui->groupBoxRounding->setEnabled(false);
        linkLabelEnabled(ui->labelPrecDefault, false, false);
        linkLabelEnabled(ui->labelPrecMin, false, false);
        linkLabelEnabled(ui->labelPrecMax, false, false);
    }
    else
    {
        if(ui->radioButtonCalculator->isChecked())
            on_radioButtonCalculator_clicked();
        else if(ui->radioButtonBaseConv->isChecked())
            on_radioButtonBaseConv_clicked();

        if(ui->radioButtonInteger->isChecked())
            on_radioButtonInteger_clicked();
        else if(ui->radioButtonFloat->isChecked())
            on_radioButtonFloat_clicked();

        if(n1_file_path.isEmpty())
            ui->textEditN1->setReadOnly(false);

        if(n2_file_path.isEmpty())
            ui->textEditN2->setReadOnly(false);

        ui->textBrowserR->setEnabled(!ui->checkBoxResultFile->isChecked());
    }
}

void MainWindow::checkBoxUppercaseShowHide(int value)
{
    if(value >= 11 && value <= 36)
        ui->checkBoxUppercase->show();
    else
        ui->checkBoxUppercase->hide();
}

void MainWindow::labelNumName(QLabel *label, int base)
{
    if(base == 2)
        label->setText("(Binary)");
    else if(base == 8)
        label->setText("(Octal)");
    else if(base == 10)
        label->setText("(Decimal)");
    else if(base == 12)
        label->setText("(Duodecimal)");
    else if(base == 16)
        label->setText("(Hexadecimal)");
    else
        label->setText("");
}

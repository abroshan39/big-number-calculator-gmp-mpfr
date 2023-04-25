/*
    [Big Number Calculator]
    Publisher: Rosybit
    Url: http://www.rosybit.com
    GitHub: https://github.com/abroshan39/big-number-calculator-gmp-mpfr
    Author: Aboutaleb Roshan [ab.roshan39@gmail.com]
    License: MIT License
*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    enum FontNameIndex  // If you want to change this enum, do it carefully, because the comboBoxFont items is connected to this enum.
    {
        OpenSans,
        OpenSansMedium,
        SourceCodePro
    };

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void calculate();
    void threadFinished(const QString &str);
    void widgetsDeactived(bool b);
    void checkBoxUppercaseShowHide(int value);
    void labelNumName(QLabel *label, int base);

private slots:
    void on_btnCalculate_clicked();
    void on_btnExit_clicked();
    void on_radioButtonCalculator_clicked();
    void on_radioButtonBaseConv_clicked();
    void on_radioButtonInteger_clicked();
    void on_radioButtonFloat_clicked();
    void on_labelPrecDefault_linkActivated(const QString &link);
    void on_labelPrecMin_linkActivated(const QString &link);
    void on_labelPrecMax_linkActivated(const QString &link);
    void on_comboBoxFont_currentIndexChanged(int index);
    void on_spinBoxFontSize_valueChanged(int arg1);
    void on_checkBoxResultFile_clicked(bool checked);
    void on_labelAbout_linkActivated(const QString &link);
    void on_spinBoxN1_valueChanged(int arg1);
    void on_spinBoxN2_valueChanged(int arg1);
    void on_spinBoxR_valueChanged(int arg1);
    void on_labelN1File_linkActivated(const QString &link);
    void on_labelN2File_linkActivated(const QString &link);
    void on_labelN1FileClear_linkActivated(const QString &link);
    void on_labelN2FileClear_linkActivated(const QString &link);
    void on_textEditN1_dragEntered(QDragEnterEvent *e);
    void on_textEditN1_dragLeaved(QDragLeaveEvent *e);
    void on_textEditN1_dropped(QDropEvent *e);
    void on_textEditN2_dragEntered(QDragEnterEvent *e);
    void on_textEditN2_dragLeaved(QDragLeaveEvent *e);
    void on_textEditN2_dropped(QDropEvent *e);

protected:
    void keyPressEvent(QKeyEvent *e) override;
    void closeEvent(QCloseEvent *e) override;

private:
    Ui::MainWindow *ui;
    bool isProcessing = false;
    QString n1_file_path;
    QString n2_file_path;
};
#endif // MAINWINDOW_H

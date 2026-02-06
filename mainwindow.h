#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <fileapi.h>
#include <QList>
#include <QSerialPort>
#include <QFile>

extern DWORD BD_rates[];
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QString controller;
    HANDLE hComm;
    int bindx;
    const char* buff;
    QFile file;

private slots:
    void controller_Info();
    void serialPort_Info();
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void openSerialPort();

    void on_comboBox_2_currentTextChanged();

    void on_comboBox_currentTextChanged();

    void clickedDisConnect();

    void on_pushButton_3_clicked();
    void my_delay (int );

    void on_comboBox_3_currentTextChanged();

    void init();
private:
    Ui::MainWindow *ui;

};

#endif // MAINWINDOW_H

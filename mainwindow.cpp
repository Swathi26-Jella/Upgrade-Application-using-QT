#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QDir>
#include <QFile>
#include <windows.h>
#include <fileapi.h>
#include <stdio.h>
#include <fcntl.h>
#include <QSerialPortInfo>
#include <QElapsedTimer>
#include <QProgressDialog>
#include <QProgressBar>

DWORD BD_rates[] = {115200,38400,19200,9600};
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setStyleSheet("QGroupBox::title {"
                        "font-size: 22px;"
                        "font-weight: Bold;"
                        "color: skyblue;"
                        "background-color: transparent;"
                        "} ");    
    serialPort_Info();
    controller_Info();
    ui->lineEdit->clear();
    init();

//    openSerialPort();

}

void MainWindow::init()
{

    connect(ui->comboBox_3,SIGNAL(activated(int)),this,SLOT(on_comboBox_3_currentTextChanged( )));
    connect(ui->comboBox,SIGNAL(activated(int)),this,SLOT(on_comboBox_currentTextChanged()));
    connect(ui->comboBox_2,SIGNAL(activated(int)),this,SLOT(on_comboBox_2_currentTextChanged()));
    connect(ui->pushButton_3,SIGNAL(clicked()),this,SLOT(on_pushButton_3_clicked()));

}
void MainWindow::serialPort_Info()
{
    QString port_name;
    const auto serialPortInfos = QSerialPortInfo::availablePorts();
    //nPorts = serialPortInfos.count(); // gives the no of ports available.

    for (const QSerialPortInfo &serialPortInfo : serialPortInfos) {
        port_name = serialPortInfo.portName();
        //QString port_loc = serialPortInfo.systemLocation();
        ui->comboBox->addItem(port_name);
    }
    ui->comboBox_2->addItem(QStringLiteral("115200"),QSerialPort::Baud115200);
    ui->comboBox_2->addItem(QStringLiteral("38400"),QSerialPort::Baud38400);
    ui->comboBox_2->addItem(QStringLiteral("19200"),QSerialPort::Baud19200);
    ui->comboBox_2->addItem(QStringLiteral("9600"),QSerialPort::Baud9600);
}
void MainWindow::openSerialPort()
{
    std::string  comPrefix = "\\\\.\\";
    std::string  comID = comPrefix+buff;
    hComm = CreateFileA(comID.c_str(),
                        GENERIC_READ | GENERIC_WRITE,
                        0,
                        nullptr,
                        OPEN_EXISTING,
                        0,
                        nullptr);

    if(hComm == INVALID_HANDLE_VALUE)
    {
        ui->lineEdit->setText("Error in opening serial port");
        my_delay(1000);

    }
    else
    {
        QPalette *palette = new QPalette();
        palette->setColor(QPalette::Base,Qt::white);
        ui->lineEdit->setPalette(*palette);
        ui->lineEdit->setStyleSheet("color: green;");
        ui->lineEdit->setText("Serial Port opened successfully");
        my_delay(1000);

    }
    DCB d1 ;
    d1.DCBlength = sizeof (d1);
    bool status;
    status = GetCommState(hComm, &d1);  // getting the comm state.

    if(status == FALSE)
    {
        ui->lineEdit->setText("Error getting comm status\r\n");
//        my_delay (1000);
        return;
    }
    d1.BaudRate = BD_rates[bindx];
//  d1.BaudRate = CBR_9600;
    d1.ByteSize = 8;
    d1.StopBits = ONESTOPBIT;
    d1.Parity = NOPARITY;
    d1.fDtrControl = 0L;
    d1.fRtsControl = 0L;
    d1.fOutxCtsFlow = 0L;
    d1.fOutxDsrFlow = 0L;

    status = SetCommState(hComm, &d1);       // setting the comm state.

    if(status == FALSE)
    {
        ui->lineEdit->setText("Error in setting DCB structure\r\n");
        return;
    }
}
MainWindow::~MainWindow()
{  
    CloseHandle (hComm);
    delete ui;
}

void MainWindow::my_delay (int ms)

{

  QElapsedTimer timer;

  timer.start();

      while(!timer.hasExpired(ms))

          QCoreApplication::processEvents();

}
void MainWindow::on_pushButton_clicked()
{

    QString file_name = QFileDialog :: getOpenFileName(this,"Select file",QDir::homePath(),"*.hex");
    file.setFileName(file_name);
    if (!file.open(QFile :: ReadOnly | QFile :: Text))
    {
        ui->label_3->setText("Unable to open file");

    }
    else
    {
         ui->label_3->setIndent(80);
         ui->label_3->setText(file_name);  // gives path and file name.

     }

}

void MainWindow::on_pushButton_2_clicked()
{
    DWORD dNoOfBytesWritten = 0;
    WriteFile(hComm,"Hello\r\n",6,&dNoOfBytesWritten,nullptr);

//    qint64 Size;        // size of  a file.
//    float count=0;
//    int nlines;         // total no of lines in a file
//    float princ;        // % increment
//    int nctspln = 0;    // number of characters per line
//    QString line;       // storing the string of a line in a file.
//    QString str;

//    Size = file.size();
//    for (int i=0;i<=4;i++)
//    {
//    str = file.readLine();      // read first line. This line maybe shorter than other lines.
//    if(nctspln < str.length())
//    {
//       nctspln = str.length();
//       continue;
//    }
//    }

//    nlines = int (Size)/(nctspln+1)  ;   // this is numbber of lines in the file

//    princ =  float (100)/float(nlines);  // this is the percentage increment

//    QProgressDialog progressDialog("Processing...", "Abort", 0,INT_MAX, this);

//    QProgressBar* bar = new QProgressBar(&progressDialog);

//    bar->setValue(0);
//    progressDialog.setBar(bar);
//    progressDialog.setMinimumWidth(350);
//    progressDialog.setMinimumDuration(1000);
//    progressDialog.setWindowModality(Qt::WindowModal);

//    file.seek(SEEK_SET);

//    while(! file.atEnd())
//    {

//    line = file.readLine();
//    ui->plainTextEdit->appendPlainText(line);

//    count +=  (princ);
//    progressDialog.setValue(int(count));
//       if(progressDialog.wasCanceled())
//            break;
//       my_delay(50);
//    }
//    //progressDialog.close();

//    file.close();
////         ui->lineEdit->setText("Successfully closed file");


}

void MainWindow::on_comboBox_2_currentTextChanged()
{
    bindx = ui->comboBox_2->currentIndex();
    ui->label_3->setNum(bindx);  // test. Only to see index value. To be removed
}

void MainWindow::on_comboBox_currentTextChanged()
{
    char buf[50];
    int index;
    QString buffer;
    index = ui->comboBox->currentIndex();
    sprintf(buf,"%d",index);
    ui->lineEdit->setText(buf);

    buffer = ui->comboBox->currentText();
    QByteArray ba = buffer.toLocal8Bit();
    buff = ba.data();
    ui->lineEdit->clear();
    ui->lineEdit->setText(buff);
}

void MainWindow::on_pushButton_3_clicked()
{

    ui->pushButton_3->setText("Disconnect");
    ui->pushButton_3->setStyleSheet("color:red");
    ui->lineEdit->setText("Connected");
    openSerialPort();
    connect(ui->pushButton_2,SIGNAL(clicked()),this,SLOT(on_pushButton_2_clicked()));
    connect(ui->pushButton_3,SIGNAL(clicked()),this,SLOT(clickedDisConnect()));
}

void MainWindow::clickedDisConnect()
{
    ui->lineEdit->setText("Disconnected");
    ui->pushButton_3->setText("Connect");
    ui->pushButton_3->setStyleSheet("color:green");
    init();
}

void MainWindow:: controller_Info()
{
    ui->comboBox_3->addItem("Controller 13");
    ui->comboBox_3->addItem("Controller 8");
    ui->comboBox_3->addItem("Controller 164P");
    ui->comboBox_3->addItem("Controller 64/128/256");
    ui->comboBox_3->addItem("Controller STM MD/MDVL");
    ui->comboBox_3->addItem("Controller STM CL/XL/HD/HDVL");
}

void MainWindow::on_comboBox_3_currentTextChanged()
{
    controller = ui->comboBox_3->currentText();
}

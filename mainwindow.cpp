#include "mainwindow.h"
#include "moduleconnection.h"
#include <iostream>
#include <QStyle>
#include <QDesktopWidget>
#include <unistd.h>
#include "ui_mainwindow.h"
#include <QMessageBox>
#include "mainconnection.h"
#include "modulesearch.h"
#include "modulesearch.cpp"
#include <string>

using namespace std;

extern ModuleConnection Module;
extern uint8_t DropConnectionReason;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);

   // ui->comboBox->clear();
    ui->lineEdit->setText("3319");

    this->setGeometry(                // Center a Window on the Screen
       QStyle::alignedRect(
           Qt::LeftToRight,
           Qt::AlignCenter,
           this->size(),
           qApp->desktop()->availableGeometry()
       )
    );

    _SetModulesInfoInComboBox(); //First search modules

}

MainWindow::~MainWindow()
{

    delete ui;
}


void MainWindow::on_pushButton_clicked()
{

  int ConvertedPortValue=ui->lineEdit->text().toInt();

  string IpAddr="";

  if(ui->comboBox->currentText().split(".").length()== 4){ //Check if entered IP
    IpAddr=ui->comboBox->currentText().toStdString();
  }else{
     IpAddr=_GetIPFromName(ui->comboBox->currentText().toStdString());     
  }

  if(ConvertedPortValue < 1 || ConvertedPortValue > 65535 || ui->comboBox->currentText().toStdString() == ""){
  QMessageBox::critical(this,"Error!","You entered the wrong data!");
  }else{

      int connectionState = Module.SetConnection(QString::fromStdString(IpAddr).toStdString(),ConvertedPortValue); // ):

     // cout << "Ip: " << ui->comboBox->currentText().toStdString() << endl; //Debug!!

      switch(connectionState){ //Handle connection state


      case 1:
          QMessageBox::critical(this,"Error!","Socket can't be created, you os broken!");
          // _ClearInput();  //Useless
          break;

      case 2:
          QMessageBox::critical(this,"Error!","You entered the wrong Ip adress or hostname!");
          // _ClearInput();
          break;

      case 3:
           QMessageBox::critical(this,"Error!","Socket connection error, maybe wrong Ip?");
          //  _ClearInput();
          break;

      case 4:
           QMessageBox::critical(this,"Error!","What?");
          // _ClearInput();
          break;
      }


      if(connectionState == 0){ //Create terminal window
      hide();
      MainConnection conn;
      conn.setModal(true);
      conn.exec();

      show();

      if(DropConnectionReason != 0){
      QMessageBox::information(this,"Information","Disconnected from target.");
      DropConnectionReason=0;
      }
      // _ClearInput();

      }

  }

}

void MainWindow::_ClearInput(){
    ui->lineEdit->setText("3319");
  //  ui->comboBox->clear();
}


void MainWindow::_SetModulesInfoInComboBox(){

    ModuleSearch mgr;

    ui->comboBox->clear();

    for(int i=0;i<256;i++){
        _ArrayOfModulesData[i]="";
    }

     _CurrentModulesCount =  mgr.SearchModules(_ArrayOfModulesData);

    for(int i=0; i<_CurrentModulesCount; i++){


        QString Editable= QString::fromStdString(_ArrayOfModulesData[i]);    //Checing adress via QString
        QStringList lst = Editable.split("~");


        if(lst.value(2)== "def"){
           ui->comboBox->addItem(lst.value(1));
        }else{
            ui->comboBox->addItem(lst.value(2));
        }

        if(i==0){
            ui->lineEdit->setText(lst.value((3)));
        }
      }

    if(_CurrentModulesCount==0){
       ui->lineEdit->setText("3319");
    }

}

string MainWindow::_GetIPFromName(string hostname){
    string FindedAddr="";

    for(int i =0; i< _CurrentModulesCount; i++){

        QString Editable= QString::fromStdString(_ArrayOfModulesData[i]);    //Checking adress via QString
        QStringList lst = Editable.split("~");


        if(lst.value(2).toStdString() == hostname){

        FindedAddr=lst.value(1).toStdString();
        break;

        }

    }

    return FindedAddr;
}

string MainWindow::_GetPortByName(string hostname){
    string FindedPort="";

    for(int i =0; i< _CurrentModulesCount; i++){

        QString Editable= QString::fromStdString(_ArrayOfModulesData[i]);    //Checing adress via QString
        QStringList lst = Editable.split("~");


        if(lst.value(2).toStdString() == hostname){

        FindedPort=lst.value(3).toStdString();
        break;

        }

    }

    return FindedPort;
}

void MainWindow::on_comboBox_currentIndexChanged(const QString &arg1)
{
   ui->lineEdit->setText(QString::fromStdString(_GetPortByName(arg1.toStdString())));
}

void MainWindow::on_pushButton_2_clicked()
{
    _SetModulesInfoInComboBox();

}

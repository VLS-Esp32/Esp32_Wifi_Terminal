#include "mainconnection.h"
#include "ui_mainconnection.h"
#include "moduleconnection.h"
#include <iostream>
#include <unistd.h>
#include <QPlainTextEdit>
#include "mainconnection.h"
#include <string>
#include <thread>
#include <QEventLoop>
#include <stdio.h>
#include <time.h>

using namespace std;

extern ModuleConnection Module;
extern uint8_t DropConnectionReason;


const std::string currentDateTime() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);    
    strftime(buf, sizeof(buf), "%X", &tstruct);
    return buf;
}


MainConnection::MainConnection(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MainConnection)
{
    ui->setupUi(this);

    std::thread ReadEvent(ReceivingEventHandler, this);
    ReadEvent.detach(); //Maybe

    if(Module.IsConnected()){

         char ip_array[Module.IpAdress().length()];  // Convert IpAdress to char array

         strcpy(ip_array, Module.IpAdress().c_str());

         this->setWindowTitle("Connected to: "+ QString(ip_array));

         ui->comboBox->clear();


         ui->comboBox->addItem("14");
         ui->comboBox->addItem("6");
         ui->comboBox->addItem("7");
         ui->comboBox->addItem("8");
         ui->comboBox->addItem("9");
         ui->comboBox->addItem("10");
         ui->comboBox->addItem("11");
         ui->comboBox->addItem("12");
         ui->comboBox->addItem("13");
         ui->comboBox->addItem("16");
         ui->comboBox->addItem("18");
         ui->comboBox->addItem("20");
         ui->comboBox->addItem("22");
         ui->comboBox->addItem("24");
         ui->comboBox->addItem("26");
         ui->comboBox->addItem("28");
         ui->comboBox->addItem("32");
         ui->comboBox->addItem("36");
         ui->comboBox->addItem("40");
         ui->comboBox->addItem("44");
         ui->comboBox->addItem("48");
         ui->comboBox->addItem("52");
         ui->comboBox->addItem("60");
         ui->comboBox->addItem("68");
         ui->comboBox->addItem("72");

         ui->textEdit->setFontPointSize(14);

    }
}

MainConnection::~MainConnection()
{    
    delete ui;
}

void MainConnection::ReceivingEventHandler(MainConnection * p){ //Main Listener thread

    bool NextLineIsTime=true;

    while(Module.IsConnected()){

        if(Module.Available()>0){

           QString tmp;

           while(Module.Available()>0){

               if(NextLineIsTime && p->_PrintTime){
                 tmp += QString::fromStdString(currentDateTime())+" :> ";
                 NextLineIsTime=false;
               }

               QString editable= QString(Module.Read());

               if(editable == '\n' && p->_PrintTime){

                   tmp +=editable;
                   NextLineIsTime=true;

               }else{
                  tmp +=editable;
               }
           }

           p->ui->textEdit->insertPlainText(tmp);

           if( p->_Autoscroling){
              p->ui->textEdit->moveCursor(QTextCursor::End);
           }

        }

        usleep(7550);
    }

    if(!Module.IsConnected()){
      DropConnectionReason=1;
      cout << "Disconnected";
      p->close();

    }

}

void MainConnection::on_pushButton_clicked()
{
    QString tm;

    if(_PrintInNewLine){
          tm = ui->lineEdit->text()+'\n';
    }else{
         tm = ui->lineEdit->text();
    }


   if(Module.Write(tm.toStdString())){

        ui->lineEdit->clear();
    }else{
      DropConnectionReason=1;
      cout << "Disconnected";
      close();
    }

}

void MainConnection::on_pushButton_2_clicked()
{
    ui->textEdit->clear();
}


void MainConnection::on_sndnl_stateChanged(int arg1)
{
 _PrintInNewLine= !_PrintInNewLine;
}



void MainConnection::on_checkBox_stateChanged(int arg1)
{
  _Autoscroling= !_Autoscroling;
}

void MainConnection::on_checkBox_2_stateChanged(int arg1)
{
  _PrintTime= !_PrintTime;
}


void MainConnection::on_comboBox_currentIndexChanged(const QString &arg1)
{
    int tmp = stoi(arg1.toStdString());

    if(tmp !=0){

     ui->textEdit->setFontPointSize(tmp);
      cout << "Set font size: " << tmp << endl;
    }else{
      cout<< "Unknown string conwerting error" << endl;
    }
}

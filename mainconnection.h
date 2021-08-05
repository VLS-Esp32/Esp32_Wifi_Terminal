#ifndef MAINCONNECTION_H
#define MAINCONNECTION_H

#include <QDialog>

namespace Ui {
class MainConnection;
}

class MainConnection : public QDialog
{
    Q_OBJECT

public:
    explicit MainConnection(QWidget *parent = nullptr);
    ~MainConnection();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_sndnl_stateChanged(int arg1);


    void on_checkBox_stateChanged(int arg1);

    void on_checkBox_2_stateChanged(int arg1);

    void on_comboBox_currentIndexChanged(const QString &arg1);

private:
    Ui::MainConnection *ui;
    static void ReceivingEventHandler(MainConnection * p);
    bool _PrintInNewLine=true;
    bool _Autoscroling=true;
    bool _PrintTime=false;
};

#endif // MAINCONNECTION_H

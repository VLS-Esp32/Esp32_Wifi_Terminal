#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "moduleconnection.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_comboBox_currentIndexChanged(const QString &arg1);

    void on_pushButton_2_clicked();

private:
    Ui::MainWindow *ui;
    void _SetModulesInfoInComboBox();
    string _ArrayOfModulesData[256];
    int _CurrentModulesCount=0;
    void _ClearInput();
    string _GetIPFromName(string hostname);
    string _GetPortByName(string hostname);
};

#endif // MAINWINDOW_H

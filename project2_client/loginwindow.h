#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QMainWindow>
#include <iostream>
#include <string>
#include<mainwindow.h>
using namespace std;

namespace Ui {
class LoginWindow;
}

class LoginWindow : public QMainWindow
{
    Q_OBJECT

public:
    client m_client;
    explicit LoginWindow(QWidget *parent = 0);
    ~LoginWindow();

private slots:
    void on_loginButton_clicked();

    void on_regButton_clicked();

    void on_submitButton_clicked();

    //void on_submitButton_2_clicked();

    void on_submitButton_2_clicked();

private:
    Ui::LoginWindow *ui;
    static string command;
    MainWindow *mw;
};

#endif // LOGINWINDOW_H

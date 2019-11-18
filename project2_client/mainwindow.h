#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<string>
#include "client.h"
using namespace std;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    client m_client;
    explicit MainWindow(client m_client,QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_iButton_clicked();

    void on_iaButton_clicked();

    void on_onlineFriendsShowbtn_clicked();

    void on_sendButton_clicked();

    void on_messageButton_clicked();

    //void on_messageButton_clicked();

private:
    Ui::MainWindow *ui;
    static string cmd;
    static string in;
};

#endif // MAINWINDOW_H

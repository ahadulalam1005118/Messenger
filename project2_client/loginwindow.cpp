#include "loginwindow.h"
#include "ui_loginwindow.h"
#include <string>
#include "mainwindow.h"
#include <QApplication>
#include<unistd.h>
using namespace std;
string LoginWindow::command = "";
LoginWindow::LoginWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LoginWindow)
{
    std::string _configuration_file = "/home/ahad/project2_client/configurations";
    m_client.begin(_configuration_file);
    m_client.run();
    ui->setupUi(this);
    ui->label->hide();
    ui->label_2->hide();
    ui->textEdit->hide();
    ui->textEdit_2->hide();
    ui->submitButton->hide();
    ui->label_3->hide();
    ui->label_4->hide();
    ui->textEdit_3->hide();
    ui->textEdit_4->hide();
    ui->submitButton_2->hide();
}

LoginWindow::~LoginWindow()
{
    delete ui;
}

void LoginWindow::on_loginButton_clicked()
{
    ui->label->show();
    ui->label_2->show();
    ui->textEdit->show();
    ui->textEdit_2->show();
    ui->submitButton->show();
    command.append("l");
}

void LoginWindow::on_regButton_clicked()
{
    ui->label_3->show();
    ui->label_4->show();
    ui->textEdit_3->show();
    ui->textEdit_4->show();
    ui->submitButton_2->show();
    command.append("r");

}

void LoginWindow::on_submitButton_clicked()
{
    //cout<< "clicked";
    command.append(":");
    command.append(ui->textEdit->toPlainText().toStdString()); //username
    command.append(":");
    command.append(ui->textEdit_2->toPlainText().toStdString()); //password
    m_client.write_to_sockfd(command);
    command.clear();
    ui->textEdit->clear();
    ui->textEdit_2->clear();
    sleep(2);
    //ui->submitButton->disconnect();
    if(m_client.login_status == true) {
        mw = new MainWindow(m_client,this);
        hide();
        mw->show();
    }
    else {
        ui->label->hide();
        ui->label_2->hide();
        ui->textEdit->hide();
        ui->textEdit_2->hide();
        ui->submitButton->hide();
    }

}

void LoginWindow::on_submitButton_2_clicked()
{
    command.append(":");
    command.append(ui->textEdit_3->toPlainText().toStdString()); //username
    command.append(":");
    command.append(ui->textEdit_4->toPlainText().toStdString()); //password
    m_client.write_to_sockfd(command);
    command.clear();
    ui->textEdit_3->clear();
    ui->textEdit_4->clear();
    sleep(2);
    //ui->submitButton_2->disconnect();


        ui->label->hide();
        ui->label_2->hide();
        ui->textEdit->hide();
        ui->textEdit_2->hide();
        ui->submitButton->hide();



}

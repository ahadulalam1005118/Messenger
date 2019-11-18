#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<iostream>
#include<string>
#include <unistd.h>
string MainWindow::cmd = "";
string MainWindow::in = "";
MainWindow::MainWindow(client m_client, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->m_client = m_client;
    QString uname = QString::fromStdString(m_client.username);
    ui->username->setText(uname);
    ui->label->hide();
    ui->label_2->hide();
    ui->textEdit->hide();
    ui->textEdit_2->hide();
    ui->sendButton->hide();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_iButton_clicked()
{
    ui->label->show();
    ui->label_2->show();
    ui->textEdit->clear();
    ui->textEdit_2->clear();
    ui->textEdit->show();
    ui->textEdit_2->show();
    ui->sendButton->show();
    in.append("i");
}

void MainWindow::on_iaButton_clicked()
{
    ui->label->show();
    ui->label_2->show();
    ui->textEdit->clear();
    ui->textEdit_2->clear();
    ui->textEdit->show();
    ui->textEdit_2->show();
    ui->sendButton->show();
    in.append("ia");

}

void MainWindow::on_onlineFriendsShowbtn_clicked()
{
    ui->online_TE->clear();
    vector<friends_info> friends = m_client.get_online_friends();
    for(int cnt =0; cnt < friends.size(); cnt++) {
        QString qname = QString::fromStdString(friends[cnt].username);
        ui->online_TE->setText(qname);
    }

}
void MainWindow::on_messageButton_clicked()
{
    ui->label->show();
    ui->label_2->show();
    ui->textEdit->clear();
    ui->textEdit_2->clear();
    ui->textEdit->show();
    ui->textEdit_2->show();
    ui->sendButton->show();
    in.append("m");
}


void MainWindow::on_sendButton_clicked()
{
    if(in == "i") {

        cmd.append("i");
        cmd.append(":");
        cmd.append(ui->textEdit->toPlainText().toStdString());
        cmd.append(":");
        cmd.append(ui->textEdit_2->toPlainText().toStdString());
        m_client.write_to_sockfd(cmd);

    }
    if(in == "ia") {
        cmd.append("ia");
        cmd.append(":");
        cmd.append(ui->textEdit->toPlainText().toStdString());
        cmd.append(":");
        cmd.append(ui->textEdit_2->toPlainText().toStdString());
        m_client.write_to_sockfd(cmd);

    }
    if(in == "m") {
        cmd.append("m");
        cmd.append(":");
        cmd.append(ui->textEdit->toPlainText().toStdString());
        cmd.append(":");
        cmd.append(ui->textEdit_2->toPlainText().toStdString());
        m_client.write_to_sockfd(cmd);
    }


}



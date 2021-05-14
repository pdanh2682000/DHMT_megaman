#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setFixedSize(700,300);
    timerId = startTimer(50);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnJump_clicked()
{
    ui->GraphicsPresenter->checkJump = 1;
}

void MainWindow::on_btnShiled_clicked()
{
    if(ui->progressBar->value() == 100){ // đầy power mới bắn được
        ui->GraphicsPresenter->checkShield = 1;
        ui->progressBar->setValue(0); // dùng shield thì mất power
        ui->GraphicsPresenter->valueProgess = 0;
    }
}

void MainWindow::on_btnBullet_clicked()
{
    ui->GraphicsPresenter->checkBullet = 1;
    ui->GraphicsPresenter->pos_bullet = 65 ;
    ui->GraphicsPresenter->const_y_bullet = height() - 60 - ui->GraphicsPresenter->pos_y_megaman;
}
void MainWindow::timerEvent(QTimerEvent *){
    if(ui->GraphicsPresenter->checkHit == 1){
        if(ui->GraphicsPresenter->valueProgess != 100){ // đầy power rồi thì thôi
            ui->GraphicsPresenter->valueProgess += 50;
            ui->progressBar->setValue(ui->GraphicsPresenter->valueProgess);
        }
    }
    ui->GraphicsPresenter->checkHit = 0;
}

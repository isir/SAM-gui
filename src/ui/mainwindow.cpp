#include "mainwindow.h"
#include "systemdisplay.h"
#include "topicplotter.h"
#include "ui/mqttconnect.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , _mc(_mqtt)
    , _ld(_mqtt)
    , _md(_mqtt)
    , _tp(_mqtt, "sam/myoband/acc")
    , _sd(_mqtt)
{
    ui->setupUi(this);
    ui->verticalLayout->insertWidget(0, &_mc);
    ui->verticalLayout->addWidget(&_ld);
    ui->horizontalLayout->addWidget(&_md);
    ui->horizontalLayout->addWidget(&_tp);

    ui->verticalLayout->setStretch(0, 3);
    ui->verticalLayout->setStretch(1, 1);

    ui->horizontalLayout->setStretch(0, 2);
    ui->horizontalLayout->setStretch(1, 5);

    ui->statusBar->addPermanentWidget(&_sd);
    setWindowTitle("SAM Monitoring");
}

MainWindow::~MainWindow()
{
    delete ui;
}

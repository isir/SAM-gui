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
    , _pw(_mqtt)
{
    ui->setupUi(this);

    auto scrollArea = new QScrollArea;
    scrollArea->setBackgroundRole(QPalette::Dark);
    scrollArea->setWidget(&_pw);
    scrollArea->setWidgetResizable(true);

    ui->verticalLayout->insertWidget(0, &_mc);
    ui->horizontalLayout_bottom->addWidget(&_ld);
    ui->horizontalLayout_bottom->addWidget(scrollArea);
    ui->horizontalLayout->addWidget(&_md);
    ui->horizontalLayout->addWidget(&_tp);

    ui->verticalLayout->setStretch(0, 3);
    ui->verticalLayout->setStretch(1, 1);

    ui->horizontalLayout->setStretch(0, 2);
    ui->horizontalLayout->setStretch(1, 5);

    ui->horizontalLayout_bottom->setStretch(0, 7);
    ui->horizontalLayout_bottom->setStretch(1, 3);

    ui->statusBar->addPermanentWidget(&_sd);
    setWindowTitle("SAM Monitoring");
}

MainWindow::~MainWindow()
{
    delete ui;
}

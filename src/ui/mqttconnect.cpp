#include "mqttconnect.h"
#include "ui_mqttconnect.h"

MqttConnect::MqttConnect(MqttClientWrapper& mqtt, QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::MqttConnect)
    , _mqtt(mqtt)
{
    ui->setupUi(this);
    QObject::connect(ui->pushButton_connect, &QPushButton::clicked, this, &MqttConnect::button_callback);
    QObject::connect(&_timer, &QTimer::timeout, this, &MqttConnect::timer_callback);

    _timer.setInterval(1000);
    _timer.start();
}

MqttConnect::~MqttConnect()
{
    delete ui;
}

void MqttConnect::button_callback()
{
    _mqtt.Client::connect(ui->lineEdit_hostname->text().toStdString(), 1883);
}

void MqttConnect::timer_callback()
{
    if (_mqtt.connected()) {
        ui->pushButton_connect->setText("Connected");
        ui->pushButton_connect->setEnabled(false);
    } else {
        ui->pushButton_connect->setText("Connect");
        ui->pushButton_connect->setEnabled(true);
    }
}

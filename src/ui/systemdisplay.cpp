#include "systemdisplay.h"
#include "ui_systemdisplay.h"

SystemDisplay::SystemDisplay(MqttClientWrapper& mqtt, QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::SystemDisplay)
    , _mqtt(mqtt)
{
    ui->setupUi(this);
    _pbs.push_back(ui->progressBar_cpu0);
    _pbs.push_back(ui->progressBar_cpu1);
    _pbs.push_back(ui->progressBar_cpu2);
    _pbs.push_back(ui->progressBar_cpu3);

    QObject::connect(&_mqtt, &MqttClientWrapper::mqtt_connected, this, &SystemDisplay::setup);
}

SystemDisplay::~SystemDisplay()
{
    delete ui;
}

void SystemDisplay::setup()
{
    auto sub = _mqtt.subscribe("system/cpu_load");
    QObject::connect(sub.get(), &MqttSubscriptionWrapper::message_received, this, &SystemDisplay::mqtt_callback_load, Qt::QueuedConnection);

    sub = _mqtt.subscribe("system/cpu_temp");
    QObject::connect(sub.get(), &MqttSubscriptionWrapper::message_received, this, &SystemDisplay::mqtt_callback_temp, Qt::QueuedConnection);
}

void SystemDisplay::mqtt_callback_load(Mosquittopp::Message msg)
{
    QStringList sl = QString::fromStdString(msg.payload()).split(' ', QString::SkipEmptyParts);
    for (int i = 0; i < 4; ++i) {
        _pbs[i]->setValue(100 * (sl.at(i + 1).toDouble()));
    }
}

void SystemDisplay::mqtt_callback_temp(Mosquittopp::Message msg)
{
    ui->label_cpu_temp->setText(QString::number(std::stoi(msg.payload()) / 1000) + "°C");
}

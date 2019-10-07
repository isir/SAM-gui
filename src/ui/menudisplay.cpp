#include "menudisplay.h"
#include "ui_menudisplay.h"

MenuDisplay::MenuDisplay(MqttClientWrapper& mqtt, QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::MenuDisplay)
    , _mqtt(mqtt)
{
    ui->setupUi(this);
    QObject::connect(ui->pushButton_send, &QPushButton::pressed, this, &MenuDisplay::send_callback);
    QObject::connect(ui->lineEdit_command, &QLineEdit::returnPressed, this, &MenuDisplay::send_callback);

    QObject::connect(&_mqtt, &MqttClientWrapper::mqtt_connected, this, &MenuDisplay::setup);
}

MenuDisplay::~MenuDisplay()
{
    delete ui;
}

void MenuDisplay::setup()
{
    auto sub = _mqtt.subscribe("sam/menu/output");
    QObject::connect(sub.get(), &MqttSubscriptionWrapper::message_received, this, &MenuDisplay::mqtt_message_callback, Qt::QueuedConnection);
}

void MenuDisplay::mqtt_message_callback(Mosquittopp::Message msg)
{
    ui->textEdit_menu->clear();
    ui->textEdit_menu->append(QString::fromStdString(msg.payload()));
}

void MenuDisplay::send_callback()
{
    _mqtt.publish("sam/menu/input", ui->lineEdit_command->text().toStdString(), MqttClientWrapper::QoS2);
    ui->lineEdit_command->clear();
}

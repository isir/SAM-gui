#include "logdisplay.h"
#include "ui_logdisplay.h"
#include <QTime>

LogDisplay::LogDisplay(MqttClientWrapper& mqtt, QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::LogDisplay)
    , _mqtt(mqtt)
{
    ui->setupUi(this);
    QObject::connect(ui->pushButton_clear, &QPushButton::pressed, [this]() { ui->textEdit->clear(); _messages.clear(); });
    QObject::connect(ui->checkBox_debug, &QCheckBox::clicked, this, &LogDisplay::refresh);
    QObject::connect(ui->checkBox_info, &QCheckBox::clicked, this, &LogDisplay::refresh);
    QObject::connect(ui->checkBox_warning, &QCheckBox::clicked, this, &LogDisplay::refresh);
    QObject::connect(ui->checkBox_critical, &QCheckBox::clicked, this, &LogDisplay::refresh);
    QObject::connect(ui->checkBox_fatal, &QCheckBox::clicked, this, &LogDisplay::refresh);

    QObject::connect(&_mqtt, &MqttClientWrapper::mqtt_connected, this, &LogDisplay::setup);
}

LogDisplay::~LogDisplay()
{
    delete ui;
}

void LogDisplay::setup()
{
    auto sub = _mqtt.subscribe("sam/log/#");
    QObject::connect(sub.get(), &MqttSubscriptionWrapper::message_received, this, &LogDisplay::mqtt_message_callback, Qt::QueuedConnection);
}

void LogDisplay::display_message(Mosquittopp::Message msg)
{
    QString payload = QTime::currentTime().toString("[hh:mm:ss] ") + QString::fromStdString(msg.payload());
    if (payload.endsWith('\n')) {
        payload.chop(1);
    }
    if (payload.endsWith('\r')) {
        payload.chop(1);
    }
    QString topic = QString::fromStdString(msg.topic());

    if ((topic.endsWith("debug") && ui->checkBox_debug->isChecked()) || (topic.endsWith("info") && ui->checkBox_info->isChecked())) {
        ui->textEdit->setTextColor(QColor::fromRgb(0, 0, 0));
        ui->textEdit->setTextBackgroundColor(QColor::fromRgb(255, 255, 255));
    } else if (topic.endsWith("warning") && ui->checkBox_warning->isChecked()) {
        ui->textEdit->setTextColor(QColor::fromRgb(253, 106, 2));
        ui->textEdit->setTextBackgroundColor(QColor::fromRgb(255, 255, 255));
    } else if (topic.endsWith("critical") && ui->checkBox_critical->isChecked()) {
        ui->textEdit->setTextColor(QColor::fromRgb(255, 0, 0));
        ui->textEdit->setTextBackgroundColor(QColor::fromRgb(255, 255, 255));
    } else if (topic.endsWith("fatal") && ui->checkBox_fatal->isChecked()) {
        ui->textEdit->setTextColor(QColor::fromRgb(255, 255, 255));
        ui->textEdit->setTextBackgroundColor(QColor::fromRgb(255, 0, 0));
    } else {
        return;
    }
    ui->textEdit->append(payload);
}

void LogDisplay::mqtt_message_callback(Mosquittopp::Message msg)
{
    display_message(msg);
    _messages.push_back(msg);
    if (_messages.size() > 1000) {
        _messages.removeFirst();
    }
}

void LogDisplay::refresh()
{
    ui->textEdit->clear();
    foreach (Mosquittopp::Message msg, _messages) {
        display_message(msg);
    }
}

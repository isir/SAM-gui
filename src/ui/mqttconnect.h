#ifndef MQTTCONNECT_H
#define MQTTCONNECT_H

#include "mqtt_client_wrapper.h"
#include <QTimer>
#include <QWidget>

namespace Ui {
class MqttConnect;
}

class MqttConnect : public QWidget {
    Q_OBJECT

public:
    explicit MqttConnect(MqttClientWrapper& mqtt, QWidget* parent = nullptr);
    ~MqttConnect();

private:
    Ui::MqttConnect* ui;

    MqttClientWrapper& _mqtt;
    QTimer _timer;

private slots:
    void button_callback();
    void timer_callback();
};

#endif // MQTTCONNECT_H

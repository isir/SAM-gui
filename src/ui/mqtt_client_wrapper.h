#ifndef MQTT_CLIENT_WRAPPER_H
#define MQTT_CLIENT_WRAPPER_H

#include "mosquittopp/client.h"
#include <QMap>
#include <QObject>
#include <QTimer>

Q_DECLARE_METATYPE(Mosquittopp::Message)

class MqttSubscriptionWrapper : public QObject {
    Q_OBJECT
public:
    MqttSubscriptionWrapper(std::shared_ptr<Mosquittopp::Subscription> sub);

private:
    std::shared_ptr<Mosquittopp::Subscription> _sub;

signals:
    void message_received(Mosquittopp::Message msg);
};

class MqttClientWrapper : public QObject, public Mosquittopp::Client {
    Q_OBJECT
public:
    explicit MqttClientWrapper(QObject* parent = nullptr);

    std::shared_ptr<MqttSubscriptionWrapper> subscribe(std::string pattern, QOS qos = QoS0);

private:
    QTimer _timer;
    QMap<std::string, std::shared_ptr<MqttSubscriptionWrapper>> _subs;

private slots:
    void timer_callback();

signals:
    void mqtt_connected();
    void mqtt_disconnected();
};

#endif // MQTT_CLIENT_WRAPPER_H

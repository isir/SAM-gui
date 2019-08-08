#include "mqtt_client_wrapper.h"

MqttSubscriptionWrapper::MqttSubscriptionWrapper(std::shared_ptr<Mosquittopp::Subscription> sub)
    : _sub(sub)
{
    _sub->add_callback(this, &MqttSubscriptionWrapper::message_received);
}

MqttClientWrapper::MqttClientWrapper(QObject* parent)
    : QObject(parent)
{
    QObject::connect(&_timer, &QTimer::timeout, this, &MqttClientWrapper::timer_callback);
    _timer.start(500);
}

std::shared_ptr<MqttSubscriptionWrapper> MqttClientWrapper::subscribe(std::string pattern, QOS qos)
{
    if (_subs.contains(pattern)) {
        return _subs[pattern];
    } else {
        auto p = std::make_shared<MqttSubscriptionWrapper>(Mosquittopp::Client::subscribe(pattern, qos));
        _subs[pattern] = p;
        return p;
    }
}

void MqttClientWrapper::timer_callback()
{
    static bool status = false;
    if (connected() && status == false) {
        emit mqtt_connected();
    } else if (!connected() && status == true) {
        emit mqtt_disconnected();
    }
    status = connected();
}

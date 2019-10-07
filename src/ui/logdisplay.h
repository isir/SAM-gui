#ifndef LOGDISPLAY_H
#define LOGDISPLAY_H

#include "mqtt_client_wrapper.h"
#include <QList>
#include <QWidget>

namespace Ui {
class LogDisplay;
}

class LogDisplay : public QWidget {
    Q_OBJECT

public:
    explicit LogDisplay(MqttClientWrapper& mqtt, QWidget* parent = nullptr);
    ~LogDisplay();

public slots:
    void setup();

private:
    void display_message(Mosquittopp::Message msg);
    void mqtt_message_callback(Mosquittopp::Message msg);

    Ui::LogDisplay* ui;
    MqttClientWrapper& _mqtt;
    QList<Mosquittopp::Message> _messages;

private slots:
    void refresh();
};

#endif // LOGDISPLAY_H

#ifndef MENUDISPLAY_H
#define MENUDISPLAY_H

#include "mqtt_client_wrapper.h"
#include <QWidget>

namespace Ui {
class MenuDisplay;
}

class MenuDisplay : public QWidget {
    Q_OBJECT

public:
    explicit MenuDisplay(MqttClientWrapper& mqtt, QWidget* parent = nullptr);
    ~MenuDisplay();

public slots:
    void setup();

private:
    void mqtt_message_callback(Mosquittopp::Message msg);

    Ui::MenuDisplay* ui;
    MqttClientWrapper& _mqtt;

private slots:
    void send_callback();
};

#endif // MENUDISPLAY_H

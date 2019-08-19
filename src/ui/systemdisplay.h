#ifndef SYSTEMDISPLAY_H
#define SYSTEMDISPLAY_H

#include "mqtt_client_wrapper.h"
#include <QList>
#include <QProgressBar>
#include <QWidget>

namespace Ui {
class SystemDisplay;
}

class SystemDisplay : public QWidget {
    Q_OBJECT

public:
    explicit SystemDisplay(MqttClientWrapper& mqtt, QWidget* parent = nullptr);
    ~SystemDisplay();

private slots:
    void setup();

    void mqtt_callback_load(Mosquittopp::Message msg);
    void mqtt_callback_temp(Mosquittopp::Message msg);

private:
    Ui::SystemDisplay* ui;
    QList<QProgressBar*> _pbs;

    MqttClientWrapper& _mqtt;
};

#endif // SYSTEMDISPLAY_H

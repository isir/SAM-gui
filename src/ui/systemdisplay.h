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

public slots:
    void setup();

private:
    Ui::SystemDisplay* ui;
    QList<QProgressBar*> _pbs;

    MqttClientWrapper& _mqtt;
};

#endif // SYSTEMDISPLAY_H

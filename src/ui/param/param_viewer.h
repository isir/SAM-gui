#ifndef PARAM_VIEWER_H
#define PARAM_VIEWER_H

#include "ui/mqtt_client_wrapper.h"
#include <QLineEdit>
#include <QMap>
#include <QWidget>

namespace Ui {
class ParamViewer;
}

class ParamViewer : public QWidget {
    Q_OBJECT

public:
    explicit ParamViewer(MqttClientWrapper& mqtt, QWidget* parent = nullptr);
    ~ParamViewer();

private slots:
    void setup();
    void mqtt_message_callback(Mosquittopp::Message msg);
    void insert_from_topic_and_value(QString topic, QString value);

private:
    Ui::ParamViewer* ui;
    MqttClientWrapper& _mqtt;
    QMap<QString, QLineEdit*> _form_entries;
    QMap<QString, QMetaObject::Connection> _con;
};

#endif // PARAM_VIEWER_H

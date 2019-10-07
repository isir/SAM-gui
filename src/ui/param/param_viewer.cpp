#include "param_viewer.h"
#include "ui_param_viewer.h"
#include <QDebug>
#include <QLabel>

ParamViewer::ParamViewer(MqttClientWrapper& mqtt, QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::ParamViewer)
    , _mqtt(mqtt)
{
    ui->setupUi(this);
    QObject::connect(&_mqtt, &MqttClientWrapper::mqtt_connected, this, &ParamViewer::setup);
}

ParamViewer::~ParamViewer()
{
    delete ui;
}

void ParamViewer::setup()
{
    auto sub = _mqtt.subscribe("sam/param/#");
    QObject::connect(sub.get(), &MqttSubscriptionWrapper::message_received, this, &ParamViewer::mqtt_message_callback, Qt::QueuedConnection);
}

void ParamViewer::mqtt_message_callback(Mosquittopp::Message msg)
{
    insert_from_topic_and_value(QString::fromStdString(msg.topic()), QString::fromStdString(msg.payload()));
}

void ParamViewer::insert_from_topic_and_value(QString topic, QString value)
{
    int cnt = ui->formLayout->rowCount();
    qDebug() << topic << value << cnt;

    if (_form_entries.contains(topic)) {
        _form_entries[topic]->setText(value);
        return;
    }

    auto edit = new QLineEdit(value);
    if (!_con.contains(topic))
        _con[topic] = QObject::connect(edit, &QLineEdit::textEdited, [this, topic](QString text) { _mqtt.publish(topic.toStdString(), text.toStdString(), Mosquittopp::Client::QoS1, true); });
    _form_entries.insert(topic, edit);
    ui->formLayout->addRow(topic, edit);
}

#include "topicplotter.h"
#include "ui_topicplotter.h"
#include <QVBoxLayout>
#include <QtCharts/QLegendMarker>

TopicPlotter::TopicPlotter(MqttClientWrapper& mqtt, QString topic_name, QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::TopicPlotter)
    , _mqtt(mqtt)
    , _topic_name(topic_name)
    , _chart(new QtCharts::QChart())
    , _chartview(_chart)
    , _max_size(100)
{
    ui->setupUi(this);

    QVBoxLayout* main_layout = new QVBoxLayout(this);
    main_layout->addWidget(&_chartview);

    _x_axis.setRange(0, _max_size);
    _y_axis.setRange(-10, 10);
    _chart->addAxis(&_x_axis, Qt::AlignBottom);
    _chart->addAxis(&_y_axis, Qt::AlignRight);

    QObject::connect(&_mqtt, &MqttClientWrapper::mqtt_connected, this, &TopicPlotter::enable);
    QObject::connect(&_mqtt, &MqttClientWrapper::mqtt_disconnected, this, &TopicPlotter::disable);
}

TopicPlotter::~TopicPlotter()
{
    disable();
    delete ui;
    while (_buffers.size() > 0) {
        delete _buffers.takeFirst();
    }
}

void TopicPlotter::enable()
{
    if (_mqtt.connected()) {
        auto sub = _mqtt.subscribe(_topic_name.toStdString());
        QObject::connect(sub.get(), &MqttSubscriptionWrapper::message_received, this, &TopicPlotter::mqtt_callback, Qt::QueuedConnection);
    }
}

void TopicPlotter::disable()
{
    if (_mqtt.connected()) {
        auto sub = _mqtt.subscribe(_topic_name.toStdString());
        QObject::disconnect(sub.get(), &MqttSubscriptionWrapper::message_received, this, &TopicPlotter::mqtt_callback);
    }
}

void TopicPlotter::mqtt_callback(Mosquittopp::Message msg)
{
    static int idx = -1;

    QStringList payload = QString::fromStdString(msg.payload()).split(' ', QString::SkipEmptyParts);

    if (_series.isEmpty()) {
        for (int i = 0; i < payload.size(); ++i) {
            _series.push_back(new QtCharts::QLineSeries());
            _series.last()->setName(_topic_name + "/" + QString::number(i));
            _series.last()->setUseOpenGL(true);
            _chart->addSeries(_series.last());
            _series.last()->attachAxis(&_x_axis);
            _series.last()->attachAxis(&_y_axis);
        }

        QList<QtCharts::QLegendMarker*> markers = _chart->legend()->markers();
        foreach (QtCharts::QLegendMarker* marker, markers) {
            QObject::connect(marker, &QtCharts::QLegendMarker::clicked, this, &TopicPlotter::marker_clicked_callback);
        }
    }

    if (_buffers.isEmpty()) {
        for (int i = 0; i < payload.size(); ++i) {
            QVector<QPointF>* buffer = new QVector<QPointF>();
            _buffers.push_back(buffer);
            for (int j = 0; j < _max_size; ++j) {
                buffer->append(QPointF(j, 0));
            }
        }
    }

    if (idx >= _max_size - 1) {
        foreach (QVector<QPointF>* b, _buffers) {
            for (int i = 0; i < b->size() - 1; ++i) {
                b->operator[](i).ry() = b->operator[](i + 1).y();
            }
        }
    } else {
        ++idx;
    }

    for (int i = 0; i < payload.size(); ++i) {
        _buffers[i]->operator[](idx).ry() = payload[i].toDouble();
    }

    if (!isVisible()) {
        return;
    }

    double vmin = std::numeric_limits<double>::max();
    double vmax = std::numeric_limits<double>::min();

    foreach (QVector<QPointF>* b, _buffers) {
        foreach (QPointF p, *b) {
            if (p.y() > vmax)
                vmax = p.y();
            if (p.y() < vmin)
                vmin = p.y();
        }
    }
    _y_axis.setRange(vmin, vmax);

    for (int i = 0; i < _series.size(); ++i) {
        _series[i]->replace(*_buffers[i]);
    }
}

void TopicPlotter::marker_clicked_callback()
{
    QtCharts::QLegendMarker* marker = qobject_cast<QtCharts::QLegendMarker*>(sender());

    if (!marker)
        return;

    marker->series()->setVisible(!marker->series()->isVisible());
    marker->setVisible(true);
}

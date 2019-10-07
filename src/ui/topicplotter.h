#ifndef TOPICPLOTTER_H
#define TOPICPLOTTER_H

#include "mqtt_client_wrapper.h"
#include <QWidget>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>

namespace Ui {
class TopicPlotter;
}

class TopicPlotter : public QWidget {
    Q_OBJECT

public:
    explicit TopicPlotter(MqttClientWrapper& mqtt, QString topic_name, QWidget* parent = nullptr);
    ~TopicPlotter();

public slots:
    void enable();
    void disable();

private:
    void mqtt_callback(Mosquittopp::Message msg);

    Ui::TopicPlotter* ui;

    MqttClientWrapper& _mqtt;

    QString _topic_name;

    QtCharts::QChart* _chart;
    QtCharts::QChartView _chartview;

    QVector<QtCharts::QLineSeries*> _series;
    QVector<QVector<QPointF>*> _buffers;
    QtCharts::QValueAxis _x_axis;
    QtCharts::QValueAxis _y_axis;

    const int _max_size;

private slots:
    void marker_clicked_callback();
};

#endif // TOPICPLOTTER_H

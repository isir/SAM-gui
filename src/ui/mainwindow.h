#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "logdisplay.h"
#include "menudisplay.h"
#include "mqttconnect.h"
#include "systemdisplay.h"
#include "topicplotter.h"
#include "ui/param/param_viewer.h"
#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow* ui;

    MqttClientWrapper _mqtt;

    MqttConnect _mc;
    LogDisplay _ld;
    MenuDisplay _md;
    TopicPlotter _tp;
    SystemDisplay _sd;
    ParamViewer _pw;
};

#endif // MAINWINDOW_H

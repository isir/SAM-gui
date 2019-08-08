#include "param_viewer.h"
#include "ui_param_viewer.h"
#include <QDebug>

ParamViewer::ParamViewer(MqttClientWrapper& mqtt, QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::ParamViewer)
    , _mqtt(mqtt)
{
    ui->setupUi(this);
    ui->treeWidget->setColumnCount(2);
    auto item = new QTreeWidgetItem();
    item->setText(0, "Parameter");
    item->setText(1, "Value");
    ui->treeWidget->setHeaderItem(item);
    ui->treeWidget->header()->setSectionResizeMode(QHeaderView::ResizeToContents);

    QObject::connect(&_mqtt, &MqttClientWrapper::mqtt_connected, this, &ParamViewer::setup);
}

ParamViewer::~ParamViewer()
{
    delete ui;
}

void ParamViewer::setup()
{
    auto sub = _mqtt.subscribe("param/#");
    QObject::connect(sub.get(), &MqttSubscriptionWrapper::message_received, this, &ParamViewer::mqtt_message_callback);
}

void ParamViewer::mqtt_message_callback(Mosquittopp::Message msg)
{
    insert_from_topic_and_value(QString::fromStdString(msg.topic()), QString::fromStdString(msg.payload()));
}

void ParamViewer::insert_from_topic_and_value(QString topic, QString value)
{
    QObject::disconnect(_con);

    qDebug() << topic << value;
    auto tree = ui->treeWidget;
    auto parent = tree->topLevelItem(0);

    QStringList hierarchy = topic.split("/", QString::SkipEmptyParts);

    for (int i = 0; i < hierarchy.length() - 1; ++i) {
        parent = add_or_fill(parent, hierarchy[i], "");
    }

    parent = add_or_fill(parent, hierarchy.last(), value);

    _con = QObject::connect(ui->treeWidget, &QTreeWidget::itemChanged, [](QTreeWidgetItem* item, int column) { qDebug() << item->text(0) << item->text(1) << column; });
}

QTreeWidgetItem* ParamViewer::add_or_fill(QTreeWidgetItem* parent, QString name, QString value)
{
    if (parent == nullptr) {
        auto child = new QTreeWidgetItem(parent);
        child->setText(0, name);
        if (value.length()) {
            child->setText(1, value);
            child->setFlags(child->flags() | Qt::ItemIsEditable);
        }
        ui->treeWidget->addTopLevelItem(child);
        return child;
    } else {
        if (parent->text(0) == name) {
            return parent;
        }

        auto children = parent->takeChildren();
        parent->addChildren(children);

        foreach (QTreeWidgetItem* twi, children) {
            if (twi->text(0) == name) {
                if (value.length()) {
                    twi->setText(1, value);
                }
                qDebug() << name << "found";
                return twi;
            }
        }

        qDebug() << name << "not found, creating it as a child of" << parent->text(0);
        auto child = new QTreeWidgetItem(parent);
        child->setText(0, name);
        if (value.length()) {
            child->setText(1, value);
            child->setFlags(child->flags() | Qt::ItemIsEditable);
        }
        parent->addChild(child);
        parent->sortChildren(0, Qt::AscendingOrder);
        qDebug() << parent->text(0) << "now has" << parent->childCount() << "children";
        return child;
    }
}

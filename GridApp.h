//
// Created by generalsuslik on 09.12.24.
//

#ifndef GRID_APP_H
#define GRID_APP_H

#include <QFile>
#include <QGridLayout>
#include <QLabel>
#include <QJsonObject>
#include <QJsonDocument>
#include <QtNetwork/QNetworkReply>
#include <QPushButton>
#include <QRandomGenerator>
#include <QTimer>
#include <QWidget>
#include <unordered_set>


class GridApp final : public QWidget {
    Q_OBJECT
private:
    QHBoxLayout* mainLayout;
    QVBoxLayout* leftLayout;
    QVBoxLayout* infoLayout;
    QGridLayout* gridLayout;
    QLabel* clickCounterLabel;
    QLabel* timeLabel;
    QLabel* temperatureLabel;
    QTimer* timer;
    QNetworkAccessManager* networkManager;
    int clickCounter;
    int nSquares;
    static const QString configFilePath;

public:
    static const QStringList colors;
    QString nextFreeColor;
    std::unordered_set<QString> usingColors;


public:
    explicit GridApp(QWidget* parent = nullptr);

    ~GridApp() override;

private:
    void setupUI();

    void initColors();

    void handleSquareClick();

    void updateTimeAndTemperature();

    void handleNetworkReply(QNetworkReply* reply) const;

    void fetchTemperature();

    QString getFreeColor() const;

    void updateUsingColors(const QString& addColor, const QString& removeColor);

    static QString getApiKey();
};

#include "main.moc"

#endif //GRID_APP_H

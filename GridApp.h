//
// Created by generalsuslik on 09.12.24.
//

#ifndef GRID_APP_H
#define GRID_APP_H

#include <QTimer>
#include <QGridLayout>
#include <QLabel>
#include <QtNetwork/QNetworkAccessManager>
#include <QPushButton>
#include <QRandomGenerator>
#include <QWidget>
#include <unordered_set>


class GridApp final : public QWidget {
    Q_OBJECT
private:
    QWidget* gridWidget;
    QGridLayout* gridLayout;
    QHBoxLayout* mainLayout;
    QVBoxLayout* leftLayout;
    QVBoxLayout* infoLayout;
    QLabel* clickCounterLabel;
    QLabel* timeLabel;
    QLabel* temperatureLabel;
    QTimer* timer;
    QNetworkAccessManager* networkManager;
    int clickCounter;
    int nSquares;

public:
    static const QStringList colors;
    QString nextFreeColor;
    std::unordered_set<QString> usingColors;


public:
    explicit GridApp(QWidget* parent = nullptr);

    ~GridApp() override;

private:
    void setupUI();

    void handleSquareClick();

    void updateTimeAndTemperature() const;

    void initColors();

    QString getFreeColor() const;

    void updateUsingColors(const QString& addColor, const QString& removeColor);
};

#include "main.moc"

#endif //GRID_APP_H

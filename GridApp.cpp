//
// Created by generalsuslik on 09.12.24.
//

#include "GridApp.h"

#include <iostream>

const QStringList GridApp::colors = {
    "white",
    "black",
    "brown",
    "blue",
    "cyan",
    "red",
    "yellow",
    "green",
    "gray",
    "pink"
};

const QString GridApp::configFilePath = "/home/generalsuslik/CLionProjects/test-task02/config.json";

GridApp::GridApp(QWidget* parent)
        : QWidget(parent)
        , mainLayout(new QHBoxLayout(this))
        , leftLayout(new QVBoxLayout())
        , infoLayout(new QVBoxLayout())
        , gridLayout(new QGridLayout())
        , clickCounterLabel(new QLabel("Clicks: 0", this))
        , timeLabel(new QLabel("Time: ", this))
        , temperatureLabel(new QLabel("Temperature: ", this))
        , timer(new QTimer(this))
        , networkManager(new QNetworkAccessManager(this))
        , clickCounter(0)
        , nSquares(3) {
    setupUI();
    initColors();
    updateTimeAndTemperature();
}

GridApp::~GridApp() {
    std::cout << gridLayout->count() << std::endl;
    for (int i = 0; i < gridLayout->count(); ++i) {
        const auto square = qobject_cast<QPushButton*>(gridLayout->itemAt(i)->widget());
        delete square;
    }
    delete networkManager;
    delete timer;
    delete temperatureLabel;
    delete timeLabel;
    delete clickCounterLabel;
    delete gridLayout;
    delete infoLayout;
    delete leftLayout;
    delete mainLayout;
}

void GridApp::setupUI() {
    gridLayout->setSpacing(5);

    for (int i = 0; i < nSquares; ++i) {
        for (int j = 0; j < nSquares; ++j) {
            const auto square = new QPushButton();
            square->setMinimumSize(100, 100);
            gridLayout->addWidget(square, i, j);
            connect(square, &QPushButton::clicked, this, &GridApp::handleSquareClick);
        }
    }
    leftLayout->addLayout(gridLayout);

    clickCounterLabel->setAlignment(Qt::AlignCenter);
    leftLayout->addWidget(clickCounterLabel);

    mainLayout->addLayout(leftLayout);

    infoLayout->addWidget(timeLabel);
    infoLayout->addWidget(temperatureLabel);
    infoLayout->setAlignment(Qt::AlignCenter);

    mainLayout->addLayout(infoLayout);

    connect(timer, &QTimer::timeout, this, &GridApp::updateTimeAndTemperature);
    timer->start(100);
}

void GridApp::initColors() {
    const int nColors = nSquares * nSquares + 1;

    std::unordered_set<int> usedColors;
    for (int i = 0; i < gridLayout->count(); ++i) {
        auto *square = qobject_cast<QPushButton *>(gridLayout->itemAt(i)->widget());
        if (square) {
            int colorIndex = QRandomGenerator::global()->bounded(nColors);
            while(usingColors.contains(colors.at(colorIndex))) {
                colorIndex = QRandomGenerator::global()->bounded(nColors);
            }
            usingColors.insert(colors.at(colorIndex));
            const QString& color = colors.at(colorIndex);
            square->setStyleSheet("background-color: " + color);
            square->setText(color);
        }
    }
    nextFreeColor = getFreeColor();
}

void GridApp::handleSquareClick() {
    const auto square = qobject_cast<QPushButton*>(sender());
    if (!square) {
        return;
    }
    clickCounterLabel->setText("Clicks: " + QString::number(++clickCounter));
    const QString currFreeColor = nextFreeColor;
    nextFreeColor = square->text();
    square->setStyleSheet("background-color: " + currFreeColor);
    square->setText(currFreeColor);
    updateUsingColors(currFreeColor, nextFreeColor);
}

void GridApp::updateTimeAndTemperature() {
    const QString currentTime = QDateTime::currentDateTime().toString("hh:mm:ss");
    timeLabel->setText("Time: " + currentTime);

    const QString city = "Moscow";
    const QUrl url("https://api.weatherapi.com/v1/current.json?key=" + getApiKey() + "&q=" + city + "&aqi=no");

    QNetworkRequest request;
    request.setUrl(url);

    QNetworkReply* reply = networkManager->get(request);
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        this->handleNetworkReply(reply);
    });
}

void GridApp::handleNetworkReply(QNetworkReply* reply) const {
     if (reply->error() != QNetworkReply::NoError) {
         temperatureLabel->setText("Network Error");
     }
     const QByteArray response = reply->readAll();
     const QJsonDocument jsonDoc = QJsonDocument::fromJson(response);

     if (!jsonDoc.isNull()) {
         const QJsonObject jsonObj = jsonDoc.object();
         if (jsonObj.contains("current") && jsonObj["current"].isObject()) {
             const double temperature = jsonObj["current"].toObject()["temp_c"].toDouble();
             temperatureLabel->setText("Temperature: " + QString::number(temperature));
         } else {
             temperatureLabel->setText("Temperature data unavailable");
         }
     } else {
         temperatureLabel->setText("Error passing response");
     }
}

QString GridApp::getFreeColor() const {
    for (const auto& color : colors) {
        if (!usingColors.contains(color)) {
            return color;
        }
    }
    return "";
}

// remove removeColor from usingColors. Replace it with addColor
void GridApp::updateUsingColors(const QString& addColor, const QString& removeColor) {
    usingColors.erase(removeColor);
    usingColors.insert(addColor);
}

QString GridApp::getApiKey() {
    QFile file(configFilePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Could not load config file";
        return "";
    }

    const QByteArray data = file.readAll();
    const QJsonDocument doc = QJsonDocument::fromJson(data);
    const QJsonObject json = doc.object();
    return json.value("weather_api_key").toString();
}


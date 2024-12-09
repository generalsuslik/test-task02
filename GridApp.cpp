//
// Created by generalsuslik on 09.12.24.
//

#include "GridApp.h"

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

GridApp::GridApp(QWidget* parent)
        : QWidget(parent)
        , gridWidget(new QWidget(this))
        , gridLayout(new QGridLayout(gridWidget))
        , mainLayout(new QHBoxLayout(this))
        , leftLayout(new QVBoxLayout())
        , infoLayout(new QVBoxLayout())
        , clickCounterLabel(new QLabel("Clicks: 0", this))
        , timeLabel(new QLabel("Time: ", this))
        , temperatureLabel(new QLabel("Temperature: ", this))
        , timer(new QTimer(this))
        , networkManager(new QNetworkAccessManager(this))
        , clickCounter(0)
        , nSquares(3) {
    setupUI();
    initColors();
}

GridApp::~GridApp() {
    for (int i = 0; i < gridLayout->count(); ++i) {
        const auto square = qobject_cast<QPushButton*>(gridLayout->itemAt(i)->widget());
        delete square;
    }

    delete gridWidget;
    delete gridLayout;
    delete mainLayout;
    delete leftLayout;
    delete infoLayout;
    delete clickCounterLabel;
    delete timeLabel;
    delete temperatureLabel;
    delete timer;
    delete networkManager;
}

void GridApp::setupUI()  {
    gridLayout->setSpacing(5);

    for (int i = 0; i < nSquares; ++i) {
        for (int j = 0; j < nSquares; ++j) {
            const auto square = new QPushButton(gridWidget);
            square->setMinimumSize(100, 100);
            gridLayout->addWidget(square, i, j);
            connect(square, &QPushButton::clicked, this, &GridApp::handleSquareClick);
        }
    }
    leftLayout->addWidget(gridWidget);

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

void GridApp::handleSquareClick() {
    const auto square = qobject_cast<QPushButton*>(sender());
    if (!square) {
        return;
    }
    clickCounterLabel->setText("Clicks: " + QString::number(++clickCounter));
    const QString currFreeColor = nextFreeColor;
    nextFreeColor = square->text();
    square->setStyleSheet("background-color: " + currFreeColor + "; color: transparent;");
    square->setText(currFreeColor);
    updateUsingColors(currFreeColor, nextFreeColor);
}

void GridApp::updateTimeAndTemperature() const {
    const QString currentTime = QDateTime::currentDateTime().toString("hh:mm:ss");
    timeLabel->setText("Time: " + currentTime);
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
            square->setStyleSheet("background-color: " + color + "; color: transparent;");
            square->setText(color);
        }
    }
    nextFreeColor = getFreeColor();
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

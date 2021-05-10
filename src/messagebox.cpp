/**
 * @file MessageBox.cpp
 * @brief This file contains the definitions of the members of the MessageBox class
 * @author Pablo Deputter
 * @date 09/05/2021
 */

#include "messagebox.h"

void MessageBox::showEvent(QShowEvent *event)  {

    this->time = 0;
    if (autoClose) {
        this->startTimer(1000);
    }
}

void MessageBox::timerEvent(QTimerEvent *event) {

    time++;
    if (time>=timeout) {
        this->done(0);
    }
}

void MessageBox::setStyleSheet_() {

    QString x = "color: #a9b7c6;\nbackground-color: #1e1d23";
    this->setWindowTitle("VaccinDistributor");
    this->setStyleSheet(x);
    this->setStandardButtons(QMessageBox::NoButton);
    this->setIcon(QMessageBox::NoIcon);
}


#ifndef MESSAGEBOX_H
#define MESSAGEBOX_H

#include <QMessageBox>
#include <string>

class MessageBox : public QMessageBox {
public:
    int timeout;
    bool autoClose;
    int time;

public:
    void showEvent(QShowEvent *event) override;

    void timerEvent(QTimerEvent *event) override;

    void setStyleSheet_();
};

#endif // MESSAGEBOX_H

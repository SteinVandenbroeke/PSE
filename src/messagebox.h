/**
 * @file MessageBox.h
 * @brief This header file contains the declarations and the members of the MessageBox class
 * @author Pablo Deputter
 * @date 09/05/2021
 */

#ifndef MESSAGEBOX_H
#define MESSAGEBOX_H

#include <QMessageBox>
#include <string>

/**
 * @brief The MessageBox class inherited from QMessageBox
 */
class MessageBox : public QMessageBox {
public:
    int timeout; ///< Current time as int
    bool autoClose; ///< Auto close QMessageBox object as bool
    int time; ///< Max time QMessageBox can be open

public:
    /**
     * @brief Override showEvent member to add timer
     *
     * @param event Event type object
     */
    void showEvent(QShowEvent *event) override;

    /**
     * @brief Override timerEvent to increase timer while time < time
     *
     * @param event Event type object
     */
    void timerEvent(QTimerEvent *event) override;

    /**
     * @brief Set standard syleSheet for QMessageBox
     */
    void setStyleSheet_();
};

#endif // MESSAGEBOX_H

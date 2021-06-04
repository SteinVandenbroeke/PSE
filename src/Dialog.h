#ifndef DIALOG_H
#define DIALOG_H

#include <QCloseEvent>
#include <QMessageBox>
#include <QtWidgets>
#include <QtGui>
#include <iostream>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QPixmap>
#include <QtConcurrent/QtConcurrent>
#include <ostream>
#include <QDialog>
#include "messagebox.h"
#include "Simulation.h"

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief Constructor Dialog
     *
     * @param parent: parant of dialog
     */
    explicit Dialog(QWidget *parent = nullptr);

    /**
     * @brief Destructor Dialog
     *
     * @pre
     * REQUIRE(properlyInitialized(), "MainWindow object must be properly initialized")
     */
    ~Dialog();

    /**
     * @brief Add centra's & hubs to the interface
     *
     * @param centra all centra's to display
     * @param all hubsto display
     *
     * @pre
     * REQUIRE(properlyInitialized(), "MainWindow object must be properly initialized")
     */
    void createModels(const std::map<std::string, VaccinationCenter*> &centra,
                      const std::vector<Hub*> &hubs);

    /**
     * @brief Check if MainWindow object is correctly inialized
     *
     * @return true if success
     */
    bool properlyInitialized() const;

private slots:

    /**
     * @brief doubleClicked on centre item
     *
     * @param index: index of clicked element
     *
     * @pre
     * REQUIRE(properlyInitialized(), "MainWindow object must be properly initialized")
     */
    void on_listView_doubleClicked(const QModelIndex &index);

    /**
     * @brief doubleClicked on hub
     *
     * @param index: index of clicked element
     *
     * @pre
     * REQUIRE(properlyInitialized(), "MainWindow object must be properly initialized")
     */
    void on_listView_2_doubleClicked(const QModelIndex &index);

    /**
     * @brief Close dialog
     *
     * @pre
     * REQUIRE(properlyInitialized(), "MainWindow object must be properly initialized")
     */
    void on_buttonStop_clicked();

    /**
     * @brief Create new transport
     *
     * @pre
     * REQUIRE(properlyInitialized(), "MainWindow object must be properly initialized")
     */
    void on_buttonCreateTransport_clicked();

private:
    Ui::Dialog *ui;
    Dialog *_initCheck;
    QStringListModel *modelCentra; ///< Hold the centraData
    QStringListModel *modelHubs; ///< Hold the hubData
    std::vector<Hub*> hubs; ///< Hubs
    std::map<std::string, VaccinationCenter*> centra; ///< Centra
    std::vector<VaccinationCenter*> centraIndex; ///< Hold the centra Index
    std::vector<std::pair<int, VaccinInHub*>> hubsIndex; ///< Hold the hubs Index

    /**
     * @brief Add new centra to interface list, and needed variabels
     *
     * @param centra: all centra's to add to this dialog
     *
     * @pre
     * REQUIRE(properlyInitialized(), "MainWindow object must be properly initialized")
     */
    void createCentra(const std::map<std::string, VaccinationCenter*> &centra);

    /**
     * @brief Add new hubs to interface list, and needed variabels
     *
     * @param hubs: all hubs to add to this dialog
     *
     * @pre
     * REQUIRE(properlyInitialized(), "MainWindow object must be properly initialized")
     */
    void createHubs(const std::vector<Hub*> &hubs);

    /**
     * @brief select hub with title from given vector
     *
     * @param hubs: hubs to search in
     * @param title: title of hub to find
     *
     * @pre
     * REQUIRE(properlyInitialized(), "MainWindow object must be properly initialized")
     */
    Hub* selectHub(std::vector<Hub*> hubs, std::string& title);

    /**
     * @brief Select vaccin from hub by given title
     *
     * @param hub: hubs to search in
     * @title title: title of vaccin
     *
     * @pre
     * REQUIRE(properlyInitialized(), "MainWindow object must be properly initialized")
     */
    VaccinInHub* selectVaccin(Hub*, std::string& title);

    /**
     * @brief Select center from hub by given title
     *
     * @param hub: hubs to search in
     * @title title: title of center
     *
     * @pre
     * REQUIRE(properlyInitialized(), "MainWindow object must be properly initialized")
     */
    VaccinationCenter* selectCenter(Hub* hub, std::string& title);
};

#endif // DIALOG_H

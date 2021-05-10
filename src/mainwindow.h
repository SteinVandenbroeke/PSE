/**
 * @file MainWindow.h
 * @brief This header file contains the declarations and the members of the MainWindow class
 * @author Pablo Deputter
 * @date 09/05/2021
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
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
#include "messagebox.h"
#include "Simulation.h"

/**
 * @brief Namespace Ui holding forwward declaration of MainWindow class
 */
namespace Ui {
class MainWindow;
}

/**
 * @brief The MainWindow class implemented for MainWindow of program inherited from QMainWindow
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

    bool pauseSimulation = false;
    bool autoSimulation = false;
public:
    /**
     * @brief Default constructor for MainWindow object
     *
     * @param parent Pointer to QWidget, std = nullptr
     *
     * @post
     * ENSURE(properlyInitialized(), "MainWindow object must be properly initialized")
     */
    explicit MainWindow(QWidget *parent = nullptr);

    /**
     * @brief Default deconstructor for MainWindow object
     *
     * @pre
     * REQUIRE(properlyInitialized(), "MainWindow object must be properly initialized")
     */
    ~MainWindow();

    /**
     * @brief Check if MainWindow object is correctly inialized
     *
     * @return true if success
     */
    bool properlyInitialized() const;

private slots:
    /**
     * @brief on_actionOpen_triggered Open file with asked format to import Simulation
     *
     * @pre
     * REQUIRE(properlyInitialized(), "MainWindow object must be properly initialized")
     */
    void on_actionOpen_triggered();

    /**
     * @brief on_buttonStart_clicked Start / import Simulation
     *
     * @pre
     * REQUIRE(properlyInitialized(), "MainWindow object must be properly initialized")
     */
    void on_buttonStart_clicked();

    /**
     * @brief on_buttonStop_clicked Stop Simulation and clear everything
     *
     * @pre
     * REQUIRE(properlyInitialized(), "MainWindow object must be properly initialized")
     */
    void on_buttonStop_clicked();

    /**
     * @brief on_buttoNext_clicked Go to next Simulation
     *
     * @pre
     * REQUIRE(properlyInitialized(), "MainWindow object must be properly initialized")
     */
    void on_buttoNext_clicked();

    /**
     * @brief on_buttonPrevious_clicked Go to previous Simulation
     *
     * @pre
     * REQUIRE(properlyInitialized(), "MainWindow object must be properly initialized")
     */
    void on_buttonPrevious_clicked();

    /**
     * @brief on_action_ini_triggered Delete all *.ini files in cwd
     *
     * @pre
     * REQUIRE(properlyInitialized(), "MainWindow object must be properly initialized")
     */
    void on_action_ini_triggered();

    /**
     * @brief on_action_ini_triggered Delete all *.bmp files in cwd
     *
     * @pre
     * REQUIRE(properlyInitialized(), "MainWindow object must be properly initialized")
     */
    void on_action_bmp_triggered();

    /**
     * @brief buttonAutoSimulation click / start auto simulation with element 'Days' days and element 'Delay' time between days
     *
     * @pre
     * REQUIRE(properlyInitialized(), "MainWindow object must be properly initialized")
     */
    void on_buttonAutoSimulation_clicked();

    /**
    * @brief buttonAutoSimulationPausePlay  click pause/un pause the auto simulation
    *
    * @pre
    * REQUIRE(properlyInitialized(), "MainWindow object must be properly initialized")
    */
    void on_buttonAutoSimulationPausePlay_clicked();

private:
    Ui::MainWindow *ui;
    QStringListModel *modelVaccins; ///< Hold the vaccinData
    bool runSimulation; ///< Can the simulation be started
    Simulation s; ///< Simulation object
    MainWindow *_initCheck;

    /**
     * @brief Create menu's
     *
     * @pre
     * REQUIRE(properlyInitialized(), "MainWindow object must be properly initialized")
     */
    void createMenus();
    /**
     * @brief Create action
     *
     * @pre
     * REQUIRE(properlyInitialized(), "MainWindow object must be properly initialized")
     */
    void createActions();
    /**
     * @brief Create models
     *
     * @pre
     * REQUIRE(properlyInitialized(), "MainWindow object must be properly initialized")
     */
    void createModels();
    /**
     * @brief Update textEdit box
     *
     * @param x QString top be appended to textEdit box
     *
     * @pre
     * REQUIRE(properlyInitialized(), "MainWindow object must be properly initialized")
     */
    void updateTextEdit(const QString &x);
    /**
     * @brief Update labelImage
     *
     * @param fileName Name of file with to image
     *
     * @pre
     * REQUIRE(properlyInitialized(), "MainWindow object must be properly initialized")
     */
    void updateLabelImage(const QString &fileName);
    /**
     * @brief Update progressBarVaccinated
     *
     * @param x New percent of progressBar as int
     *
     * @pre
     * REQUIRE(properlyInitialized(), "MainWindow object must be properly initialized")
     */
    void updateProgressBarVaccinated(const int x);
    /**
     * @brief Update models
     *
     * @param vaccins Map with type of Vaccin and amount of total delivered Vaccins
     *
     * @pre
     * REQUIRE(properlyInitialized(), "MainWindow object must be properly initialized")
     */
    void updateModels(const std::map<const std::string, int> &vaccins);
};

#endif // MAINWINDOW_H

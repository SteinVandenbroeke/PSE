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
    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog();

    void createModels(const std::map<std::string, VaccinationCenter*> &centra,
                      const std::vector<Hub*> &hubs);
private slots:

    void on_listView_doubleClicked(const QModelIndex &index);

    void on_listView_2_doubleClicked(const QModelIndex &index);

    void on_buttonStop_clicked();

    void on_buttonCreateTransport_clicked();

private:
    Ui::Dialog *ui;
    QStringListModel *modelCentra; ///< Hold the centraData
    QStringListModel *modelHubs; ///< Hold the hubData
    std::vector<Hub*> hubs; ///< Hubs
    std::map<std::string, VaccinationCenter*> centra; ///< Centra
    std::vector<VaccinationCenter*> centraIndex; ///< Hold the centra Index
    std::vector<std::pair<int, VaccinInHub*>> hubsIndex; ///< Hold the hubs Index

    void createCentra(const std::map<std::string, VaccinationCenter*> &centra);
    void createHubs(const std::vector<Hub*> &hubs);

    Hub* selectHub(std::vector<Hub*>, std::string& titel);
    VaccinInHub* selectVaccin(Hub*, std::string& titel);
    VaccinationCenter* selectCenter(Hub*, std::string& titel);
};

#endif // DIALOG_H

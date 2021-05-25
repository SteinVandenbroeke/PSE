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

private:
    Ui::Dialog *ui;
    QStringListModel *modelCentra; ///< Hold the centraData
    QStringListModel *modelHubs; ///< Hold the hubData

    void createCentra(const std::map<std::string, VaccinationCenter*> &centra);
    void createHubs(const std::vector<Hub*> &hubs);
};

#endif // DIALOG_H

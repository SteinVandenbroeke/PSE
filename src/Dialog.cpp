#include "Dialog.h"
#include "ui_Dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    this->setWindowTitle("VaccinDistributor");
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::createModels(const std::map<std::string, VaccinationCenter*> &centra,
                          const std::vector<Hub*> &hubs) {

    Dialog::createCentra(centra);
    Dialog::createHubs(hubs);



//    QInputDialog chooseCentra;
//    QString centraChose = chooseCentra.getItem(this, "Select your center!", "Center: ", centra);
}

void Dialog::createCentra(const std::map<std::string, VaccinationCenter*> &centra) {

    modelCentra = new QStringListModel(this);

    // Items cannot be manually updated
    ui->listView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->listView->setAcceptDrops(false);
    ui->listView->setModel(modelCentra);

    QStringList list;
    for (std::map<std::string, VaccinationCenter*>::const_iterator it = centra.begin(); it != centra.end(); it++) {
        QString data = QString::fromStdString(it->first);
        data.append(tr("\n"));
        data.append(tr("\t"));
        data.append(tr("- capaciteit: "));
        data.append(QString::fromStdString(ToString(it->second->getCapacity())));
        data.append(tr("\n"));

        int perVaccin = ToPercent(it->second->getVaccins(), it->second->getCapacity());
        int perVaccinated = ToPercent(it->second->getVaccinated(), it->second->getPopulation());
        if (perVaccin > 100) {
            perVaccin = 100;
        }
        QString progressVaccinated = QString::fromStdString(ProgressBar(perVaccinated, 20));
        QString progressVaccin = QString::fromStdString(ProgressBar(perVaccin, 20));
        data.append(tr("\t"));
        data.append(tr("- geavaccineerd "));
        data.append(progressVaccinated);
        data.append(tr(" "));
        data.append(QString::fromStdString(ToString(perVaccinated)));
        data.append(tr("%: "));
        data.append(QString(it->second->getVaccinated()));
        data.append(tr(" / "));
        data.append(QString::fromStdString(ToString(it->second->getPopulation())));
        data.append(tr("\n"));
        data.append(tr("\t"));
        data.append(tr("- vaccins       "));
        data.append(progressVaccin);
        data.append(tr(" "));
        data.append(QString::fromStdString(ToString(perVaccin)));
        data.append(tr("%"));
        data.append(tr("\n"));

        list.append(data);
    }
    modelCentra->setStringList(list);
}

void Dialog::createHubs(const std::vector<Hub*> &hubs) {

    modelHubs = new QStringListModel(this);
    // Items cannot be manually updated
    ui->listView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->listView->setAcceptDrops(false);
    ui->listView->setModel(modelCentra);

    QStringList list;
    for (std::vector<Hub*>::const_iterator it = hubs.begin(); it != hubs.end(); it++) {

    }
    modelHubs->setStringList(list);
}

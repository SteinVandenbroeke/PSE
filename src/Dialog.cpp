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

void Dialog::createModels(const std::map<std::string, VaccinationCenter*> &center,
                          const std::vector<Hub*> &hub) {

    modelCentra = new QStringListModel(this);

    // Items cannot be manually updated
    ui->listView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->listView->setAcceptDrops(false);
    ui->listView->setModel(modelCentra);
    Dialog::createCentra(center);
    centra = center;

    modelHubs = new QStringListModel(this);

    // Items cannot be manually updated
    ui->listView_2->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->listView_2->setAcceptDrops(false);
    ui->listView_2->setModel(modelHubs);
    Dialog::createHubs(hub);
    hubs = hub;
}

void Dialog::createCentra(const std::map<std::string, VaccinationCenter*> &centra) {

    QStringList list;
    for (std::map<std::string, VaccinationCenter*>::const_iterator it = centra.begin(); it != centra.end(); it++) {
        centraIndex.emplace_back(it->second);
        QString data = QString::fromStdString(it->first);
        data.append(tr(": "));
        data.append(tr("\n"));
        data.append(tr("\t"));
        data.append(tr("- Capacity: "));
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
        data.append(tr("- Vaccinated: "));
        data.append(progressVaccinated);
        data.append(tr(" "));
        data.append(QString::fromStdString(ToString(perVaccinated)));
        data.append(tr("%: "));
        data.append(QString::fromStdString((ToString(it->second->getVaccinated()))));
        data.append(tr(" / "));
        data.append(QString::fromStdString(ToString(it->second->getPopulation())));
        data.append(tr("\n"));
        data.append(tr("\t"));
        data.append(tr("- Vaccins:       "));
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

    QStringList list;
    int counter = 0;
    for (std::vector<Hub*>::const_iterator it = hubs.begin(); it != hubs.end(); it++) {
        QString data = tr("Hub-");
        data.append(QString::fromStdString(ToString(counter)));
        data.append(tr(": "));
        data.append(tr("\n"));
        for (std::map<std::string, VaccinInHub*>::const_iterator ite = (*it)->getVaccins().begin(); ite != (*it)->getVaccins().end(); ite++) {
            hubsIndex.emplace_back(std::make_pair(counter, ite->second));
            QString dataVaccin;
            if (ite == (*it)->getVaccins().begin()) dataVaccin = data;
            dataVaccin.append(tr("\t - "));
            dataVaccin.append(QString::fromStdString(ite->second->getType()));
            dataVaccin.append(tr(": "));
            dataVaccin.append(tr("\n"));
            dataVaccin.append(tr("\t   ° Delivery:\t"));
            dataVaccin.append(QString::fromStdString(ToString(ite->second->getDelivery())));
            dataVaccin.append(tr("\n"));
            dataVaccin.append(tr("\t   ° Interval:\t"));
            dataVaccin.append(QString::fromStdString(ToString(ite->second->getInterval())));
            dataVaccin.append(tr("\n"));
            dataVaccin.append(tr("\t   ° Renewal:\t"));
            dataVaccin.append(QString::fromStdString(ToString(ite->second->getRenewal())));
            dataVaccin.append(tr("\n"));
            dataVaccin.append(tr("\t   ° Transport:\t"));
            dataVaccin.append(QString::fromStdString(ToString(ite->second->getTransport())));
            dataVaccin.append(tr("\n"));
            dataVaccin.append(tr("\t   ° Stock:\t"));
            dataVaccin.append(QString::fromStdString(ToString(ite->second->getVaccin())));
            dataVaccin.append(tr("\n"));
            dataVaccin.append(tr("\n"));
            list.append(dataVaccin);
        }
        counter++;
    }
    modelHubs->setStringList(list);
}

void Dialog::on_listView_doubleClicked(const QModelIndex &index) {

    QString msg;
    msg.append(QString::fromStdString(centraIndex[(unsigned int) index.row()]->getName()));
    int newCapacity = QInputDialog::getInt(this, msg, tr("New Capacity: "), centraIndex[(unsigned int) index.row()]->getCapacity());
    int minNeededCapacity = centraIndex[(unsigned int) index.row()]->getVaccins()/2;

    bool change = true;
    if(newCapacity <= minNeededCapacity){
        QMessageBox::StandardButton warn;
        warn = QMessageBox::warning(this, "Capacity to small"
                                      , "Capacity is smaller then amount of vaccins current in centra. Do you want to vaccinate until there is no more overstock?"
                                      ,QMessageBox::Yes|QMessageBox::No);
        if(warn == QMessageBox::No){
            change = false;
        }
        else if(warn == QMessageBox::Yes){
            int peapleVaccinated = centraIndex[(unsigned int) index.row()]->getVaccinated();
            while(newCapacity <= minNeededCapacity){
                std::stringstream ostream;
                centraIndex[(unsigned int) index.row()]->vaccinateCenter(ostream);
                minNeededCapacity = centraIndex[(unsigned int) index.row()]->getVaccins()/2;
            }
            peapleVaccinated = centraIndex[(unsigned int) index.row()]->getVaccinated() - peapleVaccinated;
            std::string messageBoxText =  "There are " + std::to_string(peapleVaccinated) + "peaple vaccinated";
            QMessageBox::information(this, "Capacity change", messageBoxText.c_str());
        }
    }

    if (newCapacity && newCapacity >= 0 && change) {
        centraIndex[(unsigned int) index.row()]->setCapacity(newCapacity);
        createCentra(centra);
        return;
    }
    if (newCapacity < 0) {
        const char* errMsg = "Capacity must be a positive number";

        MessageBox msg;
        msg.setWindowTitle("Error");
        msg.setText(errMsg);
        msg.setStyleSheet_();
        msg.setIcon(QMessageBox::Critical);
        msg.setStandardButtons(QMessageBox::Ok);
        msg.autoClose = true;
        msg.timeout = 1;
        msg.exec();
        on_listView_doubleClicked(index);
    }
}

void Dialog::on_listView_2_doubleClicked(const QModelIndex &index) {

    QString msg = tr("Hub-");
    msg.append(QString::fromStdString(ToString(hubsIndex[(unsigned int) index.row()].first)));
    msg.append(tr(" "));
    msg.append(QString::fromStdString(hubsIndex[(unsigned int) index.row()].second->getType()));
    QStringList items;
    items << tr("Delivery") << tr("Interval") << tr("Renewal") << tr("Transport");

    bool ok;
    QString item = QInputDialog::getItem(this, msg, tr("Item: "), items, 1, -20000, &ok);
    if (ok && !item.isEmpty()) {
        int newValue = QInputDialog::getInt(this, tr("New value for ") + item, tr("New ") + item + tr(": "));
        if (newValue && newValue >= 0) {

            if (item == tr("Delivery")) hubsIndex[(unsigned int) index.row()].second->setDelivery(newValue);
            else if (item == tr("Interval")) hubsIndex[(unsigned int) index.row()].second->setInterval(newValue);
            else if (item == tr("Renewal")) hubsIndex[(unsigned int) index.row()].second->setRenewal(newValue);
            else if (item == tr("Transport")) hubsIndex[(unsigned int) index.row()].second->setTransport(newValue);
            createHubs(hubs);
            return;
        }
        if (newValue < 0) {
            // Convert QString to const char*
            QString errMsg = item + tr(" must be a positive number");

            MessageBox msg;
            msg.setWindowTitle("Error");
            msg.setText(errMsg);
            msg.setStyleSheet_();
            msg.setIcon(QMessageBox::Critical);
            msg.setStandardButtons(QMessageBox::Ok);
            msg.autoClose = true;
            msg.timeout = 1;
            msg.exec();
            on_listView_2_doubleClicked(index);
        }
    }
    return;
}

void Dialog::on_buttonStop_clicked() {
    this->close();
}

void Dialog::on_buttonCreateTransport_clicked() {
    std::string titel = "Create transport";
    Hub* hub = selectHub(hubs, titel);
    VaccinInHub* vaccin = selectVaccin(hub, titel);
    VaccinationCenter* centra = selectCenter(hub, titel);
    int transport = QInputDialog::getInt(this, titel.c_str(), "Vaccin amount:",centra->getOpenVaccinStorage(vaccin), 0, centra->getOpenVaccinStorage(vaccin));
    //transport
    int roundedToTransport = std::floor(transport/vaccin->getTransport());
    transport = vaccin->getTransport() * roundedToTransport;
    vaccin->updateVaccinsTransport(transport);
    centra->addVaccins(transport, vaccin);
    return;
}

Hub *Dialog::selectHub(std::vector<Hub *> hubs, std::string& titel) {
    QStringList items;
    for(int i = 0; i < hubs.size(); i++){
        items << tr(std::string("Hub-" + std::to_string(i)).c_str());
    }
    bool ok;
    QString item = QInputDialog::getItem(this, titel.c_str(), tr("Select hub: "), items, 0, false, &ok);
    item.remove(0,4);
    std::istringstream convert(item.toStdString());
    int hubIndex;
    convert >> hubIndex;
    return hubs[hubIndex];
}

VaccinInHub* Dialog::selectVaccin(Hub* hub, std::string& titel) {
    QStringList items;
    std::map<std::string, VaccinInHub *> vaccins = hub->getVaccins();
    for(std::map<std::string, VaccinInHub *>::iterator it = vaccins.begin();
        it != vaccins.end(); it++){
        items << tr(std::string((*it).first).c_str());
    }
    bool ok;
    QString item = QInputDialog::getItem(this, titel.c_str(), tr("Select a vaccin: "), items, 0, false, &ok);
    return vaccins[item.toStdString()];
}

VaccinationCenter *Dialog::selectCenter(Hub* hub, std::string &titel) {
    QStringList items;
    std::map<std::string, VaccinationCenter *> centra = hub->getCentra();
    for(std::map<std::string, VaccinationCenter *>::iterator it = centra.begin();
        it != centra.end(); it++){
        items << tr(std::string((*it).first).c_str());
    }
    bool ok;
    QString item = QInputDialog::getItem(this, titel.c_str(), tr("Select an center: "), items, 0, false, &ok);
    return centra[item.toStdString()];
}
/**
 * @file MainWindow.cpp
 * @brief This file contains the definitions of the members of the MainWindow class
 * @author Pablo Deputter
 * @date 09/05/2021
 */

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    _initCheck = this;
    ui->setupUi(this);
    this->setWindowTitle("VaccinDistributor");

    createMenus();
    createActions();
    createModels();

    changeStateButtons(false);

    ENSURE(properlyInitialized(), "MainWindow object must be properly initialized");
}

MainWindow::~MainWindow()
{
    REQUIRE(properlyInitialized(), "MainWindow object must be properly initialized");

    delete ui;
}

bool MainWindow::properlyInitialized() const {

    return MainWindow::_initCheck == this;
}

void MainWindow::createMenus() {

    REQUIRE(properlyInitialized(), "MainWindow object must be properly initialized");

    // CentralWidget
    this->setCentralWidget(ui->centralwidget);
    menuBar()->setNativeMenuBar(false);

    // TextEdit
    ui->textEdit->setReadOnly(true);

    // progressBarVaccinated
    ui->progressBarVaccinated->setValue(0);
    ui->buttonAutoSimulationPausePlay->setVisible(false);
}

void MainWindow::createActions() {

    REQUIRE(properlyInitialized(), "MainWindow object must be properly initialized");
//    QObject::connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(&MainWindow::on_actionOpen_triggered()));
}

void MainWindow::createModels() {

    REQUIRE(properlyInitialized(), "MainWindow object must be properly initialized");

    modelVaccins = new QStringListModel(this);

    // Items cannot be manually updated
    ui->listView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->listView->setAcceptDrops(false);
    ui->listView->setModel(modelVaccins);
}

void MainWindow::on_actionOpen_triggered()
{
    REQUIRE(properlyInitialized(), "MainWindow object must be properly initialized");

    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QString(),
                                                    tr("XML files (*.xml);;Images (*.png *.bmp *.jpeg *.gif)"));

    QFile file(fileName);
    if (!fileName.isEmpty() && file.open(QFile::ReadOnly)) {
        // Convert QString to const char*
       QByteArray ba = fileName.toLocal8Bit();
        try {
            s.importXmlFile(ba.data());
            ui->buttonStart->setEnabled(true);
        }
        catch (Exception ex) {
            MessageBox msg;
            msg.setWindowTitle("Error");
            msg.setText(ex.value().c_str());
            msg.setStyleSheet_();
            msg.setIcon(QMessageBox::Critical);
            msg.setStandardButtons(QMessageBox::Ok);
            msg.autoClose = true;
            msg.timeout = 2;
            msg.exec();
            return;
        }
    }
    // Show errMsg when file is corrupted || wrong format
    else {
        const char* errMsg = "Could not open file, try again!";

        MessageBox msg;
        msg.setWindowTitle("Error");
        msg.setText(errMsg);
        msg.setStyleSheet_();
        msg.setIcon(QMessageBox::Critical);
        msg.setStandardButtons(QMessageBox::Ok);
        msg.autoClose = true;
        msg.timeout = 2;
        msg.exec();
        return;
    }
    file.close();
}

void MainWindow::on_buttonStart_clicked()
{
    REQUIRE(properlyInitialized(), "MainWindow object must be properly initialized");

    // show errMsg when no .xml file was imported || simulation is not complete
    if (!s.checkSimulation() || !s.properlyInitialized()) {
        const char* errMsg = "Could not run simulation!\nTry loading a new .xml file.";

        MessageBox msg;
        msg.setWindowTitle("Error");
        msg.setText(errMsg);
        msg.setStyleSheet_();
        msg.setIcon(QMessageBox::Critical);
        msg.setStandardButtons(QMessageBox::Ok);
        msg.autoClose = true;
        msg.timeout = 2;
        msg.exec();
        return;
    }

    this->runSimulation = true;
    changeStateButtons(true);
    ui->buttonPrevious->setEnabled(false);

    MessageBox msg;
    msg.setWindowTitle("VaccinDistributor");
    msg.setText("Simulation file was successfully imported!");
    msg.setStyleSheet_();
    msg.setStandardButtons(QMessageBox::Ok);
    msg.autoClose = true;
    msg.timeout = 2;
    msg.exec();

    updateModels(s.getVaccinData());
    updateProgressBarVaccinated(s.getVaccinatedPercent());
}

void MainWindow::on_buttonStop_clicked()
{
    REQUIRE(properlyInitialized(), "MainWindow object must be properly initialized");

    ui->textEdit->clear();
    ui->labelImage->clear();
    ui->progressBarVaccinated->setValue(0);
    QStringList x;
    modelVaccins->setStringList(x);
    on_action_bmp_triggered();
    on_action_ini_triggered();
    this->runSimulation = false;
    changeStateButtons(false);

    MessageBox msg;
    msg.setWindowTitle("VaccinDistributor");
    msg.setText("Simulation was cleared, import a new .xml file!");
    msg.setStyleSheet_();
    msg.setStandardButtons(QMessageBox::Ok);
    msg.autoClose = true;
    msg.timeout = 2;
    msg.exec();
}

void MainWindow::on_buttoNext_clicked()
{
    REQUIRE(properlyInitialized(), "MainWindow object must be properly initialized");

    if (this->runSimulation && s.checkSimulation() && s.properlyInitialized()) {

        std::pair<std::string, std::string> pairReturn = s.simulate();

        updateTextEdit(QString::fromStdString(pairReturn.second));

        std::string imageName = s.generateBmp(pairReturn.first);
        updateLabelImage(tr(imageName.c_str()));

        updateProgressBarVaccinated(s.getVaccinatedPercent());
        updateModels(s.getVaccinData());
    }
    ui->buttonPrevious->setEnabled(true);
}

void MainWindow::updateTextEdit(const QString &x) {

    REQUIRE(properlyInitialized(), "MainWindow object must be properly initialized");

    ui->textEdit->append(x);
    ui->textEdit->toPlainText();
}

void MainWindow::updateLabelImage(const QString &fileName) {

    REQUIRE(properlyInitialized(), "MainWindow object must be properly initialized");

    if (QString::compare(fileName, QString()) != 0) {
        QImage image;
        bool valid = image.load(fileName);

        if (valid) {
            if(image.width() > image.height()){
                image = image.scaledToWidth(ui->labelImage->width(), Qt::SmoothTransformation);
                image = image.scaledToWidth(ui->labelImage->height(), Qt::SmoothTransformation);
            }
            else{
                image = image.scaledToHeight(ui->labelImage->width(), Qt::SmoothTransformation);
                image = image.scaledToHeight(ui->labelImage->height(), Qt::SmoothTransformation);
            }
            ui->labelImage->setPixmap(QPixmap::fromImage(image));
        }
        else {
            const char* errMsg = "Could not show image!";

            MessageBox msg;
            msg.setWindowTitle("Error");
            msg.setText(errMsg);
            msg.setStyleSheet_();
            msg.setIcon(QMessageBox::Critical);
            msg.setStandardButtons(QMessageBox::Ok);
            msg.autoClose = true;
            msg.timeout = 2;
            msg.exec();
        }
    }
}

void MainWindow::updateProgressBarVaccinated(const int x) {

    REQUIRE(properlyInitialized(), "MainWindow object must be properly initialized");
    ui->progressBarVaccinated->setValue(x);
}

void MainWindow::on_buttonPrevious_clicked() {
    REQUIRE(properlyInitialized(), "MainWindow object must be properly initialized");

    if (!s.undoSimulation()) {

        MessageBox msg;
        msg.setWindowTitle("VaccinDistributor");
        msg.setText("Can't go back any further!");
        msg.setStyleSheet_();
        msg.setStandardButtons(QMessageBox::Ok);
        msg.autoClose = true;
        msg.timeout = 2;
        msg.exec();
    }
    std::string imageName = "Day-" + std::to_string(s.getIter() - 1) + ".bmp";
    updateLabelImage(tr(imageName.c_str()));
    updateProgressBarVaccinated(s.getVaccinatedPercent());
    updateModels(s.getVaccinData());
    if (!s.undoSimulation()) {
        ui->buttonPrevious->setEnabled(false);
    }
}

void MainWindow::updateModels(const std::map<const std::string, int> &vaccins) {

    REQUIRE(properlyInitialized(), "MainWindow object must be properly initialized");

    QStringList list;

    for (std::map<const std::string, int>::const_iterator it = vaccins.begin(); it != vaccins.end(); it++) {

        QString data = QString::fromStdString(it->first);
        data.append("\t \t");
        data.append(QString::fromStdString(ToString(it->second)));

        list.append(data);
    }
    modelVaccins->setStringList(list);
}

void MainWindow::on_action_ini_triggered()
{
    REQUIRE(properlyInitialized(), "MainWindow object must be properly initialized");
    std::system("rm *.ini");
}

void MainWindow::on_action_bmp_triggered()
{
    REQUIRE(properlyInitialized(), "MainWindow object must be properly initialized");
    std::system("rm *.bmp");
}

void MainWindow::on_buttonAutoSimulation_clicked() {

    REQUIRE(properlyInitialized(), "MainWindow object must be properly initialized");
    if(!autoSimulation){
        ui->buttonAutoSimulationPausePlay->setVisible(true);
        ui->buttonAutoSimulation->setText("Stop");
        ui->buttonAutoSimulation->setStyleSheet("border-style: solid;\nborder-top-color: transparent;\nborder-right-color: transparent;\nborder-left-color: transparent;\nborder-bottom-width: 1px;\nborder-bottom-color: rgb(212, 60, 136);\nborder-style: solid;\ncolor: #a9b7c6;\npadding: 2px;\nbackground-color: #1e1d23;");
        autoSimulation = true;
        int days = ui->Days->value();
        int delay = ui->Delay->value();
        if (this->runSimulation && s.checkSimulation() && s.properlyInitialized()) {
            for (int i = 0; i < days && autoSimulation; i++) {
                ui->currentDay->setText(("Current day: " + std::to_string(s.getIter())).c_str());
                std::pair<std::string, std::string> pairReturn = s.simulate();

                updateTextEdit(QString::fromStdString(pairReturn.second));

                std::string imageName = s.generateBmp(pairReturn.first);
                updateLabelImage(tr(imageName.c_str()));

                updateProgressBarVaccinated(s.getVaccinatedPercent());
                updateModels(s.getVaccinData());

                QTime dieTime = QTime::currentTime().addSecs(delay);
                while (QTime::currentTime() < dieTime || pauseSimulation)
                    QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
            }
        }
        else{
            MessageBox msg;
            msg.setWindowTitle("No simulation");
            msg.setText("Open an simulation file");
            msg.setStyleSheet_();
            msg.setStandardButtons(QMessageBox::Ok);
            msg.autoClose = true;
            msg.timeout = 2;
            msg.exec();
        }
    }
    ui->buttonAutoSimulation->setText("Start");
    ui->buttonAutoSimulation->setStyleSheet("border-style: solid;\nborder-top-color: transparent;\nborder-right-color: transparent;\nborder-left-color: transparent;\nborder-bottom-width: 1px;\nborder-bottom-color: #04b97f;\nborder-style: solid;\ncolor: #a9b7c6;\npadding: 2px;\nbackground-color: #1e1d23;");
    ui->buttonAutoSimulationPausePlay->setVisible(false);
    autoSimulation = false;
}

void MainWindow::on_buttonAutoSimulationPausePlay_clicked() {

    REQUIRE(properlyInitialized(), "MainWindow object must be properly initialized");
    changePauseState(pauseSimulation);
}

void MainWindow::on_actionExport_as_mp4_triggered() {
    QString pad = QFileDialog::getSaveFileName(nullptr, "Export as mp4", ".", "Movie (*.mp4)");
    std::system(("rm '" + pad.toStdString() + "'").c_str());
    std::system(("ffmpeg -framerate 1 -i Day-%1d.bmp '" + pad.toStdString() + "'").c_str());
}

void MainWindow::on_centraButton_clicked() {

    REQUIRE(properlyInitialized(), "MainWindow object must be properly initialized");

    // show errMsg when no .xml file was imported || simulation is not complete
    if (!s.checkSimulation() || !s.properlyInitialized()) {
        const char* errMsg = "Could not show centra!\nTry loading a new .xml file.";

        MessageBox msg;
        msg.setWindowTitle("Error");
        msg.setText(errMsg);
        msg.setStyleSheet_();
        msg.setIcon(QMessageBox::Critical);
        msg.setStandardButtons(QMessageBox::Ok);
        msg.autoClose = true;
        msg.timeout = 2;
        msg.exec();
        return;
    }

    // TODO - Selecteren van item om te veranden
    changePauseState(false);

    Dialog dialog;
    dialog.setModal(true);
    dialog.createModels(s.getFcentra(), s.getHub());
    dialog.exec();
}

void MainWindow::changeStateButtons(bool state) {
    ui->centraButton->setEnabled(state);
    ui->buttonStart->setEnabled(state);
    ui->buttonStop->setEnabled(state);
    ui->buttonPrevious->setEnabled(state);
    ui->buttoNext->setEnabled(state);
    ui->buttonAutoSimulationPausePlay->setEnabled(state);
    ui->buttonAutoSimulation->setEnabled(state);
}

void MainWindow::changePauseState(bool state) {
    REQUIRE(properlyInitialized(), "MainWindow object must be properly initialized");
    pauseSimulation = !state;
    if(!pauseSimulation){
        ui->buttonAutoSimulationPausePlay->setText("||");
    }
    else{
        ui->buttonAutoSimulationPausePlay->setText("▶");
    }
}

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

void MainWindow::on_buttonPrevious_clicked()
{
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
    std::string imageName = "Day-" + std::to_string(s.getIter()) + ".bmp";
    updateLabelImage(tr(imageName.c_str()));
    updateProgressBarVaccinated(s.getVaccinatedPercent());
    updateModels(s.getVaccinData());
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

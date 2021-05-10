#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    createMenus();
    createActions();
    createModels();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createMenus() {

    this->setCentralWidget(ui->centralwidget);
    menuBar()->setNativeMenuBar(false);

    // TextEdit
    ui->textEdit->setReadOnly(true);

    // progressBarVaccinated
    ui->progressBarVaccinated->setValue(0);
}

void MainWindow::createActions() {

    connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::on_actionOpen_triggered);
}

void MainWindow::createModels() {

    modelVaccins = new QStringListModel(this);


    QStringList list;
    list.append(tr("Astrazenica \t 3530"));
    list.append(tr("Pfizer \t 4000923"));


    modelVaccins->setStringList(list);

    // Items cannot be manually updated
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
//    ui->listViewVaccins->
    ui->tableView->setAcceptDrops(false);
    ui->tableView->setModel(modelVaccins);
}

void MainWindow::on_actionOpen_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QString(),
                                                    tr("XML files (*.xml);;Images (*.png *.bmp *.jpeg *.gif)"));

    if (!fileName.isEmpty()) {
        // Convert QString to const char*
       QByteArray ba = fileName.toLocal8Bit();
       s.importXmlFile(ba.data());
    }
    else {
        QMessageBox::critical(this, tr("Error"), tr("Could not open file"));
    }


}

void MainWindow::on_buttonStart_clicked()
{
    // TODO - sleep / time

    // TODO - meerdere keren achter elkaar een nieuw bestand invoeren?
    if (!s.checkSimulation() || !s.properlyInitialized()) {
        const char* errMsg = "Could not run simulation!\nTry loading a new .xml file.";
        QMessageBox::critical(this, tr("Error"), errMsg);
        return;
    }

    this->runSimulation = true;
    MessageBox msg;
    msg.setWindowTitle("VaccinDistributor");
    msg.setText("Simulation file was successfully imported!");
    msg.setStyleSheet_();
    msg.autoClose = true;
    msg.timeout = 4;
    msg.exec();
}

void MainWindow::on_buttonStop_clicked()
{

//    QFuture<void>

//    QFuture<void> future = QtConcurrent::run( s.simulate,  );  // Thread 1

//    this->runSimulation = false;
}

void MainWindow::on_buttoNext_clicked()
{
    if (this->runSimulation && s.checkSimulation() && s.properlyInitialized()) {

        std::pair<std::string, std::string> pairReturn = s.simulate();

        updateTextEdit(QString::fromStdString(pairReturn.second));
        updateLabelImage(tr("export.bmp")); // TODO
        updateProgressBarVaccinated(s.getVaccinatedPercent());
    }
}

void MainWindow::updateTextEdit(const QString &x) {

    ui->textEdit->append(x);
    ui->textEdit->toPlainText();
}

void MainWindow::updateLabelImage(const QString &fileName) {

        if (QString::compare(fileName, QString()) != 0) {
            QImage image;
            bool valid = image.load(fileName);

            if (valid) {
                image = image.scaledToWidth(ui->labelImage->width(), Qt::SmoothTransformation);
                image = image.scaledToWidth(ui->labelImage->height(), Qt::SmoothTransformation);
                ui->labelImage->setPixmap(QPixmap::fromImage(image));
            }
            else {
                QMessageBox::critical(this, tr("Error"), tr("Could not open file"));
            }
        }
}

void MainWindow::updateProgressBarVaccinated(const int x) {

    ui->progressBarVaccinated->setValue(x);
}

void MainWindow::on_buttonPrevious_clicked()
{

    if (!s.undoSimulation()) {

        MessageBox msg;
        msg.setWindowTitle("VaccinDistributor");
        msg.setText("Cannot go back any further!");
        msg.setStyleSheet_();
        msg.autoClose = true;
        msg.timeout = 2;
        msg.exec();
    }

    // TODO - updateLabelImage
    updateProgressBarVaccinated(s.getVaccinatedPercent());
}

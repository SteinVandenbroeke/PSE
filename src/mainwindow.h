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


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionOpen_triggered();

    void on_buttonStart_clicked();

    void on_buttonStop_clicked();

    void on_buttoNext_clicked();

    void on_buttonPrevious_clicked();

private:
    Ui::MainWindow *ui;
    QStringListModel *modelVaccins;
    bool runSimulation;

    // TODO
    Simulation s;

    void createMenus();
    void createActions();
    void createModels();
    void updateTextEdit(const QString &x);
    void updateLabelImage(const QString &fileName);
    void updateProgressBarVaccinated(const int x);
    void updateModels(const std::map<const std::string, int> &vaccins);
};

#endif // MAINWINDOW_H

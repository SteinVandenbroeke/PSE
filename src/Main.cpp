/**
 * @file Main.cpp
 * @brief This file is used to run the system
 * @author Pablo Deputter
 * @date 10/03/2021
 */

#include <iostream>
#include <unistd.h>
#include "Simulation.h"
#include <sstream>
#include <QApplication>
#include "mainwindow.h"

//int main() {
//    Simulation s;
//    try{

//        s.importXmlFile("simulate_spec_renewal.xml", "knownTags_spec_20.xml" ,std::cerr);
//        std::cout << "Done importing" << std::endl;

//        s.simulate();
//        s.simulate();
//        std::cout << s.simulate().second;
//        s.simulate();
//        s.simulate();
//        s.simulate();
//        s.clearSimulation(true);
//        std::cout << s.getUndoStack().size() << std::endl;

//        // TODO - bool undoSimulation()

////        std::cout << "HUBS" << std::endl;
////        for (std::vector<Hub*>::const_iterator it = s.getHub().begin(); it != s.getHub().end(); it++) {
////            (*it)->printGraphical(std::cout);

////            for (std::map<std::string, Vaccin*>::const_iterator ite = (*it)->getVaccins().begin(); ite != (*it)->getVaccins().end(); ite++) {

////                std::cout << ite->first << " " << ite->second->getVaccin() << std::endl;
////            }
////        }
////        std::cout << "CENTRA" << std::endl;
////        std::cout << s.getFcentra().size() << std::endl;
////        for (std::map<std::string, VaccinationCenter*>::const_iterator it = s.getFcentra().begin(); it != s.getFcentra().end(); it++) {

////            it->second->printGraphical(std::cout);

////            auto a = it->second->getVaccin(false);
////            auto b = it->second->getVaccin(true);
////            a.insert(b.begin(), b.end());

////            std::cout << a.size() << " " << b.size() << std::endl;

////            for (auto ite = a.begin(); ite != a.end(); ite++) {

////                std::cout << ite->first << "\t" << std::endl;
////                std::cout << "\t" << ite->second->getTracker().size() << std::endl;
////            }
////        }
//        std::cout<< s.simulate().second;
//    }
//    catch (Exception ex) {
//        std::cerr << ex.value() << std::endl;
//    }
//    return 0;
//}



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow m;
    m.showNormal();

    return a.exec();
}

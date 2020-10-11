#include "CTCOffice/ctcoffice/ctc_main.h"
#include "HWTrackController/HWTrackController_main.h"
#include "SWTrackController/TrackController.h"
#include "TrackModel/tracklayout.hpp"
#include "TrackModel/trackmodel_main.hpp"
#include "HWTrainController/HWTrainController_main.h"
#include "SWTrainController/SWTrainController.h"

#include <QApplication>
#include <QDebug>

TrackModel::RouteFile blueLine {"Blue Line", "blue_line.csv"};

int mainArgc;
char **mainArgv;

void createNewTrain() {
    SWTrainController(mainArgc, mainArgv);
}

int main(int argc, char *argv[])
{
    mainArgc = argc;
    mainArgv = argv;

    QApplication a(argc, argv);

    TrackModel::routesToLoad.push_back(blueLine);
    int initResult = TrackModel::initializeTrackModel();
    if( initResult < 0 )
    {
        qDebug() << "Failed to load track model";
        a.quit();
        return EXIT_FAILURE;
    }

    init_CTC(argc, argv);
    //init_HWTrackController();
    //init_SWTrackController();
    //init_HWTrainController();

    return a.exec();
}

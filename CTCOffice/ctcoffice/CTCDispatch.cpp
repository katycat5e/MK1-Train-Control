#include "CTCDispatch.h"
#include <QDebug>
#include <string>
#include <sstream>
#include <QTextStream>
#include <QApplication>
#include "../../TrackModel/tracklayout.hpp"
#include "system_main.h"
#include <cmath>


//useful cout replacement command while using QT Creator Application Output
//qDebug() << QString::fromStdString(station);
//qDebug() << QString::number(hours);

//Get block 6
/*Route *rt;
rt = getRoute("Blue Line");
Block* blk6 = rt -> getBlock(6);

while(prevBlock != NULL)
Block* blk5 = blk6 -> prevBlock;*/

void CTCDispatch::setAuthority()
{
        TrackModel::TrainPathInfo route;
        route = findRoute();

        int auth = route.blocks.size();
        for(auto val : route.blocks){
            authority.push_back(std::make_pair(val->id, auth));
            auth--;
            qDebug() << "Set Authority at Block " << val->id << ": " << QString::number(auth);
        }
}

void CTCDispatch::setSpeed(float timeStart, float timeArrival)
{
    TrackModel::TrainPathInfo route;
    route = findRoute();
    float totalBlockLength = 0;

    for(auto val : route.blocks){
        totalBlockLength += val->length;
    }

    totalBlockLength = totalBlockLength/1000;
    speed = totalBlockLength/(timeArrival-timeStart);
    qDebug() << "Set Speed to: " << QString::number(speed);
}

void CTCDispatch::setLine(std::string l){
    line = l;
    qDebug() << "Set Line to: " << QString::fromStdString(line);
}

std::string CTCDispatch::getline(){
    return line;
}

void CTCDispatch::setDestination(QString dest, QString destType){
    if(destType == "Station"){
        setStation(dest.toStdString());
    }
    if(destType == "Block"){
        setBlock(dest.toInt());
    }
}

void CTCDispatch::setBlock(int blk){
    if(blk != -1){
        qDebug() << "Set Block Destination to: " << QString::number(blk);
        endblock = blk;
    }
    else{
        endblock = -1;
    }
}

int CTCDispatch::getBlock(){
    return endblock;
}

void CTCDispatch::setStation(std::string state){
    int startnum, endnum;
    std::string blknum;

    startnum = state.find('(');
    endnum = state.find(')');

    blknum = state.substr(startnum + 1, (endnum - startnum) -1);

    std::stringstream temp1(blknum);
    temp1 >> endblock;
    qDebug() << "Block: " << QString::number(endblock);
}

std::string CTCDispatch::getStation(){
    return station;
}

void CTCDispatch::setTimeStart(QTime st){

    qt = st;

    QDateTime t = systemClock->currentTime();
    QTime rt = t.time();

    float time = st.hour() + (((float)st.minute())/60);
    qDebug() << "Start Time Math: " << QString::number(time);

    timeStart = time;
}

float CTCDispatch::getTimeStart(){
    return timeStart;
}

void CTCDispatch::setTimeArrival(QTime at){

    float time = at.hour() + (((float)at.minute())/60);
    qDebug() << "Arrival Time Math: " << QString::number(time);

    timeArrival = time;
}

float CTCDispatch::getTimeArrival(){
    return timeArrival;
}

void CTCDispatch::setPassNum(){
    passNum = rand() % 220;
}

float CTCDispatch::getPassNum(){
    return passNum;
}

void CTCDispatch::dispatch(CTCSignals& c){
    setPassNum();
    carsNum = ceil(passNum/44); // ceil is taken out
    //qDebug() << "Number of Passengers on Train: " << QString::number(passNum);
    //qDebug() << "Number of Cars on Train: " << QString::number(carsNum);

    setAuthority();
    setSpeed(timeStart, timeArrival);

    if(authority.size() == 0){
        qDebug() << "Route not found.";
        return;
    }
        sendTrackController();

        createNewTrain(m, carsNum, line);

}

void CTCDispatch::sendTrackController(){

    TrackModel::TrainPathInfo route;
    route = findRoute();

    int i = 0;
    for(std::pair<int, TrackModel::SwitchState> val : route.switchStates){
          if(i == 6){
              break;
          }
          tcStates.push_back(val.second);
          i++;
    }

    std::vector<std::pair<int, TrackModel::SwitchState> > temp_r;

    int w = 0;
    for(std::pair<int, TrackModel::SwitchState> val : route.switchStates){
          if(w == 6){
              break;
          }

          temp_r.push_back(std::make_pair(val.first, val.second));
          w++;
    }

        wayside_sig.setSpeed(line, speed);
        wayside_sig.setAuthority(line, authority);

        wayside_sig.setExitBlocks(temp_r);


      alertWaysideSystem(line, wayside_sig);

      // Choose the correct HW Track Controller to instantiate
      if (existence == 1) {
          if (line == "Green Line") {
              greenreg.initialize(endblock, speed, authority);
          } else if (line == "Red Line") {
              redreg.initialize(endblock, speed, authority);
          }
      }

    //initializeHW(ctc);

    // Choose the correct HW Track Controller to instantiate
    if (line == "Green Line") {
        greenreg.initialize(endblock, speed, authority);
    } else if (line == "Red Line") {
        redreg.initialize(endblock, speed, authority);
    }

}

CTCMode* CTCDispatch::getCTCMode()
{
    return m;
}

void CTCDispatch::setCTCMode(CTCMode* mode)
{
    m = mode;
}

TrackModel::TrainPathInfo CTCDispatch::findRoute(){
    TrackModel::Route *rte;
    rte = TrackModel::getRoute(line);

    TrackModel::TrainPathInfo path;
    TrackModel::TrackRouter temp(rte);
    path = temp.findPath(0, TrackModel::BLK_NODIR , endblock);

    return path;
}

int CTCDispatch::setTimeDelay(){
    int test = systemClock->currentTime().time().QTime::msecsTo(qt);
    qDebug() << "The time satrt: " << qt.hour()<<":"<<qt.minute();

    if(test < 0){
        test = 86400000 + test;
    }

    qDebug() << "The time needed is: " << test;

    return test;
}


/*for(std::pair<int, TrackModel::SwitchState> val : route.switchStates){
 *      newvector.push_back(val.second);
 * }
 */

//Take the first 6 switch state values and put them in a vector
//Make sure that they are 0 and 1
//call calcRoute(line, switchVector) of CTCSignals class


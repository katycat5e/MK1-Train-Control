#include "trackcntrl_main.h"

QMainWindow *programmerGUI;
bool dispatched = false;

WaysideContainer *blue = new WaysideContainer(1);
WaysideContainer *green = new WaysideContainer(12);
WaysideContainer *red = new WaysideContainer(10);
TrackController temp; //= new WaysideContainer;

CTCSignals ctc_blue, ctc_green, ctc_red;

void updateCTCSignals(std::string&);

int init_SWTrackController() {

    //dispatched = false;

    programmerGUI = new SWTrackCntrlWindow();
    programmerGUI -> show();



	return 0;
}

void setPLCFile(std::string &temp) {
    blue -> waysides.begin() -> filename = temp;

    for (auto i = green -> waysides.begin(); i != green -> waysides.end(); i++) {
        i -> filename = temp;
    }

   for (auto i = red -> waysides.begin(); i != red -> waysides.end(); i++) {
        i -> filename = temp;
    }
}

void alertWaysideSystem(std::string &dis_line, CTCSignals &temp) {

    if (dis_line == "Blue Line") {

        ctc_blue = temp;

    }
    if (dis_line == "Green Line") {

        ctc_green = temp;

    }
    if (dis_line == "Red Line") {

        ctc_red = temp;

    }

    updateCTCSignals(dis_line);
}

void updateCTCSignals(std::string &l) {

    if (l == "Blue Line") {
        blue -> addCTCObj(ctc_blue);
    }
    if (l == "Green Line") {
        green -> addCTCObj(ctc_green);
    }
    if (l == "Red Line") {
        red -> addCTCObj(ctc_red);
    }
    dispatched = true;

}

TrackController getWaysideInstance(int num) {
    if (num == 1) {
        auto i = blue -> waysides.begin();
        temp = *i;
    }
    else {
        if (num < 14) {
       int count = 2;
       for (auto i = green -> waysides.begin(); i != green -> waysides.end(); i++) {
            if (count == num) {
                temp = *i;
                count++;
             }
             else {
                 count++;
             }
            }
        }
      else {
           int count = 14;
            for (auto i = red -> waysides.begin(); i != red -> waysides.end(); i++) {
                if (count == num) {
                    temp = *i;
                    count++;
                }
                else {
                    count++;
                }
            }
        }

    }
    return temp;
    }



void setSwitchUI(TrackController &temp, bool new_s) {
    temp.setSwitch(new_s);
}

void setCrossingUI(TrackController &temp, bool new_s) {
    temp.manSetCross(new_s);
}

void updateWaysides() {

    //blue -> waysides.begin() -> updateData();

    for (auto i = green -> waysides.begin(); i != green -> waysides.end(); i++) {
        i -> updateData();
    }

   /* for (auto i = red -> waysides.begin(); i != red -> waysides.end(); i++) {
        i -> updateData();
    }*/
}

bool checkDispatched() {
    return dispatched;
}

bool getControllerMode(std::string &line) {

    bool tempmode;

    if (line == "Green Line") {
        tempmode = ctc_green.getMode(line);
    }

    if (line == "Red Line") {
        tempmode = ctc_red.getMode(line);
    }

    return tempmode;
}

//  Implementation file for SW Train Controller GUI

#include "TrainCGUI.h"


#include <QString>
#include <QtLocation/QGeoRoute>
#include <iostream>

TrainControlWindow::TrainControlWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::TrainControlWindow)
{
    ui->setupUi(this);

    timerID = startTimer(1000); // timer event occurs every second
    count = 0;

    ui->releasebrakebutton->hide();
}

TrainControlWindow::~TrainControlWindow()
{
    killTimer(timerID);
    delete ui;
}

// --------------------------------------------------------------------------------- Transmitter
void TrainControlWindow::timerEvent(QTimerEvent *event)
{
    count++;
    //std::cout << "Timer has updated... " << count << std::endl; //debug

    swtc.calculatePower();
    updatePower();
    updateCircuitInfo();
    updateBrakes();
    updateSpeed();
    updateCabin();
}

// -------------------------------------------------------------------------------- Transmitter update functions
void TrainControlWindow :: updateCircuitInfo()
{
    swtc.decode(train->sendTrackCircuit());
    ui->commspeed_->setText(QString::number(swtc.getCommandedSpeed()));
    ui->authority_->setText(QString::number(swtc.getAuthority()));
}

void TrainControlWindow :: updatePower()
{
    swtc.setTrainVelocity(train->getCurrentVelocity());
    ui->currspeed_->setText(QString::number(swtc.getTrainVelocity()));

    train->setPower(swtc.getPowerCommand());
    ui->powerOutput_->setText(QString::number(swtc.getPowerCommand()));
}

void TrainControlWindow :: updateBrakes()
{
    // if the train is actively braking, display on GUI for driver
    if (swtc.getEmergencyBrakeFlag() == true){
        ui->releasebrakebutton->show();
        ui->ebrake_->setText("The emergency brake is enabled!");
    } else {
        ui->ebrake_->setText("");
    }

    if (swtc.getServiceBrakeFlag() == true){
        ui->releasebrakebutton->show();
        ui->sbrake_->setText("The service brake is enabled!");
    } else {
        ui->sbrake_->setText("");
    }

    /*// if the train has 0 velocity, reset the brake flags
    if ((train->getCurrentVelocity()) == 0.0) {
        swtc.setServiceBrake(false);
        swtc.setEmergencyBrake(false);
    }*/
}

void TrainControlWindow :: updateSpeed()
{
    ui->setpointSpeed_->setText(QString::number(swtc.getSetpointSpeed()));
}

void TrainControlWindow :: updateCabin()
{
    // Update current status of the cabin
    QString doors, lights, headlights;

    if (train->getDoorStatus())
        doors = "OPEN";
    else
        doors = "CLOSED";

    //doors = (train->getDoorStatus()) ? "OPEN" : "CLOSED";

    if (train->getCabinLights())
        lights = "ON";
    else
        lights = "OFF";

    if (train->getHeadlights())
        headlights = "ON";
    else
        headlights = "OFF";

    ui->doors_->setText(doors);
    ui->lights_->setText(lights);
    ui->headlights_->setText(headlights);


    // Send updates to the train, if needed
    bool temporary = swtc.getDoorsOpen();
    train->setDoorStatus(temporary);
    train->setCabinLights(swtc.getCabinLightsOn());
    train->setHeadlights(swtc.getHeadlightsOn());

}

// ------------------------------------------------------------------------------------------- GUI buttons n' stuff
void TrainControlWindow::on_submit_clicked() //Submits Kp and Ki
{
    //convert input string to text then assign to Kp  & Ki
    QString temp1,temp2;
    temp1 = ui->kp_textbox->toPlainText();
    kp = temp1.toDouble();
    temp2 =  ui->ki_textbox->toPlainText();
    ki = temp2.toDouble();

    //set Kp and Ki in the SWTC class for math stuffs later
    swtc.setKp(kp);
    swtc.setKi(ki);

    // Disable textboxes and submit button
    ui->ki_textbox->setReadOnly(true);
    ui->kp_textbox->setReadOnly(true);
    ui->submit->setDisabled(true);

    cout << "Kp and Ki have been set." << std::endl; //debug

    //Calculate the initial power for the yard speed (5 m/s)
    swtc.setSetpointSpeed(yardSpeed);
    swtc.calculatePower();

    cout << "The initial power for 5m/s has been set by the train controller.\n";

}

void TrainControlWindow::on_sendPowerButton_clicked()
{
    std::cout << "Setting power command to " << temp << std::endl;

    //power = swtc.calculatePower();
    swtc.setPowerCommand(temp);

    std::cout << "Power command set.\n";

    temp++;
}

void TrainControlWindow::on_serviceBrake_clicked()
{
    std::cout << "Service brake has been applied.\n"; //debug

    swtc.setPowerCommand(0.0); // set power command to zero
    swtc.setServiceBrake(true);
}

void TrainControlWindow::on_emergencyBrake_clicked()
{
    std::cout << "Emergency brake has been applied.\n"; //debug

    swtc.setPowerCommand(0.0); // set power command to zero
    swtc.setEmergencyBrake(true);
}

void TrainControlWindow::on_inc_setspeed_clicked()
{
    double current = swtc.getSetpointSpeed();
    swtc.setSetpointSpeed(current + 1.0);
}

void TrainControlWindow::on_dec_setspeed_clicked()
{
    double current = swtc.getSetpointSpeed();
    swtc.setSetpointSpeed(current - 1.0);
}

void TrainControlWindow::on_door_button_clicked()
{
    swtc.setDoorsOpen(!swtc.getDoorsOpen());
}

void TrainControlWindow::on_lights_button_clicked()
{
    swtc.setCabinLightsOn(!swtc.getCabinLightsOn());
}

void TrainControlWindow::on_headlights_button_clicked()
{
    swtc.setHeadlightsOn(!swtc.getHeadlightsOn());
}

void TrainControlWindow::on_releasebrakebutton_clicked()
{
    swtc.setServiceBrake(false);
    swtc.setEmergencyBrake(false);
    ui->releasebrakebutton->hide();
}

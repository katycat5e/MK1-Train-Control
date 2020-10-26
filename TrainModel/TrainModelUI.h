#ifndef TRAINMODELUI_H
#define TRAINMODELUI_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class TrainModelUI; }
QT_END_NAMESPACE

class TrainModelUI : public QMainWindow
{
    Q_OBJECT

public:
    TrainModelUI(QWidget *parent = nullptr);
    ~TrainModelUI();
    void updatePower(double power);
    void updateVelocity(double currVel);
    void updateBlockNum(int block);
    void updateBlockLength(double length);
    void updateBlockGrade(double grade);
    void updateNumCars(int cars);
    void updateDoors(bool);
    void updateCabinLights(bool);
    void updateTemp(int);
    void updateHeater(bool);
    void updateAC(bool);
    void updateAds(bool);
    void updateAnnouncements(bool, std::string);

private:
    Ui::TrainModelUI *ui;
};
#endif // TRAINMODELUI_H

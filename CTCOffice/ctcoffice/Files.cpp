#include "Files.h"
#include "ui_files.h"

#include <fstream>
#include <iostream>

#include <QFileInfo>

Files::Files(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Files)
{
    ui->setupUi(this);
}

Files::~Files()
{
    delete ui;
}

void Files::mapDisplay(){

    QPixmap mapfile("map.png");
    ui->lblMap->setPixmap(mapfile);
    ui->lblMap->setScaledContents(true);
}

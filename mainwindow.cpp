#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_loadVideo_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Image"), "/home", tr("Video Files (*.mp4 *.avi)"));
    ui->communicateBox->clear();
    int roadPosition = 0;
    if(ui->horizontalRadioButton->isChecked())
        roadPosition = 1;
    try{
        ui->communicateBox->appendPlainText("Przetwarzanie...");
        CarDetection detector( fileName.toStdString(), roadPosition);
        detector.detectCars();
        ui->communicateBox->appendPlainText("Gotowe");
    }
    catch( Exception e)
    {
        ui->communicateBox->appendPlainText("Błąd podczas przetwarzania pliku");
    }
}


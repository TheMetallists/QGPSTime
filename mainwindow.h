#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPortInfo>
#include <QSerialPort>
#include "tinygpspp.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
Q_OBJECT

public:
	MainWindow(QWidget *parent = nullptr);

	~MainWindow();

private slots:

	void on_btnConnect_clicked();

	void on_btnReload_clicked();

	void comm_onDataAvaiable();

private:
	Ui::MainWindow *ui;
	QSerialPort port;
	TinyGPSPlus gps;
};

#endif // MAINWINDOW_H

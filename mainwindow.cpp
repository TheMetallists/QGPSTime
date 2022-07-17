#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QRegExp>
#include <QDebug>
#include <QDateTime>

#ifdef Q_OS_WIN32
#include <windows.h>
#endif


MainWindow::MainWindow(QWidget *parent)
		: QMainWindow(parent), ui(new Ui::MainWindow) {
	ui->setupUi(this);
	Q_FOREACH(QSerialPortInfo
	port,
			QSerialPortInfo::availablePorts()) {
		this->ui->cbPort->addItem(port.portName());
	}

	//1200, 2400, 4800, 9600, 14400, 19200, 38400, 57600, 115200
	ui->cbUartSpeed->addItem("115200");
	ui->cbUartSpeed->addItem("57600");
	ui->cbUartSpeed->addItem("38400");
	ui->cbUartSpeed->addItem("19200");
	ui->cbUartSpeed->addItem("14400");
	ui->cbUartSpeed->addItem("9600");
	ui->cbUartSpeed->addItem("4800");
	ui->cbUartSpeed->addItem("2400");
	ui->cbUartSpeed->addItem("1200");
}

MainWindow::~MainWindow() {
	delete ui;
}


void MainWindow::on_btnConnect_clicked() {
	this->port.setPortName(ui->cbPort->currentText());
	this->port.setBaudRate(ui->cbUartSpeed->currentText().toInt());
	this->port.setDataBits(QSerialPort::Data8);
	this->port.setFlowControl(QSerialPort::NoFlowControl);
	this->port.setStopBits(QSerialPort::OneStop);
	bool open = this->port.open(QIODevice::ReadWrite);
	if (!open) {
		QMessageBox messageBox;
		messageBox.critical(this, "Fatality", "Failed to open comport!");
		return;
	}

	connect(&this->port, SIGNAL(readyRead()), this, SLOT(comm_onDataAvaiable()));
	//this->ui->cbPort->addItem("asd");
}

void MainWindow::on_btnReload_clicked() {
	this->ui->cbPort->clear();
	Q_FOREACH(QSerialPortInfo port, QSerialPortInfo::availablePorts()) {
			this->ui->cbPort->addItem(port.portName());
		}
}

void MainWindow::comm_onDataAvaiable() {
	if (this->ui->cbUseNMEA->isChecked()) {
		QByteArray packet = this->port.readAll();
		// feed the current data into the arduino library
		for (int i = 0; i < packet.length(); i++) {
			this->gps.encode(packet.at(i));
		}

		if (this->gps.time.isValid() && this->gps.time.isUpdated()) {
			this->ui->lblTime->setText(QString("%1-%2-%3 %4:%5:%6").arg(
					this->gps.date.day()
			).arg(
					this->gps.date.month()
			).arg(
					this->gps.date.year()
			).arg(
					this->gps.time.hour()
			).arg(
					this->gps.time.minute()
			).arg(
					this->gps.time.second()
			));

			QDateTime current = QDateTime::currentDateTime();
			if (current.time().minute() != this->gps.time.minute() ||
			    current.time().second() != this->gps.time.second()) {
				// updating system time
#ifdef Q_OS_WIN32
				SYSTEMTIME st;
				ZeroMemory(&st, sizeof(st));
				st.wDay = this->gps.date.day();
				st.wMonth = this->gps.date.month();
				st.wYear = this->gps.date.year();
				st.wHour = this->gps.time.hour();
				st.wMinute = this->gps.time.minute();
				st.wSecond = this->gps.time.second();
				bool result = SetLocalTime(&st);
				if(!result){
					ui->lblTime->setText(ui->lblTime->text() + QString("\r\n<ERROR SETTING: CHECK ADMIN RIGHTS>"));
				}
				qDebug() << "SET RES:" << result;
#endif
			}
		}

	} else {
		QRegExp rex(R"(===RRR DATEPACK: (\d+)-(\d+)-(\d+) (\d+)_(\d+)_(\d+) RRR===)");
		while (this->port.canReadLine()) {
			QString line = QString(this->port.readLine());
			qDebug() << "LINE: " << line;
			if (rex.indexIn(line) < 0) {
				qDebug() << "LESS: " << rex.indexIn(line);
				continue;
			}

			int dd, dm, dy, hh, hi, hs;
			bool ok;
			dd = rex.cap(1).toInt(&ok);
			if (!ok)
				continue;
			dm = rex.cap(2).toInt(&ok);
			if (!ok)
				continue;
			dy = rex.cap(3).toInt(&ok);
			if (!ok)
				continue;
			hh = rex.cap(4).toInt(&ok);
			if (!ok)
				continue;
			hi = rex.cap(5).toInt(&ok);
			if (!ok)
				continue;
			hs = rex.cap(6).toInt(&ok);
			if (!ok)
				continue;

			this->ui->lblTime->setText(QString("%1-%2-%3 %4:%5:%6").arg(dd).arg(dm).arg(dy).arg(hh).arg(hi).arg(hs));
			QDateTime current = QDateTime::currentDateTime();
			if (current.time().minute() != hi || current.time().second() != hs) {
				// updating system time
#ifdef Q_OS_WIN32
				SYSTEMTIME st;
				ZeroMemory(&st, sizeof(st));
				st.wDay = dd;
				st.wMonth = dm;
				st.wYear = dy;
				st.wHour = hh;
				st.wMinute = hi;
				st.wSecond = hs;
				bool result = SetLocalTime(&st);
				if(!result){
					ui->lblTime->setText(ui->lblTime->text() + QString("\r\n<ERROR SETTING: CHECK ADMIN RIGHTS>"));
				}
				qDebug() << "SET RES:" << result;
#endif
			}

		}
	}

}

uint32_t millis() {
	qint64 qiTimestamp = QDateTime::currentMSecsSinceEpoch();
	return qiTimestamp % (uint32_t) ULONG_MAX;
}
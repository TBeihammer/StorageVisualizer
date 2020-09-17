#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_StorageVisualizerMainWindow.h"

#include <QStorageInfo>
#include <QSettings>

class StorageVisualizerMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    StorageVisualizerMainWindow(QWidget *parent = Q_NULLPTR);
	~StorageVisualizerMainWindow();

private slots:
	void onAvailableGeometryChanged(QRect);

private:
    Ui::StorageVisualizerMainWindowClass ui_;
	void timerEvent(QTimerEvent *e);
	void showEvent(QShowEvent *e);
	void loadStyle();
	
	QSettings* settings_ = new QSettings("config.ini", QSettings::Format::IniFormat);
	QString rootPath_;
	QString style_;

	double storageLimitInPercentUntilWarning_ = -1;
	double widgetWidthInPercentOfDesktop_ = -1;
	double widgetHeightInPercentOfDesktop_ = -1;
};

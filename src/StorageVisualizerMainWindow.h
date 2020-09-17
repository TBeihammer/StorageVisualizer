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
	
	QSettings* settings_ = new QSettings("config.ini", QSettings::Format::IniFormat);
	QStorageInfo storageInfo_;
	QString rootPath_;

	double widgetWidthInPercentOfDesktop_ = -1;
	double widgetHeightInPercentOfDesktop_ = -1;
};

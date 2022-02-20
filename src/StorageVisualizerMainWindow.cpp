#include "StorageVisualizerMainWindow.h"

#include <QDesktopWidget>
#include <QScreen>
#include <minmax.h>
#include <QWindowStateChangeEvent>

StorageVisualizerMainWindow::StorageVisualizerMainWindow(QWidget *parent)
    : QMainWindow(parent)
{
	ui_.setupUi(this);

	if (settings_->childGroups().isEmpty())
	{
		// Set some example keys since the file doesnt exist yet
		settings_->beginGroup("General");
		settings_->setValue("StorageRootPath", "C:");
		settings_->setValue("StorageLimitInPercentUntilWarning", 80);
		settings_->endGroup();

		settings_->beginGroup("Layout");
		settings_->setValue("WidgetWidthInPercentOfDesktop", 15);
		settings_->setValue("WidgetHeightInPercentOfDesktop", 1);
		settings_->endGroup();

		settings_->sync();
	}

	setWindowFlag(Qt::WindowStaysOnTopHint);
	setWindowFlag(Qt::FramelessWindowHint);
	setWindowFlag(Qt::Tool);
	setAttribute(Qt::WA_Hover, true);
	
	rootPath_ = settings_->value("General/StorageRootPath", "C:").toString();
	storageLimitInPercentUntilWarning_ = settings_->value("General/StorageLimitInPercentUntilWarning", 80).toDouble() / 100;
	widgetWidthInPercentOfDesktop_ = settings_->value("Layout/WidgetWidthInPercentOfDesktop", 5).toDouble() / 100;
	widgetHeightInPercentOfDesktop_ = settings_->value("Layout/WidgetHeightInPercentOfDesktop", 10).toDouble() / 100;

	loadStyle();
	startTimer(1000);
	connect(screen(), &QScreen::availableGeometryChanged, this, &StorageVisualizerMainWindow::onAvailableGeometryChanged);
}

StorageVisualizerMainWindow::~StorageVisualizerMainWindow()
{
	settings_->sync();
	delete settings_;
}

void StorageVisualizerMainWindow::onAvailableGeometryChanged(QRect)
{
	timerEvent(nullptr);
}


void StorageVisualizerMainWindow::enterEvent(QEvent * event)
{
	setWindowOpacity(1);
	QWidget::enterEvent(event);
}

void StorageVisualizerMainWindow::leaveEvent(QEvent * event)
{
	setWindowOpacity(0.75);
	QWidget::leaveEvent(event);
}

void StorageVisualizerMainWindow::timerEvent(QTimerEvent *e)
{

#ifdef _DEBUG // Load the stylesheet constantly for Debug - purposes
	loadStyle();
#endif // _DEBUG

	QRect fullDesktopRect = QApplication::desktop()->screenGeometry();
	QRect availableRect = QApplication::desktop()->availableGeometry(this);

	QStorageInfo storageInfo(rootPath_);
	qint64 spaceFree = storageInfo.bytesFree() / 1024 / 1024; //MB;
	qint64 spaceTotal = storageInfo.bytesTotal() / 1024 / 1024; //MB

	if (spaceTotal <= 0 || spaceFree < 0 || spaceFree > spaceTotal)
		return;

	double filledUp = ((double)(spaceTotal - spaceFree) / (double)spaceTotal);
	bool isWarning = filledUp >= storageLimitInPercentUntilWarning_;
	ui_.progressBar->setProperty("warning", isWarning);
	this->setStyleSheet(style_);

	ui_.progressBar->setMinimum(0);
	ui_.progressBar->setMaximum(spaceTotal);
	ui_.progressBar->setValue(spaceTotal - spaceFree);
	ui_.lbText->setText(rootPath_ + QString(" (%1 MB / %2 MB) (%3%)").arg(spaceTotal - spaceFree).arg(spaceTotal).arg(QString::number(filledUp * 100, 'f', 2)));

	resize(fullDesktopRect.width() * widgetWidthInPercentOfDesktop_, fullDesktopRect.height() * widgetHeightInPercentOfDesktop_);
	
	QSize widgetSize = size();
	move(availableRect.width() - widgetSize.width(), availableRect.height() - widgetSize.height());	
}

void StorageVisualizerMainWindow::showEvent(QShowEvent *e)
{
	timerEvent(nullptr);
	return QMainWindow::showEvent(e);
}

void StorageVisualizerMainWindow::loadStyle()
{
	QFile f("./resources/style.qss");

	if (f.open(QIODevice::ReadOnly))
	{
		style_ = QString(f.readAll());
		f.close();
	}
}



#include "StorageVisualizerMainWindow.h"

#include <QDesktopWidget>
#include <QScreen>
#include <minmax.h>

StorageVisualizerMainWindow::StorageVisualizerMainWindow(QWidget *parent)
    : QMainWindow(parent)
{
	ui_.setupUi(this);

	if (settings_->childGroups().isEmpty())
	{
		// Set some example keys since the file doesnt exist yet
		settings_->beginGroup("General");
		settings_->setValue("StorageRootPath", "C:");
		settings_->endGroup();

		settings_->beginGroup("Layout");
		settings_->setValue("AlwaysOnTop", true);
		settings_->setValue("WidgetWidthInPercentOfDesktop", 5);
		settings_->setValue("WidgetHeightInPercentOfDesktop", 10);
		settings_->endGroup();

		settings_->sync();
	}

	if (settings_->value("Layout/AlwaysOnTop", true).toBool())
		setWindowFlag(Qt::WindowStaysOnTopHint);
	setWindowFlag(Qt::FramelessWindowHint);

	rootPath_ = settings_->value("General/StorageRootPath", "C:").toString();
	storageInfo_.setPath(rootPath_);

	widgetWidthInPercentOfDesktop_ = settings_->value("Layout/WidgetWidthInPercentOfDesktop", 5).toDouble() / 100;
	widgetHeightInPercentOfDesktop_ = settings_->value("Layout/WidgetHeightInPercentOfDesktop", 10).toDouble() / 100;

	startTimer(5000);

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

void StorageVisualizerMainWindow::timerEvent(QTimerEvent *e)
{
#ifdef _DEBUG
	QFile f("./resources/style.qss");
	
	if (f.open(QIODevice::ReadOnly))
	{
		QString style(f.readAll());
		setStyleSheet(style);
		f.close();
	}

#endif // _DEBUG
	QRect fullDesktopRect = QApplication::desktop()->screenGeometry();
	QRect availableRect = QApplication::desktop()->availableGeometry(this);

	qint64 available = storageInfo_.bytesAvailable() / 1000 / 1000; //MB;
	qint64 total = storageInfo_.bytesTotal() / 1000 / 1000; //MB

	if (total <= 0 || available <= 0)
		return;

	ui_.progressBar->setMinimum(0);
	ui_.progressBar->setMaximum(total);
	ui_.progressBar->setValue(total - available);
	ui_.lbText->setText(rootPath_ + QString(" (%1 MB / %2 MB)").arg(available).arg(total));

	resize(fullDesktopRect.width() * widgetWidthInPercentOfDesktop_, fullDesktopRect.height() * widgetHeightInPercentOfDesktop_);
	
	QSize widgetSize = size();
	move(availableRect.width() - widgetSize.width(), availableRect.height() - widgetSize.height());	
}

void StorageVisualizerMainWindow::showEvent(QShowEvent *e)
{
	timerEvent(nullptr);
	return QMainWindow::showEvent(e);
}

#pragma once

#pragma once

#include <QWindow>
#include <QBackingStore>

#include <QEvent>

#include <QPainter>
#include <QImage>
#include <QCoreApplication>
#include <QDir>
#include <QDesktopWidget>

#include <Windows.h>

static HWND g_workerw = 0;
static BOOL CALLBACK EnumWinid_cb(HWND tophandle, LPARAM toppearmHandle) {
	HWND p = FindWindowEx(tophandle, 0, L"SHELLDLL_Defview", 0);
	if (p != NULL) {
		g_workerw = FindWindowEx(0,
			tophandle,
			L"WorkerW",
			0);
	}
	return true;
}

class Desktop : public QWindow
{
	Q_OBJECT

public:
	Desktop(QWindow *parent = 0)
		: QWindow(parent)
		, store(this)
	{
		HWND hwndProgram = FindWindow(L"Progman", L"Program Manager");
		SendMessageTimeout(hwndProgram, 0x052c, 0, 0, SMTO_NORMAL, 1000, 0);
		EnumWindows(EnumWinid_cb, 0);

		if (g_workerw == 0) {
			abort();
		}

		QWindow * windowDesktop = QWindow::fromWinId((WId)(g_workerw));
		this->setParent(windowDesktop);
		QDesktopWidget desktopWidget;
		QRect rectDesptop = desktopWidget.availableGeometry();
		this->setGeometry(rectDesptop);
	}
	void setBackground(QString path, QString name) {
		imagePathname = path + '/' + name;
	};
protected:
	bool event(QEvent *e)
	{
		if (e->type() == QEvent::Expose
			|| e->type() == QEvent::Resize)
		{
			store.resize(this->size());
			QRect rect(QPoint(0, 0), this->size());

			QPainter painter(store.paintDevice());
			painter.fillRect(rect, Qt::white);
			painter.drawImage(0, 0, QImage(imagePathname));
			painter.drawText(50, 50, QStringLiteral("你好啊"));


			store.flush(rect);
		}
		return QWindow::event(e);
	}
private:
	QBackingStore store;
	QString imagePathname;
};

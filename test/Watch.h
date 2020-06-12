#pragma once

#include <QObject>
#include <QFileSystemWatcher>
#include <QCoreApplication>
#include <QDebug>

class Watch : public QObject
{
	Q_OBJECT

public:
    Watch(QObject *parent=Q_NULLPTR)
        :QObject(parent)
	{

		m_watch = new QFileSystemWatcher(this);
		qDebug() << QCoreApplication::applicationDirPath();
		qDebug() << m_watch->addPath(QCoreApplication::applicationDirPath() + "/2/");
		qDebug() << m_watch->addPath(QCoreApplication::applicationDirPath() + "/2/" + QStringLiteral("新建文本文档.txt"));
		qDebug() << m_watch->addPath(QCoreApplication::applicationDirPath() + "/2/" + QStringLiteral("1.txt"));

		connect(m_watch, &QFileSystemWatcher::fileChanged,
			[=](QString pathname)
		{
			qDebug() << pathname;
		});
		connect(m_watch, &QFileSystemWatcher::directoryChanged,
			[=](QString pathname)
		{
			qDebug() << pathname;
		});
		qDebug() << m_watch->directories();
	}
private:
	QFileSystemWatcher * m_watch;
};

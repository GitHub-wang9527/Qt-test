#pragma once

#include <QPushButton>
#include <QTimer>
#include <QDebug>
#include <QVariant>
#include <QStyle>

/*
Q_PROPERTY(type name
   READ getFunction
   [WRITE setFunction]
   [RESET resetFunction]
   [NOTIFY notifySignal]
   [DESIGNABLE bool]
   [SCRIPTABLE bool]
   [STORED bool]
   [USER bool]
   [CONSTANT]
   [FINAL])
*/

class PropertyButton:public QPushButton
{
	Q_OBJECT

	Q_PROPERTY(bool mute READ isMute WRITE setMute)

public:
	PropertyButton(QWidget *parent=0)
		:QPushButton(parent)
	{
		setProperty("mute", QVariant(false));//setMute(false);
		setText(QStringLiteral("你好"));
		setStyleSheet(
			"QPushButton{color:black;}"
			"QPushButton[mute=true]{color:red;}"

			"QPushButton:hover{color:yellow;}"
		);
		this->style()->polish(this);
		QTimer*  pT = new QTimer(this);
		pT->start(1000);
		connect(pT, &QTimer::timeout,
			[=]
		{
			setMute(!isMute());
		});	//5秒后，触发loop.quit
	}
	bool isMute() const{ return m_mute; }
	void setMute(bool mute) { 
	//	qDebug() << property("mute");
		m_mute = mute; 
		this->style()->polish(this);
	}
protected:
private:
	bool m_mute;
};
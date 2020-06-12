#pragma once

#include <QWidget>
#include <QPixmap>
#include <QBitmap>
#include <QVBoxLayout>
#include <QLabel>
#include <QPainter>
#include <QDebug>


/*
	像手机聊天一样，QLabel显示图片，自带圆角
*/
class PixLabel : public QLabel
{
	Q_OBJECT

public:
	PixLabel(QWidget *parent=0): QLabel(parent){
//		setWindowFlag(Qt::FramelessWindowHint, true);
        loadPixmap(":/Test/Resources/wwwwww.jpg");
	}

// 	//适合固定大小和固定图片
// 	PixLabel(QString url,QWidget *parent = 0) : QLabel(parent) {
// 		setStyleSheet(QString(
// 			"min-width:  100px;max-width:  100px;min-height: 100px;max-height: 100px;"
// 			"border-radius: 50px;"
// 			"border-width: 0 0 0 0;"
// 			"border-image: url(%1) 0 0 0 0 stretch strectch;"
// 		).arg(url));
// 	}

	void loadPixmap(QString url, QSize s=QSize(200,200)) {
		this->setPixmap(QPixmap(url));
		
		QPixmap bitPix = QPixmap(":/Test/Resources/cbg_l.png").scaled(s);
		setMask(bitPix.mask());
		resize(s);
	}
	void loadPixmap(QString url, int topL, int topR, int buttomL, int buttomR, QSize s = QSize(200, 200)) {
		QPixmap pix = QPixmap(url).scaled(s);
		qDebug() << pix.size();
		setPixmap(PixmapToRound(pix, topL, topR, buttomL, buttomR));
	}
private:
	//支持
	QPixmap PixmapToRound(const QPixmap &src, int topL, int topR, int buttomL, int buttomR)
	{
		int radius = getRadius(topL, topR, buttomL, buttomR);
		if (src.isNull() && radius) {
			return QPixmap();
		}


		QSize size=src.size();
		QBitmap mask(size);
		QPainter painter(&mask);
		painter.setRenderHint(QPainter::Antialiasing);			//消除边缘
		painter.setRenderHint(QPainter::SmoothPixmapTransform);	//平滑（算法）
		painter.fillRect(0, 0, size.width(), size.height(), Qt::white);
		painter.setBrush(QColor(0, 0, 0));
		painter.drawRoundedRect(0, 0, size.width(), size.height(), radius, radius);//圆角

		//填充圆角
		if (!topL) { painter.drawRect(0, 0, radius, radius); }
		if (!topR) { painter.drawRect(size.width()-radius, 0, radius, radius); }

		QPixmap image = src.scaled(size);
		image.setMask(mask);
		return image;
	}
	int getRadius(int topL, int topR, int buttomL, int buttomR) {
		if (topL)return topL;
		if (topR)return topR;
		if (buttomL)return buttomL;
		if (buttomR)return buttomR;
	}
};

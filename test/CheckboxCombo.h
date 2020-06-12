#pragma once

#include <QComboBox>
#include <QListView>
#include <QMouseEvent>
#include <QDebug>
#include <QList>

class CheckboxCombo : public QComboBox
{
	Q_OBJECT
public:
	explicit CheckboxCombo(QWidget *parent = nullptr)
		: QComboBox(parent)
	{
		view()->viewport()->installEventFilter(this);
		setEditable(true);
	}
	void shwoExample() {
		QPixmap icon_red(30, 30), icon_green(30, 30), icon_blue(30, 30);
		icon_red.fill(Qt::red);
		icon_green.fill(Qt::green);
		icon_blue.fill(Qt::blue);

		this->setFixedSize(200, 30);
		this->addItem("Red", QStringLiteral("数据"));
	//	this->setItemIcon(0, QIcon(icon_red));
		this->addItem("Green", QStringLiteral("数据"));
	//	this->setItemIcon(1, QIcon(icon_green));
		this->addItem("Blue", QStringLiteral("数据"));
		this->addItem("Blue", QStringLiteral("数据"));
		this->addItem("Blue", QStringLiteral("数据"));

		setChecked(QList<int>() << 3 << 4 << 0);
	//	this->setItemIcon(2, QIcon(icon_blue));

		connect(this, &QComboBox::currentTextChanged, this, [=] {qDebug() << currentText(); });
	//	connect(this, QOverload<const QString &>::of(&QComboBox::currentIndexChanged),[=](const QString &text){qDebug() << "index:" << text; });
	}
	bool eventFilter(QObject * watched, QEvent * event) override {
		if (event->type() == QEvent::MouseButtonRelease)
		{
			QModelIndex ind = view()->indexAt(((QMouseEvent*)event)->pos());
			bool checked = view()->model()->data(ind, Qt::CheckStateRole).toBool();
			view()->model()->setData(ind, !checked, Qt::CheckStateRole);
			return true;
		}
		return QObject::eventFilter(watched, event);
	}
	void hidePopup() override {
		QStringList values;
		for (int i = 0; i < count(); i++) {
			if (itemData(i, Qt::CheckStateRole).toBool()) {
				values << itemText(i);
				setCurrentIndex(i);
			}
		}
		setCurrentText(values.join(_delimiter));
		emit selectedText(values);
		QComboBox::hidePopup();
	}
	void setChecked(QList<int> idxs) {
		QString values;
		foreach(int idx, idxs) {
			values += QString(itemText(idx)+',');
		}
		;
		setCurrentText(values.remove(values.length() - 1, 1));
	}
	void showPopup() override {
		QStringList values = currentText().split(_delimiter);

		for (int i = 0; i < count(); i++) {
			setItemData(i, values.contains(itemText(i)), Qt::CheckStateRole);
		}
		QComboBox::showPopup();
	}
	void setDelimiter(QString str) { _delimiter = str; }

private:
	QString _delimiter = ",";

signals:
	void selectedText(QStringList);
};
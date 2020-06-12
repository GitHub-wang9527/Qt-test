#pragma once

#include <QScrollArea>
#include <QScrollArea>
#include <QPushButton>
#include <QButtonGroup>
#include <QGridLayout>
#include <QLabel>
#include <QList>

class ScrollArea : public QScrollArea
{
	Q_OBJECT

public:
	ScrollArea(int col, bool itemIsButton = true, QWidget *parent = 0) : QScrollArea(parent), col_count(col), m_itemIsButton(itemIsButton) {
		initArea();
	}

	void addItem(QWidget *p) {
		if (p) {
			if (m_itemIsButton) { m_pBGroup->addButton(qobject_cast<QAbstractButton*>(p)); }
			m_items.append(p);
			updateItem();
		}
	};
	void removeItem(QWidget*p) {
		m_items.removeAll(p);
		if (m_itemIsButton) { m_pBGroup->removeButton(qobject_cast<QAbstractButton*>(p)); }
		updateItem();
	}
protected:
	void updateItem()
	{
		//清空
		QLayoutItem *child;
		while ((child = pLayout->takeAt(0)) != 0)
		{
			child->widget()->setVisible(false);
		}

		//重写加入
		int count = 0;
		foreach(QWidget * p, m_items) {
			int row = count / col_count;
			int col = count % col_count;

			pLayout->addWidget(p, row, col, 1, 1);
			p->setVisible(true);
			++count;
		}

		//加空白
		if (pLayout->count() <= col_count) {
			while (pLayout->count() < col_count)
			{
				pLayout->addWidget(new QLabel, 0, pLayout->count(), 1, 1);
			}
			pLayout->addWidget(new QLabel, 1, 0, 1, 1);
		}
		if (pLayout->count() <= col_count * 2) { pLayout->addWidget(new QLabel, 2, 0, 1, 1); }
		if (pLayout->count() <= col_count * 3) { pLayout->addWidget(new QLabel, 3, 0, 1, 1); }
	}
private:
	void initArea() {
		QWidget *w = new QWidget(this);
		setFrameShape(QFrame::NoFrame);//去除可能出现的边框
		setWidget(w);
		setWidgetResizable(true);//滚动区域内的界面不自适应
		m_pBGroup = new QButtonGroup(this);

		pLayout = new QGridLayout(w);
		pLayout->setContentsMargins(10, 0, 10, 0);
		pLayout->setSpacing(20);
	}
protected:
	QButtonGroup * m_pBGroup;
	QGridLayout * pLayout;
	QList<QWidget*> m_items;

	const int col_count;
	const bool m_itemIsButton;
};


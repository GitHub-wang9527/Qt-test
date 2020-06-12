#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include <QDebug>
#include <QPushButton>
#include <QCloseEvent>
#include <QPainter>

/*

事件的派发和处理
	首先说明Qt中事件过滤器的概念. 事件过滤器是Qt中一个独特的事件处理机制, 功能强大而且使用起来灵活方便. 通过它, 可以让一个对象侦听拦截另外一个对象的事件. 
	QObject中有一个类型为QObjectList的成员变量,名字为eventFilters,当某个QObjec (qobjA)给另一个QObject (qobjB)安装了事件过滤器之后, qobjB会把qobjA的指针保存在eventFilters中. 在qobjB处理事件之前,会先去检查eventFilters列表, 如果非空, 就先调用列表中对象的eventFilter()函数. 
	一个对象可以给多个对象安装过滤器. 同样, 一个对象能同时被安装多个过滤器,。
	在事件到达之后, 这些过滤器以安装次序的反序被调用. 事件过滤器函数( eventFilter() ) 返回值是bool型, 如果返回true, 则表示该事件已经被处理完毕, Qt将直接返回, 进行下一事件的处理; 如果返回false, 事件将接着被送往剩下的事件过滤器或是目标对象进行处理.


Qt中,事件的派发是从QApplication::notify() 开始的, 
	如果有事件过滤器安装在qApp上, 先调用这些事件过滤器. 
	接下来QApplication::notify() 会过滤或合并一些事件(比如失效widget的鼠标事件会被过滤掉, 而同一区域重复的绘图事件会被合并). 之后,事件被送到reciver::event() 处理.
同样, 在reciver::event()中, 先检查有无事件过滤器安装在reciever上. 若有, 则调用之. 
接下来,根据QEvent的类型, 调用相应的特定事件处理函数. 


重载特定事件处理函数, 比如:mousePressEvent(), focusOutEvent()等等. 
但对于那些不常见的事件, 是没有相对应的特定事件处理函数的. 如果要处理这些事件, 就需要使用别的办法, 比如重载event() 函数, 或是安装事件过滤器.

*/

/*
事件的转发
 对于某些类别的事件, 如果在整个事件的派发过程结束后还没有被处理, 那么这个事件将会向上转发给它的父widget, 直到最顶层窗口. 如图所示, 事件最先发送给QCheckBox, 如果QCheckBox没有处理, 那么由QGroupBox接着处理, 如果QGroupBox没有处理, 再送到QDialog, 因为QDialog已经是最顶层widget, 所以如果QDialog不处理, QEvent将停止转发.
如何判断一个事件是否被处理了呢? 
Qt中和事件相关的函数通过两种方式相互通信.
	1、QApplication::notify(), QObject::eventFilter(), QObject::event() 通过返回bool值来表示是否已处理. “真”表示已经处理, “假”表示事件需要继续传递. 
	2、另一种是调用QEvent::ignore() 或 QEvent::accept() 对事件进行标识. 这种方式只用于event() 函数和特定事件处理函数之间的沟通. 而且只有用在某些类别事件上是有意义的, 这些事件就是上面提到的那些会被转发的事件, 包括: 鼠标, 滚轮, 按键等事件.
*/

/*
4.给QAppliction对象安装事件过滤器
一旦我们给qApp(每个程序中唯一的QApplication对象)装上过滤器,那么所有的事件在发往任何其他的过滤器时,都要先经过当前这个eventFilter(). 在debug的时候,这个办法就非常有用, 也常常被用来处理失效了的widget的鼠标事件,通常这些事件会被QApplication::notify()丢掉. ( 在QApplication::notify() 中, 是先调用qApp的过滤器, 再对事件进行分析, 以决定是否合并或丢弃)


5.继承QApplication类,并重载notify()函数
Qt是用QApplication::notify()函数来分发事件的.想要在任何事件过滤器查看任何事件之前先得到这些事件,重载这个函数是唯一的办法. 通常来说事件过滤器更好用一些, 因为不需要去继承QApplication类. 而且可以给QApplication对象安装任意个数的事件过滤器, 相比之下, notify()函数只有一个.

总结：
	Qt中,事件的派发是从QApplication::notify() 开始的, 因为QAppliction也是继承自QObject, 所以先检查QAppliation对象, 如果有事件过滤器安装在qApp上, 先调用这些事件过滤器. 
	接下来QApplication::notify() 会过滤或合并一些事件(比如失效widget的鼠标事件会被过滤掉, 而同一区域重复的绘图事件会被合并). 
	之后,事件被送到reciver::event() 处理.

	同样, 在reciver::event()中, 先检查有无事件过滤器安装在reciever上. 
	接下来,根据QEvent的类型, 调用相应的特定事件处理函数.

*/


/*
总结
Qt的事件处理实际上是分层五个层次：
	重定义事件处理函数，
	重定义 event()函数，
	为单个组件安装事件过滤器（这里应该指的是上面的notify_helper源码中目标对象的事件过滤器），
	为 QApplication 安装事件过滤器（这里应该指的是上面的notify_helper源码中全局的事件过滤器），
	重定义QCoreApplication的 notify()函数。这几个层次的控制权是逐层增大的。
*/
class Filter :public QObject
{
	Q_OBJECT
public:
	static Filter * instance() {
		static Filter *m_ins = NULL;
		if (m_ins == NULL) {
			m_ins = new Filter;
		}
		return m_ins;
	}
	bool eventFilter(QObject *watched, QEvent *event) {
		if (event->type() == QEvent::KeyPress){
			QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
			if (keyEvent->key() == Qt::Key_Tab){
				// 处理Tab键      
				qDebug() << "intercept Key_Tab";
				return true;	//事件不会继续传递
			}
		}
		else if (event->type() == QEvent::MouseButtonPress) {
			QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
			if (mouseEvent->button() == Qt::LeftButton) {
			//	return true;
			}
		}
		return false;
	}
protected:
};

class Sub : public QWidget
{
	Q_OBJECT
		int idx;
public:
	Sub(int index, QWidget *parent = 0) :QWidget(parent), idx(index) {
		this->installEventFilter(Filter::instance());

	}

	void paintEvent(QPaintEvent *event) {
		QPainter p(this);

		p.drawRect(0, 0, width(), height());
		return QWidget::paintEvent(event);
	}
	void mousePressEvent(QMouseEvent *event) {
		if (event->button() == Qt::LeftButton) {
			qDebug() << "Sub left Pressed";
		}
		else {
			qDebug() << "Sub right Pressed";
		//	return QWidget::mousePressEvent(event);	//将事件传递给父类
		//	event->ignore();	//也会将事件传递给父类

		//	event->accept();	//阻止事件继续向上传递
		//  什么都不写的话，事件也不会向上传递
		}
	}

	void keyPressEvent(QKeyEvent *event) {
		qDebug() << "sub: key";

	}
};

class EventFWidget : public QWidget
{
	Q_OBJECT

public:
	EventFWidget(QWidget *parent = 0) :QWidget(parent) {
		QVBoxLayout * pLayout = new QVBoxLayout(this);
		pLayout->addWidget(new Sub(1));
		pLayout->addWidget(new Sub(2));
		resize(600, 400);

		this->installEventFilter(Filter::instance());
	}

protected:
	void closeEvent(QCloseEvent *event) {
		//event->ignore();	//忽略该信号，无法关闭窗口
	}

	void mousePressEvent(QMouseEvent *event) {
		qDebug() << "Father Pressed";
	}

	void keyPressEvent(QKeyEvent *event) {
		qDebug() << "fateher: key";
	}

};

#include "ServerView.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QIntValidator>

ServerView::ServerView(QWidget* parent) : QWidget(parent), isRunning_(false)
{
	setWindowTitle("Chat Server");
	resize(500, 400);

	QVBoxLayout* layout = new QVBoxLayout(this);

	// Верхняя панель с настройками
	QHBoxLayout* topLayout = new QHBoxLayout;
	topLayout->addWidget(new QLabel("IP:"));
	ipEdit_ = new QLineEdit("0.0.0.0");
	topLayout->addWidget(ipEdit_);

	topLayout->addWidget(new QLabel("Port:"));
	portEdit_ = new QLineEdit("12345");
	portEdit_->setValidator(new QIntValidator(1, 65535, this));
	topLayout->addWidget(portEdit_);

	layout->addLayout(topLayout);

	// Кнопка Старт/Стоп
	startStopBtn_ = new QPushButton("Start Server");
	layout->addWidget(startStopBtn_);

	// Статус
	statusLabel_ = new QLabel("Status: Stopped");
	layout->addWidget(statusLabel_);

	// Лог
	logEdit_ = new QTextEdit;
	logEdit_->setReadOnly(true);
	layout->addWidget(logEdit_);

	connect(startStopBtn_, &QPushButton::clicked, this, &ServerView::onStartStopClicked);
}

void ServerView::onStartStopClicked()
{
	if (!isRunning_)
	{
		emit signalStartServerRequested(QHostAddress(ipEdit_->text()), portEdit_->text().toUShort());
	}
	else
	{
		emit signalStopServerRequested();
	}
}

void ServerView::slotAppendLog(const QString& text)
{
	logEdit_->append(QDateTime::currentDateTime().toString("hh:mm:ss") + " " + text);
}

void ServerView::slotSetServerStatus(bool running)
{
	isRunning_ = running;
	startStopBtn_->setText(running ? "Stop Server" : "Start Server");
	statusLabel_->setText(running ? "Status: Running" : "Status: Stopped");
	ipEdit_->setEnabled(!running);
	portEdit_->setEnabled(!running);
}




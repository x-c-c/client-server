// Окно управления сервером: запуск/остановка, отображение логов

#ifndef SERVERVIEW_H
#define SERVERVIEW_H

#include <QWidget>
#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QHostAddress>
#include <QDateTime>

class ServerView : public QWidget
{
	Q_OBJECT
public:
	explicit ServerView(QWidget* parent = nullptr);

public slots:
	// Добавить строку в лог
	void slotAppendLog(const QString& text);

	// Обновить состояние кнопок при изменении статуса сервера
	void slotSetServerStatus(bool running);

signals:
	void signalStartServerRequested(const QHostAddress& address, quint16 port);
	void signalStopServerRequested();

private slots:
	void onStartStopClicked();

private:
	QLineEdit* ipEdit_;
	QLineEdit* portEdit_;
	QPushButton* startStopBtn_;
	QTextEdit* logEdit_;
	QLabel* statusLabel_;
	bool isRunning_;
};

#endif





//Created by July IGHOR
//http://trader.uax.co
//Bitcoin Donate: 1d6iMwjjNo8ZGYeJBZKXgcgVk9o7fXcjc

#ifndef AUDIOPLAYER_H
#define AUDIOPLAYER_H

#include <QAudioOutput>
#include <QObject>
#include <QTimer>

class Generator : public QIODevice
{
	Q_OBJECT
public:
	Generator(const QAudioFormat &format, qint64 durationUs, int frequency, QObject *parent);
	~Generator();

	void start();
	void stop();

	qint64 readData(char *data, qint64 maxlen);
	qint64 writeData(const char *data, qint64 len);
	qint64 bytesAvailable() const;

private:
	void generateData(const QAudioFormat &format, qint64 durationUs, int frequency);

private:
	qint64 m_pos;
	QByteArray m_buffer;
};

class AudioPlayer : public QObject
{
	Q_OBJECT

public:
	void beep();
	AudioPlayer(QObject *parent);
	~AudioPlayer();

private:
	QByteArray       m_buffer;
	QTimer*          m_timeOutTimer;
	QAudioDeviceInfo m_device;
	Generator*       m_generator;
	QAudioOutput*    m_audioOutput;
	QIODevice*       m_output; // not owned
	QAudioFormat     m_format;
private slots:
	void pullTimerExpired();
};

#endif // AUDIOPLAYER_H


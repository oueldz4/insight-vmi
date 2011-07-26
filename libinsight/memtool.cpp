/*
 * memtool.cpp
 *
 *  Created on: 02.08.2010
 *      Author: chrschn
 */

#include <QDir>
#include <QProcess>
#include <QThread>
#include <QDataStream>
#include <QCoreApplication>
#include <QMetaType>
#include <memtool/constdefs.h>
#include <memtool/memtool.h>
#include <memtool/devicemuxer.h>
#include <memtool/memtoolexception.h>
#include "sockethelper.h"
#include "debug.h"

// Register type that might need to be queued
Q_DECLARE_METATYPE(QAbstractSocket::SocketState)
static int socket_state_id = qRegisterMetaType<QAbstractSocket::SocketState>();
// Use the symbol socket_state_id once to make the compiler happy
void socket_state_id_foo() { Q_UNUSED(socket_state_id); }


#define connectOrFail() \
    do { \
        if (connectToDaemon() != crOk) \
            memtoolError("Could not connect to memtool daemon");\
    } while (0)


Memtool::Memtool()
    : _helper(new SocketHelper())
{
}


Memtool::~Memtool()
{
    delete _helper;
}


int Memtool::connectToDaemon()
{
    QString sockFileName = QDir::home().absoluteFilePath(mt_sock_file);
    if (!QFile::exists(sockFileName))
        return crDaemonNotRunning;

    // Are we already connected?
    if (_helper->state() == QLocalSocket::ConnectingState ||
            _helper->state() == QLocalSocket::ConnectedState)
        return crOk;

    // Try to connect
    _helper->connectToServer(sockFileName);

    if (!_helper->waitForConnected(1000))
    	return crDaemonNotRunning;
    // Wait for connection acknowledgment
    if (!_helper->ret()->waitForReadyRead(5000))
    	return crUnknownError;
    // The daemon accepts the connection if he returns 0
    int ret;
	QDataStream retStream(_helper->ret());
	retStream >> ret;

    return ret;
}


void Memtool::disconnectFromDaemon()
{
    // Are we already connected?
    if (_helper->state() == QLocalSocket::ConnectingState &&
            _helper->state() == QLocalSocket::ConnectedState)
		// Disconnect
        _helper->disconnectFromServer();
}


int Memtool::eval(const QString& cmd)
{
    connectOrFail();

    _helper->clear();
    QString realCmd = cmd.trimmed() + "\n";
    // Execute the command and wait until we received a return value
    _helper->write(realCmd.toLocal8Bit());
    _helper->ret()->waitForReadyRead(-1);
    // Try to read the return value
    int ret = 0;
    if (_helper->ret()->bytesAvailable() >= (int)sizeof(ret)) {
    	QDataStream retStream(_helper->ret());
    	retStream >> ret;
    }
    return ret;
}


bool Memtool::outToStdOut() const
{
	return _helper->outToStdOut();
}


QString Memtool::readAllStdOut()
{
	QByteArray buf = _helper->out()->readAll();
	return QString::fromLocal8Bit(buf.constData(), buf.size());
}


QString Memtool::readAllStdErr()
{
	QByteArray buf = _helper->err()->readAll();
	return QString::fromLocal8Bit(buf.constData(), buf.size());
}


QByteArray Memtool::readAllBinary()
{
	return _helper->bin()->readAll();
}


void Memtool::setOutToStdOut(bool value)
{
	_helper->setOutToStdOut(value);
}


bool Memtool::errToStdErr() const
{
	return _helper->errToStdErr();
}


void Memtool::setErrToStdErr(bool value)
{
	_helper->setErrToStdErr(value);
}


bool Memtool::isDaemonRunning()
{
	int ret = connectToDaemon();
    return ret == crOk || ret == crTooManyConnections;
}


bool Memtool::daemonStart()
{
    if (isDaemonRunning())
        return true;

    QString cmd = "memtoold";
    QStringList args("--daemon");

    QProcess proc;
    proc.start(cmd, args);
    proc.waitForFinished(-1);

    return proc.exitCode() == 0 && proc.error() == QProcess::UnknownError;
}


bool Memtool::daemonStop()
{
	int ret = 0;
    if (isDaemonRunning())
        ret = eval("exit");
    return ret == 0;
}


QStringList Memtool::memDumpList()
{
	QString cmd = QString("binary %1").arg((int) bdMemDumpList);
    int ret = eval(cmd);
    if (ret)
    	memtoolError(QString("Error executing command \"%1\", error code #%2")
    			.arg(cmd)
    			.arg(ret));

    QStringList list;
    QByteArray ba = _helper->bin()->readAll();
    QDataStream in(ba);
    in >> list;
    return list;
}


bool Memtool::memDumpLoad(const QString& fileName)
{
    int ret = eval(QString("memory load %1").arg(fileName));
    return ret == 0;
}


bool Memtool::memDumpUnload(const QString& fileName)
{
    int ret = eval(QString("memory unload %1").arg(fileName));
    return ret == 0;
}


bool Memtool::memDumpUnload(int index)
{
    return memDumpUnload(QString::number(index));
}

#include "rosslclientio.h"
#include "sslremoteobjects.h"

#include <QHostAddress>
#include <QHostInfo>
#include <QDebug>

RoSslClientIo::RoSslClientIo(QObject *parent) :
	ClientIoDevice(parent),
	_socket(new QSslSocket(this))
{
	connect(_socket, &QSslSocket::readyRead,
			this, &RoSslClientIo::readyRead);
	connect(_socket, QOverload<QAbstractSocket::SocketError>::of(&QSslSocket::error),
			this, &RoSslClientIo::onError);
	connect(_socket, QOverload<const QList<QSslError> &>::of(&QSslSocket::sslErrors),
			this, &RoSslClientIo::onSslErrors);
	connect(_socket, &QSslSocket::stateChanged,
			this, &RoSslClientIo::onStateChanged);
}

RoSslClientIo::~RoSslClientIo()
{
	close();
}

QIODevice *RoSslClientIo::connection()
{
	return _socket;
}

void RoSslClientIo::connectToServer()
{
	if (isOpen())
		return;

	QHostAddress address(url().host());
	if(address.isNull()) {
		const auto addresses = QHostInfo::fromName(url().host()).addresses();
		if(addresses.isEmpty())
			return;
		address = addresses.first();
	}

	auto conf = SslRemoteObjects::prepareFromUrl(url());
	if(conf.isNull())
		return;

	_socket->setSslConfiguration(conf);
	_socket->connectToHostEncrypted(address.toString(), url().port());
}

bool RoSslClientIo::isOpen()
{
	return (!isClosing() && (_socket->state() == QAbstractSocket::ConnectedState
							 || _socket->state() == QAbstractSocket::ConnectingState));
}

void RoSslClientIo::onError(QAbstractSocket::SocketError error)
{
	qWarning() << Q_FUNC_INFO << _socket->errorString();

	switch (error) {
	case QAbstractSocket::HostNotFoundError:     //Host not there, wait and try again
	case QAbstractSocket::ConnectionRefusedError:
		emit shouldReconnect(this);
		break;
	default:
		break;
	}
}

void RoSslClientIo::onSslErrors(const QList<QSslError> &errors)
{
	for(auto error : errors)
		qWarning() << Q_FUNC_INFO << error.errorString();
}

void RoSslClientIo::onStateChanged(QAbstractSocket::SocketState state)
{
	if (state == QAbstractSocket::ClosingState && !isClosing()) {
		_socket->abort();
		emit shouldReconnect(this);
	}
	if (state == QAbstractSocket::ConnectedState) {
		m_dataStream.setDevice(connection());
		m_dataStream.resetStatus();
	}
}

void RoSslClientIo::doClose()
{
	if(_socket->isOpen()) {
		connect(_socket, &QSslSocket::disconnected,
				this, &QObject::deleteLater);
		_socket->disconnectFromHost();
	} else
		deleteLater();
}

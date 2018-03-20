#include "rosslserverio.h"
#include "sslremoteobjects.h"

#include <QHostInfo>

RoSslServerIo::RoSslServerIo(QSslSocket *socket, QObject *parent) :
	ServerIoDevice(parent),
	_socket(socket)
{
	_socket->setParent(this);
	connect(_socket, &QSslSocket::readyRead,
			this, &RoSslServerIo::readyRead);
	connect(_socket, &QSslSocket::disconnected,
			this, &RoSslServerIo::disconnected);
	connect(_socket, QOverload<QAbstractSocket::SocketError>::of(&QSslSocket::error),
			this, &RoSslServerIo::onError);
	connect(_socket, QOverload<const QList<QSslError> &>::of(&QSslSocket::sslErrors),
			this, &RoSslServerIo::onSslErrors);
}

QIODevice *RoSslServerIo::connection() const
{
	return _socket;
}

void RoSslServerIo::onError(QAbstractSocket::SocketError error)
{
	Q_UNUSED(error)
	qWarning() << Q_FUNC_INFO << _socket->errorString();
}

void RoSslServerIo::onSslErrors(const QList<QSslError> &errors)
{
	for(auto error : errors)
		qWarning() << Q_FUNC_INFO << error.errorString();
}

void RoSslServerIo::doClose()
{
	_socket->disconnectFromHost();
}



RoSslServer::RoSslServer(QObject *parent) :
	QConnectionAbstractServer(parent),
	_server(new QSslServer(this)),
	_originalUrl()
{
	connect(_server, &QSslServer::newConnection,
			this, &RoSslServer::newConnection);
	connect(_server, QOverload<QAbstractSocket::SocketError>::of(&QSslServer::acceptError),
			this, &RoSslServer::onAcceptError);
}

RoSslServer::~RoSslServer()
{
	close();
}

bool RoSslServer::hasPendingConnections() const
{
	return _server->hasPendingConnections();
}

ServerIoDevice *RoSslServer::configureNewConnection()
{
	if(!_server->isListening() || !_server->hasPendingConnections())
		return nullptr;
	else
		return new RoSslServerIo(_server->nextPendingConnection());
}

QUrl RoSslServer::address() const
{
	return _originalUrl;
}

bool RoSslServer::listen(const QUrl &address)
{
	QHostAddress host(address.host());
	if (host.isNull()) {
		if(address.host().isEmpty()) {
			host = QHostAddress::Any;
		} else {
			qWarning() << address.host() << " is not an IP address, trying to resolve it";
			auto info = QHostInfo::fromName(address.host());
			if (info.addresses().isEmpty())
				host = QHostAddress::Any;
			else
				host = info.addresses().constFirst();
		}
	}

	auto conf = SslRemoteObjects::prepareFromUrl(address);
	if(conf.isNull())
		return false;

	_server->setSslConfiguration(conf);
	if(_server->listen(host, address.port())) {
		_originalUrl = address;
		return true;
	} else
		return false;
}

QAbstractSocket::SocketError RoSslServer::serverError() const
{
	//cannot return ssl errors for now
	return _server->serverError();
}

void RoSslServer::close()
{
	_server->close();
}

void RoSslServer::onAcceptError(QAbstractSocket::SocketError socketError)
{
	Q_UNUSED(socketError)
	qWarning() << Q_FUNC_INFO << _server->errorString();
}

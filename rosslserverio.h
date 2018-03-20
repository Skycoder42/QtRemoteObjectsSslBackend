#ifndef ROTLSSERVERIO_H
#define ROTLSSERVERIO_H

#include <QObject>
#include <QSslSocket>
#include <qsslserver.h>
#include <QtRemoteObjects/QConnectionAbstractServer>

class RoSslServerIo : public ServerIoDevice
{
	Q_OBJECT
public:
	explicit RoSslServerIo(QSslSocket *socket, QObject *parent = nullptr);

	QIODevice *connection() const override;

public Q_SLOTS:
	void onError(QAbstractSocket::SocketError error);
	void onSslErrors(const QList<QSslError> &errors);

protected:
	void doClose() override;

private:
	QSslSocket *_socket;
};

class RoSslServer : public QConnectionAbstractServer
{
	Q_OBJECT

public:
	explicit RoSslServer(QObject *parent);
	~RoSslServer();

	bool hasPendingConnections() const override;
	ServerIoDevice *configureNewConnection() override;
	QUrl address() const override;
	bool listen(const QUrl &address) override;
	QAbstractSocket::SocketError serverError() const override;
	void close() override;

private Q_SLOTS:
	void onAcceptError(QAbstractSocket::SocketError socketError);

private:
	QSslServer *_server;
	QUrl _originalUrl;
};

#endif // ROTLSSERVERIO_H

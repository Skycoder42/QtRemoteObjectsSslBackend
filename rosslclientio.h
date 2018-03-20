#ifndef ROTLSCLIENTIO_H
#define ROTLSCLIENTIO_H

#include <QObject>
#include <QSslSocket>
#include <QtRemoteObjects/QConnectionAbstractServer>

class RoSslClientIo : public ClientIoDevice
{
	Q_OBJECT

public:
	explicit RoSslClientIo(QObject *parent = nullptr);
	~RoSslClientIo() override;

	QIODevice *connection() override;
	void connectToServer() override;
	bool isOpen() override;

public Q_SLOTS:
	void onError(QAbstractSocket::SocketError error);
	void onSslErrors(const QList<QSslError> &errors);
	void onStateChanged(QAbstractSocket::SocketState state);

protected:
	void doClose() override;

private:
	QSslSocket *_socket;
};

#endif // ROTLSCLIENTIO_H

#ifndef TLSREMOTEOBJECTS_H
#define TLSREMOTEOBJECTS_H

#include <QSslConfiguration>
#include <QString>
#include <QUrl>

namespace SslRemoteObjects
{

extern const QString UrlScheme;

// ssl://<host>:<port>/p12?path=<path>&pass=<passPhrase>
QUrl generateP12Url(const QString &host, quint16 port,
					const QString &path,
					const QString &passPhrase = QString());
// ssl://<host>:<port>/conf?key=<config>
QUrl generateConfigUrl(const QString &host, quint16 port,
					   const QSslConfiguration &config);
//! ssl://<host>:<post>/plain
QUrl generatePlainUrl(const QString &host, quint16 port);

QSslConfiguration prepareFromUrl(const QUrl &url);

QString prepareSslConfig(const QSslConfiguration &config);
QSslConfiguration getSslConfig(const QString &key);

}

#endif // TLSREMOTEOBJECTS_H

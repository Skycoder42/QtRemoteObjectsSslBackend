# QtRemoteObjectsSslBackend
A backend for [QtRemoteObjects](https://doc.qt.io/qt-5/qtremoteobjects-index.html) to connect via an encrypted SSL connection.

## Usage
Once added to you project, the backend will automatically register itself. All you need to to is to create a remote object host/node using a special `ssl://...` url. The tricky part is that for SSL/TLS, we need certificates - but certificates can hardly be passed via an url. Thus, a number of possibilities are presented to make it work by encoding special information in the url.

In order to use it, create an url using one of the provided methods. The possible options are:

- `SslRemoteObjects::generateP12Url`: Creates an url from the path to a p12 container. The backend will automatically load the certificates and key from that file and use it
- `SslRemoteObjects::generateConfigUrl`: Caches a custom QSslConfiguration and passes an identifier with the url, which is the used by the backend to load the config again.
- `SslRemoteObjects::generatePlainUrl`: Uses the standard QSslConfiguration::defaultConfiguration() for the server. You will have to modify that one to add a local certificate and private key.

The recommended one is the first variant, or the second, if you need finer configuration than what the p12 variant provides.

## Example
As the package is mostly backend code, the example shows how to use the p12 variant to create a secure remote object host node. As long as you want to authenticate both sides (recommended), the code stays the same for the client:

```cpp
auto url = SslRemoteObjects::generateP12Url("127.0.0.1", 4711, "/path/to/cert.p12", "password");
auto host = new QRemoteObjectHost(url, this);
// ...
```

## Installation
The package is providet as qpm  package, [`de.skycoder42.qtremoteobjectsbackend.ssl`](https://www.qpm.io/packages/de.skycoder42.qtremoteobjectsbackend.ssl/index.html). You can install it either via qpmx (preferred) or directly via qpm.

**Note:** The package depends on [QSslServer](https://github.com/Skycoder42/QSslServer). When using qpmx/qpm the
dependency is installed automatically. Otherwise you need to add it to your project manually.

### Via qpmx
[qpmx](https://github.com/Skycoder42/qpmx) is a frontend for qpm (and other tools) with additional features, and is the preferred way to install packages. To use it:

1. Install qpmx (See [GitHub - Installation](https://github.com/Skycoder42/qpmx#installation))
2. Install qpm (See [GitHub - Installing](https://github.com/Cutehacks/qpm/blob/master/README.md#installing), for **windows** see below)
3. In your projects root directory, run `qpmx install de.skycoder42.qtremoteobjectsbackend.ssl`

### Via qpm

1. Install qpm (See [GitHub - Installing](https://github.com/Cutehacks/qpm/blob/master/README.md#installing), for **windows** see below)
2. In your projects root directory, run `qpm install de.skycoder42.qtremoteobjectsbackend.ssl`
3. Include qpm to your project by adding `include(vendor/vendor.pri)` to your `.pro` file

Check their [GitHub - Usage for App Developers](https://github.com/Cutehacks/qpm/blob/master/README.md#usage-for-app-developers) to learn more about qpm.

**Important for Windows users:** QPM Version *0.10.0* (the one you can download on the website) is currently broken on windows! It's already fixed in master, but not released yet. Until a newer versions gets released, you can download the latest dev build from here:
- https://storage.googleapis.com/www.qpm.io/download/latest/windows_amd64/qpm.exe
- https://storage.googleapis.com/www.qpm.io/download/latest/windows_386/qpm.exe
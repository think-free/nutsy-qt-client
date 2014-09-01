#include "platforminfo.h"

PlatformInfo::PlatformInfo(QString app,QObject *parent) : QObject(parent)
{
    m_package = "";
    m_tactileScreen = false;

    // Define platform variables

    QString OS;
    QString STORAGEPATH;
    bool TACTIL;

    #ifdef Q_OS_LINUX
        OS = "Linux";
        STORAGEPATH = QDir::homePath ();
        TACTIL = false;
    #endif
    #ifdef Q_OS_WIN
        OS = "Windows";
        STORAGEPATH = QDir::homePath ();
        TACTIL = false;
    #endif
    #ifdef Q_OS_MACX
        OS = "Mac";
        STORAGEPATH = QDir::homePath ();
        TACTIL = false;
    #endif
    #ifdef Q_OS_ANDROID
        OS = "Android";
        STORAGEPATH = getenv("EXTERNAL_STORAGE");
        TACTIL = true;
    #endif
    #ifdef Q_OS_IOS
        OS = "IOS";
        QString cur_dir = QDir::currentPath();
        STORAGEPATH = cur_dir.left( cur_dir.lastIndexOf( "/" ) + 1 ).append("Documents");
        TACTIL = true;
    #endif
    // Create storage folder if doesn't exists

    STORAGEPATH.append("/.");
    STORAGEPATH.append(app);
    STORAGEPATH.append("/");

    // Set platform variables

    setPlatform(OS);
    setTactileScreen(TACTIL);
    setStoragePath(STORAGEPATH);
}

QVariant PlatformInfo::getSetting(QString key, QVariant deflt){
    QString filePath = configFilePath();
    if (filePath.isEmpty())
       filePath = storagePath() + "config.ini";
    qDebug() << "Loading config from : " << filePath;
    QSettings * settings = 0;
    settings = new QSettings( filePath , QSettings::IniFormat );
    QVariant v = settings->value( key, deflt);
    delete settings;

    return v;
}

void PlatformInfo::setSetting(QString key, QVariant value){

    QSettings *settings = 0;

    QString filePath = configFilePath();
    if (filePath.isEmpty())
       filePath = storagePath() + "config.ini";

    qDebug() << "Loading config from : " << filePath;
    settings = new QSettings( storagePath() + filePath, QSettings::IniFormat );
    settings->setValue(key,value);
    delete settings;
}

void PlatformInfo::notify(QString message){

    #if defined(QT_ANDROIDEXTRAS_LIB)

        QAndroidJniObject javaNotification = QAndroidJniObject::fromString(message);

        QAndroidJniObject::callStaticMethod<void>(
                                     m_package.toUtf8(),
                                     "notify",
                                     "(Ljava/lang/String;)V",
                                     javaNotification.object<jstring>());

    #else

        Q_UNUSED(message)

    #endif
}

void PlatformInfo::setPackage(QString package){

    m_package = package;
}

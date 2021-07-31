#include <QCoreApplication>

#include <QFile>
#include <QTextStream>
#include <QString>
#include <QCommandLineParser>
#include <QDebug>

#include <quentier/utility/EncryptionManager.h>
#include <quentier/types/ErrorString.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QCoreApplication::setApplicationName("crackenex");
    QCoreApplication::setApplicationVersion("0.1");

    QCommandLineParser parser;
    parser.setApplicationDescription("Crack/Brute-force hashes from enex files.");
    parser.addHelpOption();
    parser.addVersionOption();

    QCommandLineOption dictionaryOption(QStringList() << "d" << "dict",
                                        QCoreApplication::translate("main", "Use dictionary <dict>"),
                                        QCoreApplication::translate("main", "dict"));
    parser.addOption(dictionaryOption);

    QCommandLineOption enexName(QStringList() << "s" << "source",
                                  QCoreApplication::translate("src", "Base64 encrypted file <source>"),
                                  QCoreApplication::translate("src", "source"));
    parser.addOption(enexName);
    parser.process(a);

    QString dictName = parser.value(dictionaryOption);
    QString encName = parser.value(enexName);

    QString passphrase, encryptedText, decryptedText;
    quentier::EncryptionManager em;
    quentier::ErrorString err;
    bool test;

    QFile dictFile(dictName);
    QFile encFile(encName);

    if(!encFile.open(QIODevice::ReadOnly))
    {
        qInfo() << "Could not open enc file";
        return EXIT_FAILURE;
    }
    QTextStream enc(&encFile);
    encryptedText = enc.readAll();

    if (dictFile.open(QIODevice::ReadOnly))
    {
        QTextStream in(&dictFile);
        while(!in.atEnd())
        {
            passphrase = in.readLine();

            qDebug() << "Try: "  << passphrase;
            test = em.decrypt(encryptedText, passphrase, QStringLiteral("AES"), 128, decryptedText, err);
            if (test == true)
            {
                qInfo() << "Success!";
                qInfo() << "Password: " << passphrase;
                return EXIT_SUCCESS;
            }
        }
        dictFile.close();
    }
    else
    {
        qInfo()  << "Could not open dict file";
        return EXIT_FAILURE;
    }
    qInfo() << "Password not found";
    return a.exec();
}

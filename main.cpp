#include <QCoreApplication>

#include <QFile>
#include <QTextStream>
#include <QString>
#include <QCommandLineParser>
#include <QDebug>
#include <QRegularExpression>

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

    QCommandLineOption enexOption(QStringList() << "s" << "source",
                                  QCoreApplication::translate("src", "Base64 encrypted file <source>"),
                                  QCoreApplication::translate("src", "source"));
    parser.addOption(enexOption);
    parser.process(a);

    QString dictName = parser.value(dictionaryOption);
    QString enexName = parser.value(enexOption);

    QString passphrase, encryptedText, decryptedText;
    quentier::EncryptionManager manager;
    quentier::ErrorString err;
    bool test;

    if(enexName.isEmpty() || dictName.isEmpty())
    {
        qInfo() << "Missing arguments. See --help for more info";
        return EXIT_FAILURE;
    }

    QFile dictFile(dictName);
    QFile enexFile(enexName);

    if(!enexFile.open(QIODevice::ReadOnly))
    {
        qInfo() << "Could not open enc file";
        return EXIT_FAILURE;
    }

    QTextStream enc(&enexFile);
    QString enexContent = enc.readAll();

    QRegularExpression re("<en-crypt[a-zA-Z0-9 =\"]*>(\\K.*?)(?=<\\/en-crypt>)");
    QRegularExpressionMatch result = re.match(enexContent);

    encryptedText =  result.captured(0);
    enexFile.close();

    QTextStream in(stdin);

    if (dictFile.open(QIODevice::ReadOnly))
    {
        QTextStream in(&dictFile);
    }
    else
    {
        qInfo() << "Could not open dict";
        return EXIT_FAILURE;
    }


    while(!in.atEnd())
    {
        passphrase = in.readLine();

        qDebug() << "Try: "  << passphrase;
        test = manager.decrypt(encryptedText, passphrase, QStringLiteral("AES"), 128, decryptedText, err);
        if (test == true)
        {
            qInfo() << "Success!";
            qInfo() << "Password: " << passphrase;
            return EXIT_SUCCESS;
        }
    }

    dictFile.close();

    qInfo() << "Password not found";
    return EXIT_SUCCESS;
}

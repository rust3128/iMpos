#include "mainwindow.h"
#include "LoggingCategories/loggingcategories.h"
#include "DataBases/databases.h"
#include "DataBases/options.h"
#include "LoginDialog/logindialog.h"
#include <QApplication>
#include <QFile>
#include <QDateTime>
#include <QTranslator>
#include <QLibraryInfo>
#include <QMessageBox>

// Умный указатель на файл логирования
static QScopedPointer<QFile>   m_logFile;

// Объявление обработчика
void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Устанавливаем файл логирования
    m_logFile.reset(new QFile("iMpos.log"));
    // Открываем файл логирования
    m_logFile.data()->open(QFile::Append | QFile::Text);
    // Устанавливаем обработчик
    qInstallMessageHandler(messageHandler);
    qInfo(logInfo()) << "Запуск программы.";

#ifndef QT_NO_TRANSLATION
    //Определяем имя языкового файла в зависимости от текущей локали
    QString translatorFileName = QLatin1String("qt_");
    translatorFileName += QLocale::system().name();
    //Создаем и пытаемся загрузить
    QTranslator *translator = new QTranslator(&a);
    if (translator->load(translatorFileName, QLibraryInfo::location(QLibraryInfo::TranslationsPath)))
        a.installTranslator(translator);
    else
        qWarning(logWarning()) << "Не удалось загрузить языковый файл.";
#endif

    DataBases *db = new DataBases();
    if(!db->connectOptions()){
        qInfo(logInfo()) << "Аварийное завершение работы.";
        return 1;
    }

    Options opt;
    //Проверяем значение опции 1000
    if(opt.getOption(1000).toBool()){
        //Создаем диалогвое окно
        LoginDialog *loginDlg = new LoginDialog();
        loginDlg->exec();
        //Если вход не состоялся завершаем работу программы
        if(loginDlg->result() == QDialog::Rejected){
            QMessageBox::critical(nullptr,"Ошибка входа",
                                  "Не выполнен вход в систему!<br>Дальнейшая работа не возможна.");
            qCritical(logCritical()) << "Не выполнен вход в систему. Закрытие программы.";
            return 1;
        }
    }

    if(!db->connectCenralDB()){
        qInfo(logInfo()) << "Завершение работы. Не удалось подключится к центральной базе.";
        return 1;
    }

    MainWindow w;
    w.show();

    return a.exec();
}

void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    // Открываем поток записи в файл
    QTextStream out(m_logFile.data());
    QTextStream console(stdout);


    // Записываем дату записи
    out << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz ");
    // По типу определяем, к какому уровню относится сообщение
    switch (type)
    {
#ifdef QT_DEBUG
    case QtInfoMsg:     out << "[INF] "; console << "Info:     " << msg << endl; break;
    case QtDebugMsg:    out << "[DBG] "; console << "Debug:    " << msg << endl; break;
    case QtWarningMsg:  out << "[WRN] "; console << "Warning:  " << msg << endl; break;
    case QtCriticalMsg: out << "[CRT] "; console << "Critical: " << msg << endl; break;
    case QtFatalMsg:    out << "[FTL] "; console << "Fatality: " << msg << endl; break;
#else
    case QtInfoMsg:     out << "[INF] "; break;
    case QtDebugMsg:    out << "[DBG] "; break;
    case QtWarningMsg:  out << "[WRN] "; break;
    case QtCriticalMsg: out << "[CRT] "; break;
    case QtFatalMsg:    out << "[FTL] "; break;
#endif

    }
    // Записываем в вывод категорию сообщения и само сообщение
    out << context.category << ": " << msg << endl;
    // Очищаем буферизированные данные
    out.flush();
    console.flush();
}

#include "SeaBattleWindow.h"
#include <QtWidgets/QApplication>
#include <QTranslator>
#include <QLibraryInfo>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Перевод текстовых строк Qt
    QTranslator tr;
    // Поиск файла qtbase_ru.qm в папке с программой
    if (!tr.load("qtbase_ru.qm", QApplication::applicationDirPath()))
        // Если не найден, то загрузить из установленной библиотеки Qt
        tr.load("qtbase_ru.qm", QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    a.installTranslator(&tr);

    SeaBattleWindow w;
    w.show();
    w.showStartMessage();
    return a.exec();
}

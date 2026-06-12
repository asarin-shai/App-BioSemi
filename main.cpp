#include <QApplication>
#include "mainwindow.h"
#include <QCommandLineParser>
#include <string>

static void set_if_provided(const QCommandLineParser &parser, const QString &opt, const char *env_key) {
    if (parser.isSet(opt))
        qputenv(env_key, parser.value(opt).toUtf8());
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QCommandLineParser parser;
    parser.setApplicationDescription("BioSemi LSL bridge");
    parser.addHelpOption();

    QCommandLineOption configOpt(QStringList() << "c" << "config", "Configuration file.", "file", "biosemi_config.cfg");
    parser.addOption(configOpt);
    parser.addOption({"cap-circumference", "Override cap circumference.", "value"});
    parser.addOption({"cap-knownschemes", "Override known cap schemes list (comma separated).", "value"});
    parser.addOption({"cap-defaultscheme", "Override default cap scheme.", "value"});
    parser.addOption({"locations-file", "Override channel locations file.", "file"});
    parser.addOption({"reference-channels", "Override reference channels list (comma separated).", "value"});
    parser.addOption({"channel-subset", "Override channel subset index.", "index"});
    parser.addOption({"resample", "Enable resampling."});
    parser.addOption({"no-resample", "Disable resampling."});
    parser.addOption({"autoconnect", "Automatically connect on startup."});

    parser.process(a);

    set_if_provided(parser, "cap-circumference", "BIOSEMI_CAP_CIRCUMFERENCE");
    set_if_provided(parser, "cap-knownschemes", "BIOSEMI_CAP_KNOWNSCHEMES");
    set_if_provided(parser, "cap-defaultscheme", "BIOSEMI_CAP_DEFAULTSCHEME");
    set_if_provided(parser, "locations-file", "BIOSEMI_LOCATIONS_FILENAME");
    set_if_provided(parser, "reference-channels", "BIOSEMI_REFERENCE_CHANNELS");
    set_if_provided(parser, "channel-subset", "BIOSEMI_RECORDING_CHANSUBSET");
    if (parser.isSet("resample")) qputenv("BIOSEMI_RECORDING_RESAMPLE", "1");
    if (parser.isSet("no-resample")) qputenv("BIOSEMI_RECORDING_RESAMPLE", "0");
    if (parser.isSet("autoconnect")) qputenv("BIOSEMI_AUTOCONNECT", "1");

    MainWindow w(nullptr, parser.value(configOpt).toStdString());
    w.show();

    return a.exec();
}

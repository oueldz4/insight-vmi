/*
 * shell.cpp
 *
 *  Created on: 01.04.2010
 *      Author: chrschn
 */

#include "shell.h"
#include <string.h>
#include <QtAlgorithms>
#include "compileunit.h"

Shell::Shell(const KernelSymbols& symbols)
    : _sym(symbols)
{
    // Register all commands
    _commands.insert("exit",
            Command(
                &Shell::cmdExit,
                "Exits the program",
                "This command exists the program."));

    _commands.insert("help",
            Command(
                &Shell::cmdHelp,
                "Displays some help for a command",
                "Without any arguments, this command displays a list of all "
                "commands. For more detailed information about a command, try "
                "\"help <command>\" for any command."));

    _commands.insert("list",
            Command(
                &Shell::cmdList,
                "Lists various types of read symbols",
                "This command lists various types of read symbols.\n"
                "  list sources      List the source files\n"
                "  list types        List the types\n"
                "  list variables    List the variables"));

}


Shell::~Shell()
{
}


int Shell::start()
{
    _stdin.open(stdin, QIODevice::ReadOnly);
    _stdout.open(stdout, QIODevice::WriteOnly);
    _out.setDevice(&_stdout);

    int ret = 0;
    QString line, cmd;
    QByteArray buf;
    QStringList words;

    while (ret == 0 && !_stdin.atEnd()) {
        _out << ">>> " << flush;

        buf = _stdin.readLine();
        line = QString::fromLocal8Bit(buf.constData(), buf.size()).trimmed();
        ret = exec(line);
    }

    return ret;
}


int Shell::exec(QString command)
{
    QStringList words = command.split(QRegExp("\\s+"), QString::SkipEmptyParts);
    if (words.isEmpty())
        return 0;

    int ret = 0;
    QString cmd = words[0].toLower();
    words.pop_front();

    if (_commands.contains(cmd)) {
        ShellCallback c =_commands[cmd].callback;
        ret = (this->*c)(words);
    }
    else {
        _out << "Command not recognized: " << cmd << endl;
    }

    return ret;
}


int Shell::cmdExit(QStringList)
{
    return 1;
}


int Shell::cmdHelp(QStringList args)
{
    // If no arguments, show a generic cmdHelp cmdList
    if (args.size() <= 0) {
        _out << "The following represents a complete list of valid commands:" << endl;

        QStringList cmds = _commands.keys();
        cmds.sort();

        for (int i = 0; i < cmds.size(); i++) {
            _out << "  " << qSetFieldWidth(12) << left << cmds[i]
                 << qSetFieldWidth(0) << _commands[cmds[i]].helpShort << endl;
        }
    }
    // Show long cmdHelp for given command
    else {
        QString cmd = args[0].toLower();
        if (_commands.contains(cmd)) {
            _out << "Command: " << cmd << endl
                 << "Description: " << _commands[cmd].helpLong << endl;
        }
    }

    return 0;
}


int getFieldWidth(quint32 maxVal, int base = 16)
{
    int w = 0;
    if (base == 16)
        do { w++; } while ( (maxVal >>= 4) );
    else
        do { w++; } while ( (maxVal /= base) );
    return w;
}

void Shell::hline(int width)
{
	const int bufLen = 256;
	char buf[bufLen] = { 0 };
	// Make sure we don't overrun the buffer
	if (width + 2 > bufLen)
		width = bufLen - 2;
	// Construct and terminate the string
	memset(buf, '-', width);
	buf[width + 2] = 0;

    _out << buf << endl;
}


int Shell::cmdList(QStringList args)
{
    // Show cmdHelp, of no argument is given
    if (args.isEmpty())
        return cmdHelp(QStringList("list"));

    QString s = args[0].toLower();

    if (s == "sources") {
        QList<int> keys = _sym.factory().sources().keys();
        qSort(keys);
        // Find out required field width (keys needs to be sorted for that)
        int w = getFieldWidth(keys.last());

        _out << qSetFieldWidth(w) << right << "ID" << qSetFieldWidth(0) << "  "
             << qSetFieldWidth(0) << "File" << endl;

        hline();

        for (int i = 0; i < keys.size(); i++) {
            CompileUnit* unit = _sym.factory().sources().value(keys[i]);
            _out << qSetFieldWidth(w) << right << hex << unit->id() << qSetFieldWidth(0) << "  "
                 << qSetFieldWidth(0) << unit->file() << endl;
        }

        hline();
        _out << "Total source files: " << keys.size() << endl;
    }
    else if (s == "types") {
        static BaseType::RealTypeRevMap tRevMap = BaseType::getRealTypeRevMap();
        const BaseTypeList& types = _sym.factory().types();
        CompileUnit* unit = 0;
        // Find out required field width (the types are sorted by ascending ID)
        int w = getFieldWidth(types.last()->id());

        _out << qSetFieldWidth(w)  << right << "ID" << qSetFieldWidth(0) << "  "
             << qSetFieldWidth(10) << left << "Type"
             << qSetFieldWidth(24) << "Name"
             << qSetFieldWidth(5)  << right << "Size" << qSetFieldWidth(0) << "  "
             << qSetFieldWidth(14) << left << "Source"
             << qSetFieldWidth(0)  << endl;

        hline();

		QString src;
        for (int i = 0; i < types.size(); i++) {
            BaseType* type = types[i];
            // Construct name and line of the source file
            if (type->srcFile() >= 0) {
            	if (!unit || unit->id() != type->srcFile())
            		unit = _sym.factory().sources().value(type->srcFile());
            	if (!unit)
            		src = QString("(unknown id: %1)").arg(type->srcFile());
            	else
            		src = QString("%1").arg(unit->file());
            }
            else
            	src = "--";

            _out << qSetFieldWidth(w)  << right << hex << type->id() << qSetFieldWidth(0) << "  "
                 << qSetFieldWidth(10) << left << tRevMap[type->type()]
                 << qSetFieldWidth(24) << (type->name().isEmpty() ? "(none)" : type->name())
                 << qSetFieldWidth(5) << right << type->size() << qSetFieldWidth(0) << "  "
                 << qSetFieldWidth(14) << left << src
                 << qSetFieldWidth(0) << endl;
        }

        hline();
        _out << "Total types: " << types.size() << endl;
    }
    else if (s == "variables") {
        _out << "Not implemented in " << __FILE__ << ":" << __LINE__ << endl;
    }

    return 0;
}


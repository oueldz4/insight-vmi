/*
 * programoptions.h
 *
 *  Created on: 01.06.2010
 *      Author: chrschn
 */

#ifndef PROGRAMOPTIONS_H_
#define PROGRAMOPTIONS_H_

#include <QStringList>


/// Actions that can be triggered from the command line
enum Action {
    acNone            = 0,
    acUsage           = (1 << 0),
    acParseSymbols    = (1 << 1),
    acLoadSymbols     = (1 << 2)
};

/// The expected next token when parsing the command line arguments
enum NextToken {
    ntOption,
    ntInFileName,
    ntMemFileName
};

/// Represents one command line option
struct Option {
    const char* shortOpt;   ///< the short option to trigger this argument
    const char *longOpt;    ///< the long option to trigger this argument
    const char *help;       ///< short help text describing this option
    Action action;          ///< the action that is triggered by this option
    NextToken nextToken;    ///< the expected next token that should follow this option
    int conflictOptions;    ///< options this option conflicts (bitwise or'ed)
};

/// Total number of command line options
extern const int OPTION_COUNT;

/// Available command line options
extern const struct Option options[];


/**
 * Holds the option of a program instance
 */
class ProgramOptions
{
public:
    /**
     * Constructor
     */
    ProgramOptions();

    /**
     * Parses command line options
     * @param args command line options given to the program
     * @return \c true if parsing succeeded, \c false in case of errors
     */
    bool parseCmdOptions(QStringList args);

    /**
     * Prints command line options help
     */
    static void cmdOptionsUsage();

    /**
     * @return name of the input file
     */
    QString inFileName() const;

    /**
     * Sets the input file name
     * @param inFileName new file name
     */
    void setInFileName(QString inFileName);

    /**
     * @return the list of memory dump files
     */
    QStringList memFileNames() const;

    /**
     * Sets the memory dump files
     * @param memFiles the new memory dump files
     */
    void setMemFileNames(const QStringList& memFiles);

    /**
     * @return the initial action to perform
     */
    Action action() const;

    /**
     * Sets the initial action to perform
     * @param action new initial action
     */
    void setAction(Action action);

    /**
     * @return integer of bitwise or'ed options that were passed at the command
     * line
     */
    int activeOptions() const;

private:
    QString _inFileName;
    QStringList _memFileNames;
    Action _action;
    int _activeOptions;
};

/// Global options instance
extern ProgramOptions programOptions;

#endif /* PROGRAMOPTIONS_H_ */
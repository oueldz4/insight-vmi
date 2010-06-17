/*
 * kernelsymbolreader.h
 *
 *  Created on: 28.05.2010
 *      Author: chrschn
 */

#ifndef KERNELSYMBOLREADER_H_
#define KERNELSYMBOLREADER_H_

#include "longoperation.h"

// forward declarations
class QIODevice;
class SymFactory;

/**
 * This class reads kernel symbols in a self-defined, compact format to a file
 * or any other QIODevice.
 */
class KernelSymbolReader: public LongOperation
{
public:
    /**
     * Constructor
     * @param from source to read the previsouly saved debugging symbols from
     * @param factory the SymFactory to use for symbol creation
     */
    KernelSymbolReader(QIODevice* from, SymFactory* factory);

    /**
     * Starts the reading process
     * @exception ReaderWriterException error in data format
     * @exception GenericException out of memory
     */
    void read();

protected:
    /**
     * Displays progress information
     */
    virtual void operationProgress();

private:
    QIODevice* _from;
    SymFactory* _factory;
};

#endif /* KERNELSYMBOLREADER_H_ */
/*
 * typedef.h
 *
 *  Created on: 05.04.2010
 *      Author: chrschn
 */

#ifndef TYPEDEF_H_
#define TYPEDEF_H_

#include "refbasetype.h"

class Typedef: public RefBaseType
{
public:
    /**
     * Constructor
     * @param symbols the kernel symbols this symbol belongs to
     */
    Typedef(KernelSymbols* symbols);

    /**
     * Constructor
     * @param symbols the kernel symbols this symbol belongs to
     * @param info the type information to construct this type from
     */
    Typedef(KernelSymbols* symbols, const TypeInfo& info);

	/**
	 @return the actual type of that polimorphic variable
	 */
	virtual RealType type() const;
};


#endif /* TYPEDEF_H_ */

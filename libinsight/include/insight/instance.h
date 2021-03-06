/*
 * instance.h
 *
 *  Created on: 02.07.2010
 *      Author: chrschn
 */

#ifndef INSTANCE_H_
#define INSTANCE_H_

#include <QtAlgorithms>
#include "instancedata.h"
#include "basetype.h"
#include "virtualmemory.h"
#include "virtualmemoryexception.h"
#include "instance_def.h"


inline Instance::Instance(const Instance& other)
    : _d(other._d)
{
}


#ifdef Q_COMPILER_RVALUE_REFS
inline Instance::Instance(Instance &&other)
{
    qSwap(_d, other._d);
}
#endif


inline Instance& Instance::operator=(const Instance& other)
{
    _d = other._d;
    return *this;
}


#ifdef Q_COMPILER_RVALUE_REFS
inline Instance& Instance::operator=(Instance&& other)
{
    qSwap(_d, other._d);
    return *this;
}
#endif


inline int Instance::id() const
{
    return _d->id;
}


inline Instance::Origin Instance::origin() const
{
    return (Origin)_d->origin;
}


inline void Instance::setOrigin(Instance::Origin orig)
{
    _d->origin = orig;
}


inline quint64 Instance::address() const
{
    return _d->address;
}


inline quint64 Instance::endAddress() const
{
    if (size() > 0) {
        if (_d->vmem->memSpecs().vaddrSpaceEnd() - size() <= _d->address)
            return _d->vmem->memSpecs().vaddrSpaceEnd();
        else
            return _d->address + size() - 1;
    }
    return _d->address;
}


inline void Instance::setAddress(quint64 addr)
{
    _d->address = addr;
    if (_d->vmem && (_d->vmem->memSpecs().arch & MemSpecs::ar_i386))
        _d->address &= 0xFFFFFFFFUL;
}


inline void Instance::addToAddress(quint64 offset)
{
    _d->address += offset;
    if (_d->vmem && (_d->vmem->memSpecs().arch & MemSpecs::ar_i386))
        _d->address &= 0xFFFFFFFFUL;
}


inline int Instance::bitSize() const
{
    return _d->bitSize;
}


inline void Instance::setBitSize(qint8 size)
{
    _d->bitSize = size;
}


inline int Instance::bitOffset() const
{
    return _d->bitOffset;
}


inline void Instance::setBitOffset(qint8 offset)
{
    _d->bitOffset = offset;
}


inline QString Instance::name() const
{
    return _d->name;
}


inline void Instance::setName(const QString& name)
{
    _d->name = name;
}


inline const BaseType* Instance::type() const
{
    return _d->type;
}


inline void Instance::setType(const BaseType* type)
{
    _d->type = type;
}


inline QString Instance::typeName() const
{
    return _d->type ? _d->type->prettyName() : QString("void");
}


inline uint Instance::typeHash() const
{
    return _d->type ? _d->type->hash() : 0;
}


inline quint32 Instance::size() const
{
    return _d->type ? _d->type->size() : 0;
}


inline bool Instance::isNull() const
{
    return _d->address == 0;
}


inline bool Instance::isValid() const
{
    return _d->type != 0;
}


inline bool Instance::isAccessible() const
{
    return !isNull() && _d->vmem->safeSeek(_d->address);
}


inline int Instance::sizeofPointer() const
{
    return _d->vmem ? _d->vmem->memSpecs().sizeofPointer : sizeof(void*);
}


inline int Instance::sizeofLong() const
{
    return _d->vmem ? _d->vmem->memSpecs().sizeofLong : sizeof(long);
}


inline void Instance::setProperties(const StringHash& properties)
{
    _d->properties = properties;
    _d->checkForProperties = !properties.isEmpty();
}


inline bool Instance::isList() const
{
    return _d->listNext.data() != 0;
}


inline Instance Instance::listNext() const
{
    return Instance(_d->listNext);
}


inline qint8 Instance::toInt8() const
{
    return isNull() || !isValid() ? 0 : _d->type->toInt8(_d->vmem, _d->address);
}


inline quint8 Instance::toUInt8() const
{
    return isNull() || !isValid() ? 0 : _d->type->toUInt8(_d->vmem, _d->address);
}


inline qint16 Instance::toInt16() const
{
    return isNull() || !isValid() ? 0 : _d->type->toInt16(_d->vmem, _d->address);
}


inline quint16 Instance::toUInt16() const
{
    return isNull() || !isValid() ? 0 : _d->type->toUInt16(_d->vmem, _d->address);
}


inline qint32 Instance::toInt32() const
{
    return isNull() || !isValid() ? 0 : _d->type->toInt32(_d->vmem, _d->address);
}


inline quint32 Instance::toUInt32() const
{
    return isNull() || !isValid() ? 0 : _d->type->toUInt32(_d->vmem, _d->address);
}


inline qint64 Instance::toInt64() const
{
    return isNull() || !isValid() ? 0 : _d->type->toInt64(_d->vmem, _d->address);
}


inline quint64 Instance::toUInt64() const
{
    return isNull() || !isValid() ? 0 : _d->type->toUInt64(_d->vmem, _d->address);
}


inline qint64 Instance::toLong() const
{
    if (isNull() || !isValid())
        return 0;
    return sizeofLong() == 4 ?
                (qint64) _d->type->toInt32(_d->vmem, _d->address) :
                (qint64) _d->type->toInt64(_d->vmem, _d->address);
}


inline quint64 Instance::toULong() const
{
    if (isNull() || !isValid())
        return 0;
    return sizeofLong() == 4 ?
                (qint64) _d->type->toUInt32(_d->vmem, _d->address) :
                (qint64) _d->type->toUInt64(_d->vmem, _d->address);
}


inline quint64 Instance::toUIntBitField() const
{
    return (quint64) toIntBitField();
}


inline qint64 Instance::toNumber() const
{
    if (isNull() || !isValid() || !(_d->type->type() & IntegerTypes))
        return 0;

    if (bitSize() >= 0)
        return toIntBitField();

    switch (size()) {
    case 8:  return toInt64();
    default: return toInt32(); // make integer the default
    case 2:  return toInt16();
    case 1:  return toInt8();
    }
}


inline quint64 Instance::toUnsignedNumber() const
{
    if (isNull() || !isValid() || !(_d->type->type() & IntegerTypes))
        return 0;

    if (bitSize() >= 0)
        return toIntBitField();

    switch (size()) {
    case 8:  return toUInt64();
    default: return toUInt32(); // make integer the default
    case 2:  return toUInt16();
    case 1:  return toUInt8();
    }
}


inline float Instance::toFloat() const
{
    return isNull() || !isValid() ? 0 : _d->type->toFloat(_d->vmem, _d->address);
}


inline double Instance::toDouble() const
{
    return isNull() || !isValid() ? 0 : _d->type->toDouble(_d->vmem, _d->address);
}


inline void* Instance::toPointer() const
{
    return isNull() || !isValid() ? (void*)0
                                  : _d->type->toPointer(_d->vmem, _d->address);
}


inline QString Instance::toString(const ColorPalette* col) const
{
    return toStringPrivate(col, true);
}

//inline QString Instance::derefUserLand(const QString &pgd) const
//{
//	//TODO
//	//diekmann
//	QString ret;
//	if (isNull()) {
//    	ret = "NULL";
//    }
//    else {
//    	bool ok;

//    	qint64 pgd_d = pgd.toULongLong(&ok, 16);
//        if (!ok)
//            virtualMemoryError("(PDG invalid)");

//        _d->vmem->setUserLand(pgd_d);
//        try {
//            ret = _d->type->toString(_d->vmem, _d->address);
//        } catch(...) {
//            _d->vmem->setKernelSpace();
//    		throw;
//    	}
//        _d->vmem->setKernelSpace();
//    }
//    return ret;
//}


inline VirtualMemory* Instance::vmem() const
{
    return _d->vmem;
}



template<class T>
inline QVariant Instance::toVariant() const
{
    return isNull() || !isValid() ? QVariant() : _d->type->toVariant<T>(_d->vmem, _d->address);
}

#endif /* INSTANCE_H_ */

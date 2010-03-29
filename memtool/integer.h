#ifndef INTEGER_H
#define INTEGER_H

#include "basetype.h"

template<class T>
class GenericInteger : public BaseType
{
public:
    /**
      Constructor
      @see BaseType::BaseType()
     */
    GenericInteger(const QString& name, const quint32 id, const quint32 size,
            QIODevice *memory);

    /**
      @return the native value of this type
     */
    T toInt(size_t offset) const;

    /**
      @return a string representation of this type
     */
    virtual QString toString(size_t offset) const;

    /**
      @return the value of this type as a variant
     */
    virtual QVariant value(size_t offset) const;
};

//------------------------------------------------------------------------------
class Integer : public GenericInteger<int>
{
public:
    /**
      Constructor
      @see BaseType::BaseType()
     */
    Integer(const quint32 id, const quint32 size, QIODevice *memory);

    /**
      @return the actual type of that polimorphic instance
     */
    virtual RealType type() const;
};

//------------------------------------------------------------------------------
class UnsignedInteger : public GenericInteger<unsigned int>
{
public:
    /**
      Constructor
      @see BaseType::BaseType()
     */
    UnsignedInteger(const quint32 id, const quint32 size, QIODevice *memory);

    /**
      @return the actual type of that polimorphic instance
     */
    virtual RealType type() const;
};

#endif // INTEGER_H

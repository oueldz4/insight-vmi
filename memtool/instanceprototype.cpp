/*
 * instanceprototype.cpp
 *
 *  Created on: 05.07.2010
 *      Author: chrschn
 */

#include "instanceprototype.h"
#include <QScriptEngine>
#include "basetype.h"
#include "debug.h"

Q_DECLARE_METATYPE(Instance*)

InstancePrototype::InstancePrototype(QObject *parent)
    : QObject(parent)
{
}


InstancePrototype::~InstancePrototype()
{
}


int InstancePrototype::Id() const
{
    Instance* inst;
    return (inst = thisInstance()) ? inst->id() : -1;
}


QString InstancePrototype::Address() const
{
	Instance* inst;
    return (inst = thisInstance()) ?
            QString("%1").arg(inst->address(), inst->pointerSize() << 1, 16, QChar('0')) :
            QString("0");
}


quint32 InstancePrototype::AddressHigh() const
{
    Instance* inst;
    return (inst = thisInstance()) ? (inst->address() >> 32) : 0;
}


quint32 InstancePrototype::AddressLow() const
{
    Instance* inst;
    return (inst = thisInstance()) ? quint32(inst->address() & 0xFFFFFFFFUL) : 0;
}


bool InstancePrototype::IsNull() const
{
	Instance* inst;
    return (inst = thisInstance()) ? inst->isNull() : true;
}


bool InstancePrototype::IsAddressNull() const
{
    Instance* inst;
    return (inst = thisInstance()) ? (inst->address() == 0) : true;
}


QString InstancePrototype::Name() const
{
	Instance* inst;
    return (inst = thisInstance()) ? inst->name() : QString();
}


QString InstancePrototype::ParentName() const
{
	Instance* inst;
    return (inst = thisInstance()) ? inst->parentName() : QString();
}


QString InstancePrototype::FullName() const
{
	Instance* inst;
    return (inst = thisInstance()) ? inst->fullName() : QString();
}


QStringList InstancePrototype::MemberNames() const
{
    Instance* inst;
    return (inst = thisInstance()) ? inst->memberNames() : QStringList();
}


InstanceList InstancePrototype::Members() const
{
	Instance* inst;
    return (inst = thisInstance()) ? inst->members() : InstanceList();
}


//const BaseType* InstancePrototype::type() const
//{
//	Instance* inst;
//    return (inst = thisInstance()) ? inst->type() : 0;
//}


QString InstancePrototype::TypeName() const
{
	Instance* inst;
    return (inst = thisInstance()) ? inst->typeName() : QString();
}


QString InstancePrototype::Type() const
{
    // Init static member
    static BaseType::RealTypeRevMap realTypeNames =
            BaseType::getRealTypeRevMap();

    Instance* inst;
    return ( (inst = thisInstance()) && inst->type() ) ?
            realTypeNames[inst->type()->type()] : QString("unknown");
}


quint32 InstancePrototype::Size() const
{
	Instance* inst;
    return (inst = thisInstance()) ? inst->size() : 0;
}


bool InstancePrototype::MemberExists(const QString& name) const
{
	Instance* inst;
    return (inst = thisInstance()) ? inst->memberExists(name) : false;
}


Instance InstancePrototype::FindMember(const QString& name) const
{
	Instance* inst;
    return (inst = thisInstance()) ? inst->findMember(name) : Instance();
}


int InstancePrototype::TypeIdOfMember(const QString& name) const
{
	Instance* inst;
    return (inst = thisInstance()) ? inst->typeIdOfMember(name) : -1;
}


int InstancePrototype::PointerSize() const
{
    Instance* inst;
    return (inst = thisInstance()) ? inst->pointerSize() : 8;
}


qint8 InstancePrototype::toInt8() const
{
    Instance* inst;
    return (inst = thisInstance()) ? inst->toInt8() : 0;
}


quint8 InstancePrototype::toUInt8() const
{
    Instance* inst;
    return (inst = thisInstance()) ? inst->toUInt8() : 0;
}


qint16 InstancePrototype::toInt16() const
{
    Instance* inst;
    return (inst = thisInstance()) ? inst->toInt16() : 0;
}


quint16 InstancePrototype::toUInt16() const
{
    Instance* inst;
    return (inst = thisInstance()) ? inst->toUInt16() : 0;
}


qint32 InstancePrototype::toInt32() const
{
    Instance* inst;
    return (inst = thisInstance()) ? inst->toInt32() : 0;
}


quint32 InstancePrototype::toUInt32() const
{
    Instance* inst;
    return (inst = thisInstance()) ? inst->toUInt32() : 0;
}


QString InstancePrototype::toInt64(int base) const
{
    Instance* inst;
    return (inst = thisInstance()) ?
            QString::number(inst->toInt64(), base) : QString("0");
}


QString InstancePrototype::toUInt64(int base) const
{
    Instance* inst;
    return (inst = thisInstance()) ?
            QString::number(inst->toUInt64(), base) : QString("0");
}


quint32 InstancePrototype::toUInt64High() const
{
    Instance* inst;
    return (inst = thisInstance()) ? (inst->toUInt64() >> 32) : 0;
}


quint32 InstancePrototype::toUInt64Low() const
{
    Instance* inst;
    return (inst = thisInstance()) ? quint32(inst->toUInt64() & 0xFFFFFFFFUL) : 0;
}


float InstancePrototype::toFloat() const
{
    Instance* inst;
    return (inst = thisInstance()) ? inst->toFloat() : 0;
}


double InstancePrototype::toDouble() const
{
    Instance* inst;
    return (inst = thisInstance()) ? inst->toDouble() : 0;
}


QString InstancePrototype::toString() const
{
	Instance* inst;
    return (inst = thisInstance()) ? inst->toString() : QString();
}


Instance* InstancePrototype::thisInstance() const
{
	Instance* inst = qscriptvalue_cast<Instance*>(thisObject().data());
	if (!inst) {
		if (context())
            context()->throwError("Called an Instance member function on a "
                    "non-Instance object");
		else
			debugerr("Called an Instance member function on a non-Instance "
                    "object");
	}

    return inst;
}

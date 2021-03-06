/*
 * instance_def.h
 *
 *  Created on: 15.10.2010
 *      Author: chrschn
 */

#ifndef INSTANCE_DEF_H_
#define INSTANCE_DEF_H_

#include <QString>
#include <QStringList>
#include <QMetaType>
#include <QSharedDataPointer>

#include "instancedata.h"
#include "expressionresult.h"
#include "memberlist.h"
#include "embedresult.h"

class BaseType;
class VirtualMemory;
class Instance;
class Structured;
class ColorPalette;
class TypeRuleEngine;

/// A list of Instance objects
typedef QList<Instance> InstanceList;

/// Which knowledge sources to use when accessing members?
enum KnowledgeSources {
    ksAll = 0,                 ///< all sources
    ksNoAltTypes      = (1 << 0), ///< do not use alternative types
    ksNoRulesEngine   = (1 << 1), ///< do not use the rule engine
    ksNone = ksNoAltTypes|ksNoRulesEngine ///< use only the declared types
};


/**
 * This class wraps a variable instance in a memory dump.
 */
class Instance
{
public:
    /// How was this instance originally created?
    enum Origin {
        orUnknown = 0,   ///< unknown origin
        orVariable,      ///< from a Variable object
        orBaseType,      ///< from a BaseType object (manually)
        orMember,        ///< from a regular member access
        orCandidate,     ///< from a candidate type (AltRefType)
        orRuleEngine,    ///< from a scripted rule in the TypeRuleEngine
        orMemMapNode     ///< from a MemoryMapNode
    };

    /**
     * Returns a string representation of origin \a o.
     * @param o origin
     * @return string
     */
    static const char* originToString(Origin o);

    /**
     * Constructor
     */
    Instance();

    /**
     * Copy constructor
     * @param other constant reference
     */
    Instance(const Instance &other);

#ifdef Q_COMPILER_RVALUE_REFS
    /**
     * Move constructor
     * @param other
     */
    Instance (Instance&& other);
#endif

    /**
     * Constructor which sets the origin
     * @param orig origin of this instance
     */
    Instance(Origin orig);

    /**
     * Constructor
     * @param address the address of that instance
     * @param type the underlying BaseType of this instance
     * @param vmem the virtual memory device to read data from
     * @param id the ID of the variable this instance represents, if any
     */
    Instance(size_t address, const BaseType* type, VirtualMemory* vmem,
            int id = -1, Origin orig = orUnknown);

    /**
     * Constructor
     * @param address the address of that instance
     * @param type the underlying BaseType of this instance
     * @param name the name of this instance
     * @param parentNames the full name of the parent
     * @param vmem the virtual memory device to read data from
     * @param id the ID of the variable this instance represents, if any
     */
    Instance(size_t address, const BaseType* type, const QString& name,
            const QStringList& parentNames, VirtualMemory* vmem, int id = -1,
             Origin orig = orUnknown);

    /**
     * copy operator
     * @param other constant reference
     * @return this
     */
    Instance& operator=(const Instance& other);

#ifdef Q_COMPILER_RVALUE_REFS
    /**
     * Move operator
     * @param other rvalue
     * @return this
     */
    Instance& operator=(Instance&& other);
#endif

    /**
     * @return the ID of this instance, if it is a variable instance, -1
     * otherwise
     */
    int id() const;

    /**
     * Returns the origin of this instance.
     * @return see Origin
     * \sa setOrigin()
     */
    Origin origin() const;

    /**
     * Mark the origin of this instance as begin from \a orig. This value is
     * of pure informational purpose.
     * @param orig origin
     * \sa origin()
     */
    void setOrigin(Origin orig);

    /**
     * @return the array index of the memory dump this instance belongs to
     */
    int memDumpIndex() const;

    /**
     * @return the virtual address of the variable in memory
     * \sa setAddress()
     */
    quint64 address() const;

    /**
     * Sets the virtual addres of the variable in memory
     * @param addr the new virtual address
     * \sa address()
     */
    void setAddress(quint64 addr);

    /**
     * Adds \a offset to the virtual address.
     * @param offset the offset to add
     */
    void addToAddress(quint64 offset);

    /**
     * @return the virtual address of the last byte of this instance in memory
     */
    quint64 endAddress() const;

    /**
     * @return the bit size of this bit-field integer declaration
     */
    int bitSize() const;

    /**
     * Sets the bit size of this bit-field integer declaration.
     * @param size new bit size of bit-field integer declaration
     */
    void setBitSize(qint8 size);

    /**
     * @return the bit offset of this bit-field integer declaration
     */
    int bitOffset() const;

    /**
     * Sets the bit offset of this bit-field integer declaration.
     * @param offset new bit offset of bit-field integer declaration
     */
    void setBitOffset(qint8 offset);

    /**
     * This gives the (short) name of this Instance, i. e., its name its
     * parent's struct. For example, if you have accessed this Instance via
     * \c init_task.children.next, this will return \c next.
     * @return the name of this instance
     * \sa setName()
     */
    QString name() const;

    /**
     * Sets the name of this instance.
     * @param name new name for this instance
     * \sa name()
     */
    void setName(const QString& name);

    /**
     * This function returns the full name of the parent's struct, as it was
     * found. For example, if you have accessed this Instance via
     * \c init_task.children.next, this will return \c init_task.children .
     * @return the full name of the parent's struct
     * \sa parentNameComponents(), name(), fullName()
     */
    QString parentName() const;

    /**
     * This function returns all component names of the parent's struct, as it
     * was found, e. g. \c init_task, \c children
     * @return a list of the name components of the parent's struct
     */
    QStringList parentNameComponents() const;

    /**
     * Sets the component of all parent structs.
     * @param names list of names
     */
    void setParentNameComponents(const QStringList& names);

    /**
     * Use this function to retrieve the full name of this instance as it was
     * found following the names and members of structs in dotted notation,
     * for example, \c init_task.children.next.
     * @return the full name of this instance
     */
    QString fullName() const;

    /**
     * Use this function to retrieve all components of the full name of this
     * instance as it was found following the names and members of structs.
     * @return the full name components of this instance
     */
    QStringList fullNameComponents() const;

    /**
     * Returns the name of member no. \a index. Calling "inst.memberName(i)" is
     * much more efficient than calling "inst.member(i).mame()", since it does
     * not construct an intermediate Instance object.
     * @param index index into the member list
     * @return name of member \a index
     * \sa memberCount(), members(), memberNames()
     */
    const QString& memberName(int index) const;

    /**
     * Gives access to the names of all members if this instance.
     * @return the names of all direct members of this instance
     * \sa members(), memberExists()
     */
    const QStringList& memberNames() const;

    /**
     * Gives access to all members of this instance. If a member has exactly
     * one candidate type, this type will be used instead of the originally
     * declared type of the member. To have the declared types instead, set
     * \a declaredTypes to \c true.
     * @param src selects which sources of knowledge to use when accessing
     *  members
     * be used, where applicable
     * @return a list of instances of all members
     */
    InstanceList members(KnowledgeSources src = ksAll) const;

    /**
     * Gives access to the concrete BaseType of this instance.
     * @return the BaseType of this instance
     */
    const BaseType* type() const;

    /**
     * Changes the type of this instance to the provided type.
     * @param type the new type for this instance
     */
    void setType(const BaseType* type);

    /**
     * Convenience function to access type()->prettyName()
     * @return the name of this instance's type
     */
    QString typeName() const;

    uint typeHash() const;

    /**
     * Convenience function to access type()->size()
     * @return the size of this instance's type
     */
    quint32 size() const;

    /**
     * Checks if this instance has a non-null address.
     * @return \c true if this object is null, \c false otherwise
     * \sa isValid()
     */
    bool isNull() const;

    /**
     * Checks if this instance has a type.
     * @return \c true if this object has a type, \c false otherwise
     * \warning This does \e not check if the address is null or not!
     * \sa isNull()
     */
    bool isValid() const;

    /**
     * Checks if this instance is accessible, meaning that its value can be read
     * from the memory dump through its virtual address.
     * @return \c true if the instance is accessible, \c false otherwise
     */
    bool isAccessible() const;

    /**
     * Compares this Instance with \a other on a value basis. Two instances
     * must have the same BaseType as returned by type() to potentially be
     * equal. In addition, their following contents is compared to determine
     * their equality:
     *
     * \li NumericBaseType: the numeric value
     * \li Enum: the enumeration value
     * \li FuncPointer: the function's virtual address
     * \li Array: the array values, if the array length is known
     * \li untyped Pointer (void *): the virtual address pointed to
     * \li For any other RefBaseType (i.e., ConstType, Pointer, Typedef,
     *      VolatileType), the equality decision is delegated to the referenced
     *      type's equals() function.
     * \li Structured: all members of the above mention types are compared,
     *      but nested structs are ignored
     *
     * @param other the Instance object to compare this instance to
     * @return \c true if the two instances are considered to be equal, \c false
     * otherwise
     */
    bool equals(const Instance& other) const;

    /**
     * Compares this struct or union Instance with \a other using the equals()
     * function and returns a list of member names that are different.
     *
     * If this instance is not a struct or union or is not of the same type as
     * \a other, then a QStringList with just a single, empty string is
     * returned.
     *
     * If this instance equals \a other for all members, then an empty
     * QStringList is returned.
     *
     * @param other the Instance object to compare this instance to
     * @param recursive if \c true, this function recurses into nested structs
     *      or unions
     * @return If this Instance and \a other are structs or unions and
     * comparable, a list of member names that are different is returned.
     * Otherwise a list containing only one empty string is returned.
     */
    QStringList differences(const Instance& other, bool recursive) const;

    /**
     * Treats this Instance as an array and returns a new instance
     * at index \a index. The behavior depends on the type():
     *
     *  * For pointers of type <tt>T*</tt>, the pointer is dereferenced and
     *    \a index * <tt>sizeof(T)</tt> is added to the resulting address. The
     *    resulting type is <tt>T</tt>. If the pointer cannot be dereferenced,
     *    either because it is of type <tt>void*</tt> or the address is not
     *    accessible, an empty Instance is returned.
     *  * For arrays of type <tt>T[]</tt>, \a index * <tt>sizeof(T)</tt> is
     *    added to the current address. The resulting type is <tt>T</tt>.
     *  * For any other type <tt>T</tt>, only \a index * <tt>sizeof(T)</tt> is
     *    added to the current address. The type <tt>T</tt> remains unchanged.
     *
     * \warning For Array types, the length() parameter is never checked against
     * \a index!
     *
     * @param index array index to access
     * @return a new Instance as described above
     * \sa arrayLength()
     */
    Instance arrayElem(int index) const;

    /**
     * If this Instance represents an Array with a defined length, this length
     * os returned by this function, otherwise the return value is -1.
     * @return array length field as described above
     * \sa arrayElem()
     */
    int arrayLength() const;

    /**
     * Dereferences this instance as far as possible.
     * @param resolveTypes which types to automatically resolve, see
     * BaseType::TypeResolution
     * @param maxPtrDeref max. number of pointer dereferenciations, use -1 for
     * infinity
     * @param derefCount pointer to a counter variable for how many types have
     * been followed to create the instance
     * @return a dereferenced version of this instance
     * \sa BaseType::TypeResolution
     */
    Instance dereference(int resolveTypes, int maxPtrDeref = -1,
                         int* derefCount = 0) const;

    /**
     * @return the number of members, if this is a struct, \c 0 otherwise
     */
    int memberCount() const;

    /**
     * Access function to the members of this instance, if it is a struct. If
     * this instance is no struct/union or if the index is out of bounds, a
     * null Instance is returned.
     *
     * \note Make sure to check Instance::isNull() on the returned object to
     * see if it is valid or not.
     *
     * @param index index into the member list
     * @param resolveTypes which types to automatically resolve, see
     * BaseType::TypeResolution
     * @param maxPtrDeref the maximum levels of pointers that should be
     * dereferenced
     * @param src selects which sources of knowledge to use when accessing
     *  members
     * @param result provides the result of the knowledge source evaluation,
     * see TypeRuleEngine::MatchResult
     * @return Instance object of the specified member
     * \sa BaseType::TypeResolution KnowledgeSources
     */
    Instance member(int index, int resolveTypes = 0,
                    int maxPtrDeref = -1, KnowledgeSources src = ksAll,
                    int *result = 0) const;

    /**
     * Retrieves a member (i.e., struct components) of this Instance, if it
     * exists. If this struct or  union has no member \a name, all anonymous
     * nested structs or unions are searched as well. This is conforming to the
     * C standard.
     *
     * \note Make sure to check Instance::isNull() on the returned object to
     * see if it is valid or not.
     *
     * @param name the name of the member
     * @param resolveTypes which types to automatically resolve, see
     * BaseType::TypeResolution
     * @param maxPtrDeref the maximum levels of pointers that should be
     * dereferenced
     * @param src selects which sources of knowledge to use when accessing
     *  members
     * @param result provides the result of the knowledge source evaluation,
     * see TypeRuleEngine::MatchResult
     * @return Instance object of the specified member
     * \sa BaseType::TypeResolution, KnowledgeSources
     */
    Instance member(const QString& name, int resolveTypes = 0,
                    int maxPtrDeref = -1, KnowledgeSources src = ksAll,
                    int *result = 0) const;

    /**
     * Retrieves a member of this Instance based on a provided MemberList.
     * If this Instance is not a structure or union or if the member cannot
     * be resolved based on the provided list, a default Instance is returned.
     *
     * \note Make sure to check Instance::isNull() on the returned object to
     * see if it is valid or not.
     *
     * @param members the memberlist that defines the location of the member
     * one wants to retrieve
     * @param resolveTypes which types to automatically resolve, see
     * BaseType::TypeResolution
     * @param maxPtrDeref the maximum levels of pointers that should be
     * dereferenced
     * @param src selects which sources of knowledge to use when accessing
     *  members
     * @param result provides the result of the knowledge source evaluation,
     * see TypeRuleEngine::MatchResult
     * @return Instance object of the specified member
     * \sa BaseType::TypeResolution, KnowledgeSources
     */
    Instance member(const ConstMemberList& members, int resolveTypes,
                    int maxPtrDeref, KnowledgeSources src = ksAll,
                    int *result = 0) const;

    /**
     * Obtain the member of this instance that has the given offset provided that
     * this instance is a structure. If \a exactMatch is true the function will only
     * return a member if it can find a member within the struct that has the exact
     * offset \a offset. In case exactMatch is false, the function will return the
     * member that ecompasses the given offset \a offset.
     *
     * \note Make sure to check Instance::isNull() on the returned object to
     * see if it is valid or not.
     *
     * @param off The offset of the member that we are looking for
     * @param exactMatch should the function only return a member if it has the exact
     * offset \a offset.
     * @return Instance object of the member at offset off or a null Instance
     * \sa BaseType::TypeResolution
     */
     Instance memberByOffset(size_t off, bool exactMatch = true) const;

    /**
     * Gives access to the BaseType's of a member, if this is a struct or union.
     * If this is no struct or union or if \a index is out ouf bounds, null is
     * returned
     *
     * @param index index into the member list
     * @param resolveTypes which types to automatically resolve, see
     * BaseType::TypeResolution
     * @param maxPtrDeref the maximum levels of pointers that should be
     * dereferenced
     * @param src selects which sources of knowledge to use when accessing
     *  members
     * @param result provides the result of the knowledge source evaluation,
     * see TypeRuleEngine::MatchResult
     * @return pointer to the type of member \a index, or 0 if this is no struct
     * or union or \a index is out of bounds
     */
    const BaseType* memberType(int index, int resolveTypes = 0,
                               int maxPtrDeref = -1, KnowledgeSources src = ksAll,
                               int *result = 0) const;

    /**
     * Calculates the virtual address of a member, if this is a struct or union.
     * @param index index into the member list
     * @param resolveTypes which types to automatically resolve, see
     * BaseType::TypeResolution
     * @param maxPtrDeref the maximum levels of pointers that should be
     * dereferenced
     * @param src selects which sources of knowledge to use when accessing
     *  members
     * @param result provides the result of the knowledge source evaluation,
     * see TypeRuleEngine::MatchResult
     * @return the virtual address of member \a index, or 0 if this is no struct
     * or union
     */
    quint64 memberAddress(int index, int resolveTypes = 0, int maxPtrDeref = -1,
                          KnowledgeSources src = ksAll, int *result = 0) const;

    /**
     * Calculates the virtual address of a member, if this is a struct or union.
     * @param name the name of the member
     * @param resolveTypes which types to automatically resolve, see
     * BaseType::TypeResolution
     * @param maxPtrDeref the maximum levels of pointers that should be
     * dereferenced
     * @param src selects which sources of knowledge to use when accessing
     *  members
     * @param result provides the result of the knowledge source evaluation,
     * see TypeRuleEngine::MatchResult
     * @return the virtual address of member \a index, or 0 if this is no struct
     * or union
     */
    quint64 memberAddress(const QString& name, int resolveTypes = 0,
                          int maxPtrDeref = -1, KnowledgeSources src = ksAll,
                          int* result = 0) const;

    /**
     * Returns the offset of member \a name within a struct, if this is a struct
     * or union.
     * @param name the name of the member
     * @return offset of member \a name within the struct, or -1 if no such
     * member exists or if this instance is no struct or union
     * \sa memberAddress()
     */
    int memberOffset(const QString& name) const;

    /**
     * Checks if a member with the given name \a name exists in this instance.
     * @param name the name of the member to find
     * @return \c true if that member exists, \c false otherwise
     */
    bool memberExists(const QString& name) const;

    /**
     * Retrieves the index of the member with name \a name. This index can be
     * used as array index to both members() and memberNames().
     * @param name the name of the member to find
     * @return the index of that member, if found, or \c -1 otherwise.
     */
    int indexOfMember(const QString& name) const;

    /**
     * Retrieves the type ID of the member \a name.
     * @param name the name of the member
     * @param resolveTypes which types to automatically resolve, see
     * BaseType::TypeResolution
     * @param maxPtrDeref the maximum levels of pointers that should be
     * dereferenced
     * @param src selects which sources of knowledge to use when accessing
     *  members
     * @param result provides the result of the knowledge source evaluation,
     * see TypeRuleEngine::MatchResult
     * @return the ID of the type, if that member exists, \c 0 otherwise.
     */
    int typeIdOfMember(const QString& name, int resolveTypes = 0,
                       int maxPtrDeref = -1, KnowledgeSources src = ksAll,
                       int *result = 0) const;

    /**
     * Returns the number of candidate types for a particular member.
     * @param name the name of the member
     * @return 0 if only the originally declared type is available, otherwise
     * the number of alternative candidate types
     */
    int memberCandidatesCount(const QString& name) const;

    /**
     * Returns the number of candidate types for a particular member.
     * @param index index of the member
     * @return 0 if only the originally declared type is available, otherwise
     * the number of alternative candidate types
     */
    int memberCandidatesCount(int index) const;

    /**
     * Retrieves the candidate type no. \a cndtIndex for member with index
     * \a mbrIndex. You can check for the number of members with memberCount()
     * and the number of candidate types for a particular member with
     * memberCandidatesCount().
     * @param mbrIndex index of the member
     * @param cndtIndex index of the candidate type for that member
     * @return a new Instance object for the member with the selected candidate
     * type, if such a member and candiate exists, or an empty object otherwise
     */
    Instance memberCandidate(int mbrIndex, int cndtIndex) const;

    /**
     * Retrieves the candidate type no. \a cndtIndex for member \a name.
     * You can check for the number of candidate types for a particular member
     * with memberCandidatesCount().
     * @param name the name of the member
     * @param cndtIndex index of the candidate type for that member
     * @return a new Instance object if the member \a name with the selected
     * candidate type exists, or an empty object otherwise
     */
    Instance memberCandidate(const QString& name, int cndtIndex) const;

    /**
     * Checks if this Instance is compatible with the expression that needs to
     * be evaluated for candidate type with index \a cndtIndex of member at
     * index \a mbrIndex.
     * @param mbrIndex the member index
     * @param cndtIndex index of the candidate type for that member
     * @return \c true if this Instance is compatible with the expression,
     * \c false otherwise
     */
    bool memberCandidateCompatible(int mbrIndex, int cndtIndex) const;

    /**
     * Checks if this Instance is compatible with the expression that needs to
     * be evaluated for candidate type with index \a cndtIndex of member
     * \a name.
     * @param name the name of the member
     * @param cndtIndex index of the candidate type for that member
     * @return \c true if this Instance is compatible with the expression,
     * \c false otherwise
     */
    bool memberCandidateCompatible(const QString& name, int cndtIndex) const;

    /**
     * Retrieves the BaseType of candidate no. \a cndtIndex for member with index
     * \a mbrIndex. You can check for the number of members with memberCount()
     * and the number of candidate types for a particular member with
     * memberCandidatesCount().
     * @param mbrIndex index of the member
     * @param cndtIndex index of the candidate type for that member
     * @return a new Instance object for the member with the selected candidate
     * type, if such a member and candiate exists, or an empty object otherwise
     */
    const BaseType* memberCandidateType(int mbrIndex, int cndtIndex) const;

    /**
     * @return a string-indexed hash of properties that were set for this
     * instance within the scripting engine
     */
    const StringHash& properties() const { return _d->properties; }

    /**
     * Sets a string-indexed hash of properties that will be available as
     * additional proerties within the scripting engine.
     * @param properties hash of properties
     */
    void setProperties(const StringHash& properties);

    /**
     * Flag that indicates whether the QScriptValue wrapping this instance
     * should be checked for additional properties when converting it back
     * to an Instance object.
     * @return \c true if properties should be check, \c false otherwise
     * \sa setCheckForProperties()
     */
    bool checkForProperties() const { return _d->checkForProperties; }

    /**
     * Set flag that indicates whether the QScriptValue wrapping this instance
     * should be checked for additional properties when converting it back
     * to an Instance object.
     * @param valu
     * \sa checkForProperties()
     */
    void setCheckForProperties(bool value) { _d->checkForProperties = value; }

    /**
     * Returns \c true if this instance is part of a list of instances, i.e.,
     * if a call to listNext() retrieves a valid instance. Otherwise \c false
     * is returned. To retrieve the entire list, call toList() on the list's
     * head instance.
     *
     * Example usage:
     * \code
     * Instance inst = ...;
     * // use inst
     * ...
     * while (inst.isList()) {
     *     inst = inst.listNext();
     *     // use inst
     *    ...
     * }
     * \endcode
     *
     * @return \c true if this is a list, \c false otherwise
     * \sa toList(), listNext(), fromList()
     */
    bool isList() const;

    /**
     * If this instance is an element of a list, this function returns the next
     * instance in the list, otherwise an invalid instance is returned.
     * @return next element in list, if it exists, an invalid instance otherwise
     * \sa isList(), toList(), fromList()
     */
    Instance listNext() const;

    /**
     * Returns the list of instances (including this instance). If this instance
     * does not represent a list, this instance will be the only element in the
     * list.
     * @return list of instances
     * \sa isList(), fromList(), listNext()
     */
    InstanceList toList() const;

    /**
     * Explicit representation of this instance as qint8.
     * @return the value of this type as a qint8
     */
    qint8 toInt8() const;

    /**
     * Explicit representation of this instance as quint8.
     * @return the value of this type as a quint8
     */
    quint8 toUInt8() const;

    /**
     * Explicit representation of this instance as qint16.
     * @return the value of this type as a qint16
     */
    qint16 toInt16() const;

    /**
     * Explicit representation of this instance as quint16.
     * @return the value of this type as a quint16
     */
    quint16 toUInt16() const;

    /**
     * Explicit representation of this instance as qint32.
     * @return the value of this type as a qint32
     */
    qint32 toInt32() const;

    /**
     * Explicit representation of this instance as quint32.
     * @return the value of this type as a quint32
     */
    quint32 toUInt32() const;

    /**
     * Explicit representation of this instance as qint64.
     * @return the value of this type as a qint64
     */
    qint64 toInt64() const;

    /**
     * Explicit representation of this instance as quint64.
     * @return the value of this type as a quint64
     */
    quint64 toUInt64() const;

    /**
     * Explicit representation of this instance as <tt>long int</tt>. The value
     * will be read with the storage size of the guest system (32 or 64 bit),
     * but will be returned as 64 bit integer.
     * @return the value of this type as a <tt>long int</tt>
     * \sa sizeofLong()
     */
    qint64 toLong() const;

    /**
     * Explicit representation of this instance as <tt>unsigned long int</tt>.
     * The value will be read with the storage size of the guest system (32 or
     * 64 bit), but will be returned as 64 bit integer.
     * @return the value of this type as a <tt>unsigned long int</tt>
     * \sa sizeofLong()
     */
    quint64 toULong() const;

    /**
     * If this is instance represents a structured bit field, then this function
     * returns the corresponding integer value as signed integer.
     * @return integer bit field value
     * \sa toUIntBitField(), bitSize(), bitOffset()
     */
    qint64 toIntBitField() const;

    /**
     * If this is instance represents a structured bit field, then this function
     * returns the corresponding integer value as unsigned integer.
     * @return unsigned integer bit field value
     * \sa toIntBitField(), bitSize(), bitOffset()
     */
    quint64 toUIntBitField() const;

    /**
     * If this instance represents an integer type or a structured bit field,
     * this function returns the corresponding value as a signed integer.
     * @return numeric value of this instance
     * \sa toUnsignedNumber()
     */
    qint64 toNumber() const;

    /**
     * If this instance represents an integer type or a structured bit field,
     * this function returns the corresponding value as an unsigned integer.
     * @return numeric value of this instance
     * \sa toNumber()
     */
    quint64 toUnsignedNumber() const;

    /**
     * Explicit representation of this instance as float.
     * @return the value of this type as a float
     */
    float toFloat() const;

    /**
     * Explicit representation of this instance as double.
     * @return the value of this type as a double
     */
    double toDouble() const;

    /**
     * Explicit representation of this instance as a pointer.
     * @return the value of this type as a variant
     * @warning This function should only be called for a pointer type!
     * @warning The pointer will be read with the size of the guest, but the
     * return value has the pointer size of the host system! So a 32-bit guest
     * reading a pointer from a 64-bit host will lose information! Use
     * sizeofPointer() to retrieve the pointer size of the guest system.
     */
    void* toPointer() const;

    /**
     * Explicit representation of this instance as an ExpressionResult struct.
     * Depending on the BaseType of this instance, the result is converted into
     * a numeric representation. Lexical types are automatically dereferenced.
     * Pointer values will be converted to an unsigned 32 bit or 64 bit integer,
     * depending the memory specifications. Any other type will result in an
     * invalid value (erInvalid).
     * @param addrOp set to \c true if desired result is the expression "&inst",
     * otherwise set to \c false (the default)
     * @return a numeric representation of this instance
     * \sa pointerSize()
     */
    ExpressionResult toExpressionResult(bool addrOp = false) const;

    /**
     * Explicit representation of this instance as QVariant.
     * @return the value of this type as a variant
     */
    template<class T>
    QVariant toVariant() const;
//    {
//        return value<T>(mem, offset);
//    }

    /**
     * Returns a string representation of this instance.
     * @param col color palette to use for colorizing the output
     */
    QString toString(const ColorPalette *col = 0) const;

//    /**
//     * Returns a toString() representation of this instance using the page
//     * global directory (i. e., page table) specified as \a pgd for
//     * virtual-to-physical address translation. This allows to read an instance
//     * that is located in user-land address space.
//     *
//     * @param pgd the page global directory of the process this instance belongs
//     * to, most likely the content of the \c CR3 register, as a hex-encoded string
//     * @return the same as toString() but tries to access user-land memory,
//     * if possible, using the page table specified as \a pgd
//     */
//    QString derefUserLand(const QString &pgd) const;

    /**
     * @return the storage size of a pointer for the guest platform,
     * given in byte
     * \sa toPointer()
     */
    int sizeofPointer() const;

    /**
     * @return the storage size of a <tt>long int</tt> for the guest platform,
     * given in byte
     * \sa toLong(), toULong()
     */
    int sizeofLong() const;

    /**
      * Get the VirtualMemory object that is used by this instance
      */
    VirtualMemory* vmem() const;

    /**
     * Function to compare two Instances.
     * @param inst instance to compare to
     * @param embedded \c true if one instance is embedded inside the other
     * @param overlap \c true if instances overlap (not embedded)
     * @param thisParent \c true if \a this instance contains \a inst
     *                  (\c false if \a inst contains \a this instance)
     * @return \c true if instances do not conflict with each other
     */
    bool compareInstance(const Instance& inst,
        bool &embedded, bool &overlaps, bool &thisParent) const;

    /**
     * Function to compare two Instances, where one is a rtUnion.
     * Used in compareInstances
     * @param inst instance to compare to
     * @param thisParent \c true if \a this instance contains \a inst
     *                  (\c false if \a inst contains \a this instance)
     * @return \c true if instances do not conflict with each other
     */
    bool compareUnionInstances(const Instance& inst, bool &thisParent) const;

    /**
     * Function to compare two Instances Types.
     * Used in compareInstance.
     * @param inst instance to compare to
     * @return \c true if instances types do not conflict with each other
     */
    bool compareInstanceType(const Instance& inst) const ;

    /**
     * Function to check consistency by considering const / enum members.
     * This function is only useful for structured types.
     * Used in compareInstance.
     * @return \c true if instance is considered as consistent. Also
     *         \c true if instance is not a structured type.
     */
    bool isValidConcerningMagicNumbers(bool *constants = 0) const;

    /**
     * Returns \c true if this instance has a parent set, \c false otherwise.
     * \sa parent()
     */
    inline bool hasParent() const { return _d->parent.data() != 0; }

    /**
     * Returns the parent instance, if available.
     * \note Most instances do \b not have the parent available, in which case
     * this function returns an invalid Instance. Be sure to check hasParent()
     * before or isValid() afterwards!
     * \sa fromParent(), hasParent(), isValid()
     */
    Instance parent() const;

    /**
     * If this instance has its parent available, this method returns the
     * StructuredMember which lead from parent() to this instance.
     * @return member that was followed the parent to this instance, if
     * available, \c null otherwise
     * \sa hasParent(), parent()
     */
    inline const StructuredMember* fromParent() const { return _d->fromParent; }

    /**
     * Sets the parent for this instance. This is normally done
     * automatically and only required for Variable::toInstance().
     * @param parent the parent instance
     * @param fromParent the member to reach this instance from \a parent
     */
    void setParent(const Instance& parent, const StructuredMember* fromParent);

    /**
     * Returns \c true if the memory area occupied by this instance overlaps with
     * the memory area of \a other, otherwise returns \c false. No check about
     * the values of the members is performed. If you need a semantic overlap
     * check, see embeds().
     * @param other other instance
     */
    bool overlaps(const Instance& other) const;

    /**
     * Checks how this instance relates to another instance in memory and if
     * both possibly share the same memory.
     * @param other other instance
     * @return see ObjectRelation, where this is the first object and \a other
     * is the second object
     * \sa overlaps()
     */
    ObjectRelation embeds(const Instance &other) const;

    /**
     * Returns the global rule engine used by all instances.
     */
    inline static const TypeRuleEngine* ruleEngine() { return _ruleEngine; }

    /**
     * Sets the global rule engine used by all instances.
     * @param engine engine to use
     */
    inline static void setRuleEngine(const TypeRuleEngine* engine)
    {
        _ruleEngine = engine;
    }

    /**
     * Creates a single instance from a list of instances. Call isList() on that
     * instance to check if the instance represents a list, and toList() to
     * retrieve the list of instances.
     * @param list list of instances
     * @return a single instance that can be converted back to the list
     * \sa isList(), toList(), listNext()
     */
    static Instance fromList(const InstanceList& list);

private:
    typedef QSet<quint64> VisitedSet;

    inline Instance(QSharedDataPointer<InstanceData> data) : _d(data) {}

    void setListNext(const Instance& inst);


    Instance *typeRuleMatchRek(ConstMemberList &members, int *match) const;

    void differencesRek(const Instance& other, const QString& relParent,
            bool includeNestedStructs, QStringList& result,
            VisitedSet& visited) const;

    Instance memberCandidate(const StructuredMember* m, int cndtIndex) const;

	bool memberCandidateCompatible(const StructuredMember* m,
								   int cndtIndex) const;

	QString toStringPrivate(const ColorPalette *col, bool firstArrayElem) const;

	QSharedDataPointer<InstanceData> _d;

	static const TypeRuleEngine* _ruleEngine;
};

Q_DECLARE_METATYPE(Instance*)
Q_DECLARE_METATYPE(Instance)
Q_DECLARE_METATYPE(InstanceList)

#endif /* INSTANCE_DEF_H_ */

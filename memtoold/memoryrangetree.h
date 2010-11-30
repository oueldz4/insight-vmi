/*
 * memoryrangetree.h
 *
 *  Created on: 30.11.2010
 *      Author: chrschn
 */

#ifndef MEMORYRANGETREE_H_
#define MEMORYRANGETREE_H_

#include <QList>

struct MemoryRangeTreeNode;
class MemoryMapNode;

typedef QList<const MemoryMapNode*> ConstNodeList;

/**
 * This class holds MemoryMapNode objects in a search tree, ordered by their
 * start address, which allows fast queries of MemoryMapNode's and their
 * properties within a memory range.
 */
class MemoryRangeTree
{
public:
    typedef ConstNodeList NodeList;

    MemoryRangeTree(quint64 addrSpaceEnd);
    virtual ~MemoryRangeTree();

    bool isEmpty() const { return _root != 0; };
    int size() const;
    void clear();

    void insert(const MemoryMapNode* node);

    /**
     * Finds all objects in virtual memory that occupy space between
     * \a addrStart and \a addrEnd. Objects that only partly fall into that
     * range are included.     *
     * @param addrStart the virtual start address
     * @param addrEnd the virtual end address (including)
     * @return a list of MemoryMapNode objects
     */
    ConstNodeList objInRange(quint64 addrStart, quint64 addrEnd) const;

private:
    MemoryRangeTreeNode* _root;
    MemoryRangeTreeNode* _first;
    MemoryRangeTreeNode* _last;
    int _size;
    quint64 _addrSpaceEnd;
};

#endif /* MEMORYRANGETREE_H_ */

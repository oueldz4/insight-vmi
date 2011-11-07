/*
 * astwalker.h
 *
 *  Created on: 28.03.2011
 *      Author: chrschn
 */

#ifndef ASTWALKER_H_
#define ASTWALKER_H_

#include <astnode.h>
#include <QString>

enum ASTWalkerFlags {
    wfIsList      = (1<<0),
    wfFirstInList = (1<<1),
    wfLastInList  = (1<<2)
};

class AbstractSyntaxTree;

/**
 * This class implements a walker on an abstract syntax tree.
 */
class ASTWalker
{
public:
    ASTWalker(AbstractSyntaxTree* ast = 0);
    virtual ~ASTWalker();

    int walkTree();
    AbstractSyntaxTree* ast();
    const AbstractSyntaxTree* ast() const;

protected:
    int walkTree(pASTNodeList head);
    int walkTree(pASTNode node, int flags = 0);

    virtual void beforeChildren(pASTNode node, int flags);
    virtual void beforeChild(pASTNode node, pASTNode childNode);
    virtual void afterChild(pASTNode node, pASTNode childNode);
    virtual void afterChildren(pASTNode node, int flags);

    AbstractSyntaxTree* _ast;
    bool _stopWalking;
};


inline void ASTWalker::beforeChildren(pASTNode /*node*/, int /*flags*/) {}
inline void ASTWalker::beforeChild(pASTNode /*node*/, pASTNode /*childNode*/) {}
inline void ASTWalker::afterChild(pASTNode /*node*/, pASTNode /*childNode*/) {}
inline void ASTWalker::afterChildren(pASTNode /*node*/, int /*flags*/) {}
inline const AbstractSyntaxTree* ASTWalker::ast() const { return _ast; }
inline AbstractSyntaxTree* ASTWalker::ast() { return _ast; }

#endif /* ASTWALKER_H_ */

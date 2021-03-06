#ifndef ASTEXPRESSIONEVALUATOR_H
#define ASTEXPRESSIONEVALUATOR_H

//#include <astwalker.h>
#include <QHash>
#include <QList>
#include <QStack>
#include <realtypes.h>
#include <insight/astexpression.h>

class ASTType;
class ASTNode;
class ASTNodeList;
class ASTExpression;
class ASTBinaryExpression;
class ASTVariableExpression;
class ASTTypeEvaluator;
class SymFactory;
class AbstractSyntaxTree;

typedef QList<ASTExpression*> ASTExpressionList;
typedef QHash<const ASTNode*, ASTExpression*> ASTNodeExpressionHash;
typedef QHash<const ASTNode*, const ASTNode*> ASTNodeNodeHash;
typedef QStack<const ASTNode*> ASTNodeStack;

/**
  This class evaluates expressions within a syntax tree.
 */
class ASTExpressionEvaluator
{
public:
    ASTExpressionEvaluator(ASTTypeEvaluator* eval, SymFactory* factory);
    virtual ~ASTExpressionEvaluator();

    ASTExpression* exprOfNode(const ASTNode *node,
                              const ASTNodeNodeHash& ptsTo);
    static ExprResultSizes realTypeToResultSize(RealType type);

    /**
     * Clears the internal cache of expressions and forces a re-evaluation
     * of all AST nodes. This method is meant for debugging only.
     */
    void clearCache();

protected:
    /**
     * Converts a pANTLR3_COMMON_TOKEN to a QString.
     * @param tok the ANTLR3 token to convert to a QString
     * @return the token \a tok as a QString
     */
    QString antlrTokenToStr(const pANTLR3_COMMON_TOKEN tok) const;

    /**
     * Converts a pANTLR3_STRING to a QString.
     * @param s the ANTLR3 string to convert to a QString
     * @return the string \a s as a QString
     */
    QString antlrStringToStr(const pANTLR3_STRING s) const;

private:
    template<class T> T* createExprNode();
    template<class T, class PT> T* createExprNode(PT param);
    template<class T, class PT1, class PT2> T* createExprNode(PT1 param1,
                                                              PT2 param2);

    ASTExpression *exprOfAssignmentExpr(
            const ASTNode *node, const ASTNodeNodeHash& ptsTo);
    ASTExpression *exprOfBinaryExpr(
            const ASTNode *node, const ASTNodeNodeHash& ptsTo);
    ASTExpression *exprOfBuiltinFuncAlignOf(
            const ASTNode *node, const ASTNodeNodeHash& ptsTo);
    ASTExpression *exprOfBuiltinFuncChooseExpr(
            const ASTNode *node, const ASTNodeNodeHash& ptsTo);
    ASTExpression *exprOfBuiltinFuncConstant(
            const ASTNode *node, const ASTNodeNodeHash& ptsTo);
    ASTExpression *exprOfBuiltinFuncExpect(
            const ASTNode *node, const ASTNodeNodeHash& ptsTo);
    ASTExpression *exprOfBuiltinFuncObjectSize(
            const ASTNode *node, const ASTNodeNodeHash& ptsTo);
    ASTExpression *exprOfBuiltinFuncOffsetOf(
            const ASTNode *node, const ASTNodeNodeHash& ptsTo);
    ASTExpression* exprOfBuiltinFuncOffsetOfSingle(
            const ASTNode *node, const BaseType* bt, const ASTType* type,
            const ASTNodeNodeHash &ptsTo, bool exceptions);
    ASTExpression *exprOfBuiltinFuncSizeof(
            const ASTNode *node, const ASTNodeNodeHash& ptsTo);
    ASTExpression *exprOfBuiltinFuncTypesCompatible(
            const ASTNode *node, const ASTNodeNodeHash& ptsTo);
    ASTExpression *exprOfConditionalExpr(
            const ASTNode *node, const ASTNodeNodeHash& ptsTo);
    ASTExpression *exprOfConstant(
            const ASTNode *node, const ASTNodeNodeHash& ptsTo);
    ASTExpression *exprOfNodeList(
            const ASTNodeList *list, const ASTNodeNodeHash& ptsTo);
    ASTExpression *exprOfPostfixExpr(
            const ASTNode *node, const ASTNodeNodeHash& ptsTo);
    ASTExpression *exprOfPrimaryExpr(
            const ASTNode *node, const ASTNodeNodeHash& ptsTo);
    ASTExpression *exprOfUnaryExpr(
            const ASTNode *node, const ASTNodeNodeHash& ptsTo);

    unsigned int sizeofType(const ASTType *type);

    bool appendPostfixExpressionSuffixes(const ASTNodeList *suffixList,
                                         ASTVariableExpression *varExpr);

    ASTExpressionList _allExpressions;
    ASTNodeExpressionHash _expressions;
    AbstractSyntaxTree* _ast;
    ASTTypeEvaluator* _eval;
    SymFactory* _factory;
    ASTNodeStack _evalNodeStack;
};

#endif // ASTEXPRESSIONEVALUATOR_H

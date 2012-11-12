#include "altreftyperulewriter.h"
#include "ioexception.h"
#include "filenotfoundexception.h"
#include "typeruleparser.h"
#include "typefilter.h"
#include "osfilter.h"
#include "symfactory.h"
#include "structured.h"
#include "structuredmember.h"
#include "refbasetype.h"
#include "variable.h"
#include "astexpression.h"
#include <debug.h>
#include <QXmlStreamWriter>
#include <QDir>
#include <QFile>
#include <QDateTime>

int AltRefTypeRuleWriter::_indentation = -1;
const QString AltRefTypeRuleWriter::_srcVar("src");


int AltRefTypeRuleWriter::write(const QString& name, const QString& baseDir)
{
    _filesWritten.clear();
    const MemSpecs& specs = _factory->memSpecs();

    // Check if directories exist
    QDir dir(baseDir);
    QDir rulesDir(dir.absoluteFilePath(name));
    if (!dir.exists(name) && !dir.mkpath(name))
        ioError(QString("Error creating directory \"%1\".")
                    .arg(rulesDir.absolutePath()));

    QFile incFile(dir.absoluteFilePath(name + ".xml"));
    if (!incFile.open(QIODevice::WriteOnly|QIODevice::Truncate))
        ioError(QString("Error opening file \"%1\" for writing.")
                    .arg(incFile.fileName()));

    _filesWritten.append(incFile.fileName());
    QXmlStreamWriter writer(&incFile);
    writer.setAutoFormatting(true);
    writer.setAutoFormattingIndent(_indentation);

    // Begin document with a comment including guest OS details
    writer.writeStartDocument();
    writer.writeComment(QString("\nFile created on %1\n%2")
                           .arg(QDateTime::currentDateTime().toString())
                           .arg(specs.toString()));

    // Begin knowledge file
    writer.writeStartElement(xml::typeknowledge); // typeknowledge
    writer.writeAttribute(xml::version, QString::number(xml::currentVer));

    // Write OS version information
    writer.writeAttribute(xml::os, specs.version.sysname);
    if (specs.arch == MemSpecs::ar_i386)
        writer.writeAttribute(xml::architecture, xml::arX86);
    else if (specs.arch == MemSpecs::ar_i386_pae)
        writer.writeAttribute(xml::architecture, xml::arX86PAE);
    else if (specs.arch == MemSpecs::ar_x86_64)
        writer.writeAttribute(xml::architecture, xml::arAMD64);
    else
        typeRuleWriterError(QString("Unknown architecture: %1").arg(specs.arch));
    writer.writeAttribute(xml::minver, specs.version.release);
    writer.writeAttribute(xml::maxver, specs.version.release);

    // Rule includes
    writer.writeStartElement(xml::ruleincludes); // ruleincludes
    writer.writeTextElement(xml::ruleinclude, "./" + name);
    writer.writeEndElement(); // ruleincludes

    // Go through all variables and types and write the information
    QString fileName;
    foreach(const BaseType* type, _factory->types()) {
        if (type->type() & RefBaseTypes) {
            const RefBaseType* rbt = dynamic_cast<const RefBaseType*>(type);
            if (rbt->hasAltRefTypes()) {
                fileName = write(rbt, rulesDir);
                if (!fileName.isEmpty()) {
                    writer.writeTextElement(xml::include, fileName);
                    _filesWritten.append(rulesDir.absoluteFilePath(fileName));
                }
            }
        }
        else if (type->type() & StructOrUnion) {
            const Structured* s = dynamic_cast<const Structured*>(type);
            fileName = write(s, rulesDir);
            if (!fileName.isEmpty()) {
                writer.writeTextElement(xml::include, fileName);
                _filesWritten.append(rulesDir.absoluteFilePath(fileName));
            }
        }
    }

    foreach(const Variable* var, _factory->vars()) {
        if (var->hasAltRefTypes()) {
            fileName = write(var, rulesDir);
            if (!fileName.isEmpty()) {
                writer.writeTextElement(xml::include, fileName);
                _filesWritten.append(rulesDir.absoluteFilePath(fileName));
            }
        }
    }

    writer.writeEndElement(); // typeknowledge
    writer.writeEndDocument();

    return _filesWritten.size();
}


QString AltRefTypeRuleWriter::write(const RefBaseType *rbt, const QDir &rulesDir) const
{
    Q_UNUSED(rulesDir);
    debugmsg(QString("Alt. ref. type in RefBaseType '%1' (0x%2)")
                .arg(rbt->prettyName()).arg(rbt->id(), 0, 16));

    return QString();
}


QString AltRefTypeRuleWriter::uniqueFileName(const QDir &dir, QString fileName) const
{
    int i = 1;
    while(_filesWritten.contains(dir.absoluteFilePath(fileName))) {
        QFileInfo info(fileName);
        fileName = info.path() + "/" + info.baseName() + QString::number(i++) +
                "." + info.completeSuffix();
    }
    return fileName;
}


QString AltRefTypeRuleWriter::write(const Structured *s, const QDir &rulesDir) const
{
    QString fileName = rulesDir.relativeFilePath(
                uniqueFileName(rulesDir, fileNameFromType(s)));
    QFile outFile(rulesDir.absoluteFilePath(fileName));
    QXmlStreamWriter writer;

    ConstMemberList members;
    QStack<int> memberIndex;
    const Structured *cur = s;
    int i = 0, count = 0;
    while (i < cur->members().size()) {
        const StructuredMember *m = cur->members().at(i);
        members.append(m);
        if (m->hasAltRefTypes()) {
            // Prepare the XML file, if not yet done
            if (!outFile.isOpen()) {
                openXmlRuleFile(rulesDir.absoluteFilePath(fileName), outFile, writer,
                                QString("Type: %1\n" "Type ID: 0x%2\n")
                                    .arg(s->prettyName())
                                    .arg((uint)s->id(), 0, 16));
            }
            // Write the rule
            count += write(writer, m->altRefTypes(), QString(), s, members);
        }

        // Use members and memberIndex to recurse through nested structs
        const BaseType* mt = m->refTypeDeep(BaseType::trLexical);
        if (mt->type() & StructOrUnion) {
            memberIndex.push(i);
            cur = dynamic_cast<const Structured*>(mt);
            i = 0;
        }
        else {
            ++i;
            members.pop_back();

            while (i >= cur->members().size() && !members.isEmpty()) {
                i = memberIndex.pop() + 1;
                cur = members.last()->belongsTo();
                members.pop_back();
            }
        }

    }

    if (outFile.isOpen()) {
        writer.writeEndDocument();
        outFile.close();
        if (count > 0)
            return fileName;
        else
            outFile.remove();
    }
    return QString();
}


QString AltRefTypeRuleWriter::write(const Variable *var, const QDir &rulesDir) const
{
    QString fileName = rulesDir.relativeFilePath(
                uniqueFileName(rulesDir, fileNameFromVar(var)));
    QFile outFile;
    QXmlStreamWriter writer;
    openXmlRuleFile(rulesDir.absoluteFilePath(fileName), outFile, writer,
                    QString("Variable: %1\n" "Variable ID: 0x%2\n")
                    .arg(var->prettyName())
                    .arg((uint)var->id(), 0, 16));

    int count = write(writer, var->altRefTypes(), var->name(), var->refType(),
                      ConstMemberList());
    writer.writeEndDocument();
    outFile.close();

    if (count > 0)
        return fileName;
    else
        outFile.remove();
    return QString();
}



int AltRefTypeRuleWriter::write(QXmlStreamWriter &writer,
                                const AltRefTypeList& altRefTypes,
                                const QString& varName,
                                const BaseType* srcType,
                                const ConstMemberList& members) const
{
    ASTConstExpressionList tmpExp;

    QStringList memberNames;
    foreach (const StructuredMember *member, members)
        memberNames += member->name();

    int count = 0;

    try {
        foreach(const AltRefType& art, altRefTypes) {
            // We require a valid expression
            if (!art.expr())
                continue;

            // Find non-pointer source type
            const BaseType* srcTypeNonPtr = srcType->dereferencedBaseType();

            // Find the target base type
            const BaseType* target = _factory->findBaseTypeById(art.id());
            if (!target)
                typeRuleWriterError(QString("Cannot find base type with ID 0x%1.")
                                    .arg((uint)art.id(), 0, 16));

            // Flaten the expression tree of alternatives
            ASTConstExpressionList alternatives = art.expr()->expandAlternatives(tmpExp);

            foreach(const ASTExpression* expr, alternatives) {
                bool skip = false;
                // Find all variable expressions
                ASTConstExpressionList varExpList = expr->findExpressions(etVariable);
                const ASTVariableExpression* varExp = 0;
                for (int i = 0; !skip && i < varExpList.size(); ++i) {
                    const ASTVariableExpression* ve =
                            dynamic_cast<const ASTVariableExpression*>(varExpList[i]);
                    if (ve) {
                        const BaseType* veTypeNonPtr = ve->baseType() ?
                                    ve->baseType()->dereferencedBaseType() : 0;
                        // If we don't have a base type or the variable's type
                        // does not match the given source type, we don't need
                        // to proceed
                        if (!veTypeNonPtr || srcTypeNonPtr->id() != veTypeNonPtr->id()) {
                            writer.writeComment(
                                        QString("Source type in expression is "
                                                "'%1' does not match base type "
                                                "'%2' of candidate %3.%4.")
                                                .arg(expr->toString())
                                                .arg(srcType->prettyName())
                                                .arg(srcType->name())
                                                .arg(memberNames.join("."))
                                                .replace("--", "- - ")); // avoid "--" in comments
                            skip = true;
                            break;
                        }
                        // Use the first one as the triggering variable expression
                        if (!varExp)
                            varExp = ve;
                        else {
                            // We expect exactly one type of variable expression
                            if (varExp->baseType()->id() != ve->baseType()->id()) {
                                typeRuleWriterError(QString("Expression %1 for %2.%3 has %4 different variables.")
                                                    .arg(expr->toString())
                                                    .arg(srcType->name())
                                                    .arg(memberNames.join("."))
                                                    .arg(varExpList.size()));
                                skip = true;
                                break;
                            }
                        }
                    }
                }

                // Make sure we found a valid variable expression
                if (!varExp || skip)
                    continue;

                // We expect that the members in the array match the ones
                // within the transformations
                int j = 0;
                for (int i = 0; !skip && i < memberNames.size(); ++i) {
                    // Skip anonymous member
                    if (memberNames[i].isEmpty())
                        continue;
                    // Forward to the first member
                    while (!skip && j < varExp->transformations().size() &&
                           varExp->transformations().at(j).type != ttMember)
                        ++j;
                    if (j >= varExp->transformations().size() ||
                        memberNames[i] != varExp->transformations().at(j).member)
                    {
                        skip = true;
                        writer.writeComment(QString("Member names in expression"
                                                    " %1 do not match members "
                                                    "in %2.%3.")
                                            .arg(expr->toString())
                                            .arg(srcType->name())
                                            .arg(memberNames.join("."))
                                            .replace("--", "- - ")); // avoid "--" in comments
                    }
                    else
                        ++j;
                }

                if (skip)
                    continue;

                QString exprStr = expr->toString(true);
                exprStr.replace("(" + varExp->baseType()->prettyName() + ")", _srcVar);

                QString ruleName(varName.isEmpty() ? srcType->name() : varName);
                if (!memberNames.isEmpty())
                    ruleName += "." + memberNames.join(".");

                writer.writeStartElement(xml::rule); // rule

                writer.writeTextElement(xml::name, ruleName);
                writer.writeTextElement(xml::description, expr->toString() +
                                        " => (" + target->prettyName() + ")");

                writer.writeStartElement(xml::filter); // filter
                if (!varName.isEmpty())
                    writer.writeTextElement(xml::variablename, varName);
                writer.writeTextElement(xml::datatype, realTypeToStr(srcType->type()));
                writer.writeTextElement(xml::type_name, srcType->name());

                if (varExp->transformations().memberCount() > 0) {
                    writer.writeStartElement(xml::members); // members
                    // Add a filter rule for each member
                    foreach (const SymbolTransformation& trans,
                             varExp->transformations())
                    {
                        if (trans.type == ttMember)
                            writer.writeTextElement(xml::member, trans.member);
                    }
                    writer.writeEndElement(); // members
                }
                writer.writeEndElement(); // filter

                writer.writeStartElement(xml::action); // action
                writer.writeAttribute(xml::type, xml::expression);
                writer.writeTextElement(xml::srcType, varExp->baseType()->prettyName(_srcVar));
                writer.writeTextElement(xml::targetType, target->prettyName());
                writer.writeTextElement(xml::expression, exprStr);
                writer.writeEndElement(); // action

                writer.writeEndElement(); // rule

                ++count;
            }
        }
    }
    catch (...) {
        // exceptional cleanup
        foreach(const ASTExpression* expr, tmpExp)
            delete expr;
        throw;
    }

    // regular cleanup
    foreach(const ASTExpression* expr, tmpExp)
        delete expr;

    return count;
}


void AltRefTypeRuleWriter::openXmlRuleFile(const QString& fileName,
                                           QFile& outFile,
                                           QXmlStreamWriter& writer,
                                           const QString& comment) const
{
    outFile.setFileName(fileName);
    if (_filesWritten.contains(fileName))
        debugmsg("Overwriting file " << outFile.fileName());
    if (!outFile.open(QIODevice::WriteOnly|QIODevice::Truncate))
        ioError(QString("Error opening file \"%1\" for writing.")
                    .arg(outFile.fileName()));

    writer.setDevice(&outFile);
    writer.setAutoFormatting(true);
    writer.setAutoFormattingIndent(_indentation);

    writer.writeStartDocument();
    // Begin document with a comment
    QString c(comment);
    writer.writeComment(QString("\nFile created on %1\n%2")
                            .arg(QDateTime::currentDateTime().toString())
                            .arg(c.replace("--", "- - ")));

    writer.writeStartElement(xml::typeknowledge); // typeknowledge
    writer.writeAttribute(xml::version, QString::number(xml::currentVer));
    writer.writeStartElement(xml::rules); // rules
}


QString AltRefTypeRuleWriter::fileNameFromType(const BaseType *type) const
{
    QString s = QString("type_%1.xml").arg(fileNameEscape(type->prettyName()));
    if (type->name().isEmpty())
        s = s.replace(str::anonymous,
                      QString("0x%1").arg((uint)type->id(), 0, 16));
    return s;
}


QString AltRefTypeRuleWriter::fileNameFromVar(const Variable *var) const
{
    QString n(var->name());
    if (n.isEmpty())
        n = QString("0x%1").arg((uint)var->id(), 0, 16);
    QString s = QString("var_%1_%2.xml")
                    .arg(n)
                    .arg(fileNameEscape(var->refType()->prettyName()));
    if (var->refType()->name().isEmpty())
        s = s.replace(str::anonymous,
                      QString("0x%1").arg((uint)var->refTypeId(), 0, 16));
    return s;
}


QString AltRefTypeRuleWriter::fileNameEscape(QString s) const
{
    s = s.trimmed();
    s = s.replace(QRegExp("\\[([^\\]]*)\\]"), "_array\\1");
    s = s.replace(QChar('?'), "N");
    s = s.replace(QChar(' '), "_");
    s = s.replace(QChar('*'), "_ptr");
    return s;
}
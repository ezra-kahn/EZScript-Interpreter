//
// Created by Ezra Kahn on 5/11/26.
//

#include "Evaluator.h"

Evaluator::Evaluator(ASTNode* codeRoot)
{
    this->codeRoot = codeRoot;
}

ValueStore Evaluator::evaluateUnary(ASTNode* node)
{
    ValueStore valueStore = evaluateNode(node -> firstChild);
    switch (node -> opType)
    {
    case TILDE:
        valueStore.value.integer = ~valueStore.value.integer;
    }
    return valueStore;
}

ValueStore Evaluator::evaluateBinary(ASTNode* node)
{
    ValueStore valueA = evaluateNode(node -> firstChild);
    ValueStore valueB = evaluateNode(node -> firstChild -> nextSibling);
    switch (node -> opType)
    {
    case PLUS:
        return {valueA.valueType, valueA.value.integer + valueB.value.integer};
    case MINUS:
        return {valueA.valueType, valueA.value.integer - valueB.value.integer};
    case STAR:
        return {valueA.valueType, valueA.value.integer * valueB.value.integer};
    case FRONT_SLASH:
        return {valueA.valueType, valueA.value.integer / valueB.value.integer};
    default:
        abort(); // We shouldn't be here
    }
}


ValueStore Evaluator::evaluateNode(ASTNode* node)
{
    switch (node -> opClass)
    {
    case STATEMENT_GROUP:
        evaluateChildren(node);
        break;
    case PRINT_OP:
        Parser::printVal(evaluateNode(node -> firstChild));
    // case UNARY_OP:
    //     return evaluateUnary(node);
        break;
    case BINARY_OP:
        return evaluateBinary(node);
    case DECLARATION_OP:
        {
            auto val = evaluateNode(node->firstChild);
            valueMap.emplace(node->context.valueLabel, val);
            break;
        }
    case ASSIGNMENT_OP:
        valueMap.at(node->context.valueLabel) = evaluateNode(node->firstChild);
        break;
    case RETRIEVAL_OP:
        return valueMap.at(node->context.valueLabel);
    case LITERAL_OP:
        return node -> context.valueStore;
    default:
        break;
    }

    return {NO_VALUE};
}

void Evaluator::evaluateChildren(ASTNode* node)
{
    for (ASTNode* child = codeRoot->firstChild; child != nullptr; child = child->nextSibling)
    {
        evaluateNode(child);
    }
}


void Evaluator::evaluateCode()
{
    evaluateChildren(codeRoot);
}

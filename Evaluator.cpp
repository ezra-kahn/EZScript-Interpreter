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
        printValue(evaluateNode(node -> firstChild));
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
    case BRANCH_OP:
        return evaluateBranch(node);
    default:
        break;
    }

    return {NO_VALUE};
}

bool Evaluator::isTrue(const ValueStore& valueStore)
{
    switch (valueStore.valueType)
    {
    case INTEGER_VALUE:
        return valueStore.value.integer > 0;
    case BOOLEAN_VALUE:
        return valueStore.value.boolean;
    default:
        return false;
    }
}


ValueStore Evaluator::evaluateBranch(ASTNode* node)
{
    ValueStore condition = evaluateNode(node -> firstChild);
    if (isTrue(condition))
    {
        return evaluateNode(node -> firstChild -> nextSibling);
    } else
    {
        return evaluateNode(node -> firstChild -> nextSibling -> nextSibling);
    }
}

void Evaluator::evaluateChildren(ASTNode* node)
{
    for (ASTNode* child = node->firstChild; child != nullptr; child = child->nextSibling)
    {
        evaluateNode(child);
    }
}


void Evaluator::evaluateCode()
{
    evaluateChildren(codeRoot);
}


void Evaluator::printValue(const ValueStore& v)
{
    switch (v.valueType)
    {
    case TEXT_VALUE:
        std::cout << v.value.text << std::endl;
        break;
    case BOOLEAN_VALUE:
        std::cout << v.value.boolean << std::endl;
        break;
    case INTEGER_VALUE:
    default:
        std::cout << v.value.integer << std::endl;
    }
}

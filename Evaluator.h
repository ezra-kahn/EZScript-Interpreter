//
// Created by Ezra Kahn on 5/11/26.
//

#ifndef EVALUATOR_H
#define EVALUATOR_H
#include "Parser.h"


class Evaluator {
public:
    Evaluator(ASTNode* codeRoot);
    void evaluateCode();
private:
    ValueStore evaluateBinary(ASTNode* node);
    ValueStore evaluateUnary(ASTNode* node);
    ValueStore evaluateNode(ASTNode* node);
    ValueStore evaluateBranch(ASTNode* node);
    void evaluateChildren(ASTNode* node);

    static void printValue(const ValueStore& v);

    static bool isTrue(const ValueStore& valueStore);

    ASTNode* codeRoot;
    std::unordered_map<std::string_view, ValueStore> valueMap;
};



#endif //EVALUATOR_H

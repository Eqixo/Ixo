#include <vector>
#include <string>
#include <lexer.cpp>

class ASTNode {
public:
    virtual ~ASTNode() {}
    virtual void acceptVisitor(ASTVisitor* visitor) = 0;
};

class BinaryExpression : public ASTNode {
public:
    BinaryExpression(const std::string& operatorToken, ASTNode* left, ASTNode* right)
        : operatorToken(operatorToken), left(left), right(right) {}

    void acceptVisitor(ASTVisitor* visitor) override {
        visitor->visitBinaryExpression(this);
    }

private:
    std::string operatorToken;
    ASTNode* left;
    ASTNode* right;
};

class VariableDeclaration : public ASTNode {
public:
    VariableDeclaration(const std::string& name, ASTNode* value)
        : name(name), value(value) {}

    void acceptVisitor(ASTVisitor* visitor) override {
        visitor->visitVariableDeclaration(this);
    }

private:
    std::string name;
    ASTNode* value;
};

class FunctionCall : public ASTNode {
public:
    FunctionCall(const std::string& functionName, std::vector<ASTNode*> arguments)
        : functionName(functionName), arguments(arguments) {}

    void acceptVisitor(ASTVisitor* visitor) override {
        visitor->visitFunctionCall(this);
    }

private:
    std::string functionName;
    std::vector<ASTNode*> arguments;
};

class ASTVisitor {
public:
    virtual void visitBinaryExpression(BinaryExpression* node) = 0;
    virtual void visitVariableDeclaration(VariableDeclaration* node) = 0;
    virtual void visitFunctionCall(FunctionCall* node) = 0;
};

class ASTBuilder : public ASTVisitor {
public:
    std::vector<ASTNode*> buildAST(const std::vector<Token>& tokens) {
        // Use the tokens to construct the AST
        // Call the appropriate visit method for each token
        // Create and return the root AST node

        // Example implementation:
        std::vector<ASTNode*> nodes;
        ASTNode* root = nullptr;

        for (const auto& token : tokens) {
            if (token.type == TokenType::IDENTIFIER) {
                if (token.value == "var") {
                    VariableDeclaration* variableDeclaration = new VariableDeclaration(token.value, nullptr);
                    nodes.push_back(variableDeclaration);
                } else if (token.value == "function") {
                    // Parse function declaration
                    // Create a FunctionCall node
                    // Add arguments to the FunctionCall node
                    // Add the FunctionCall node to the nodes vector
                }
            } else if (token.type == TokenType::OPERATOR) {
                // Parse binary expression
                // Create a BinaryExpression node
                // Add left and right operands to the BinaryExpression node
                // Add the BinaryExpression node to the nodes vector
            }
        }

        return nodes;
    }
};

std::vector<ASTNode*> parseAST(const std::vector<Token>& tokens) {
    ASTBuilder builder;
    return builder.buildAST(tokens);
}
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <sstream>
#include <cctype>
#include <stdexcept>
#include <locale>
#include <fstream>

// Token类型枚举
enum class TokenType {
    // 关键字
    INT, IF, ELSE, WHILE, RETURN, PRINT,

    // 标识符和字面量
    IDENTIFIER, NUMBER, STRING,

    // 运算符
    PLUS, MINUS, MULTIPLY, DIVIDE, ASSIGN,

    // 比较运算符
    EQ, NE, LT, GT, LE, GE,

    // 分隔符
    LPAREN, RPAREN, LBRACE, RBRACE, SEMICOLON, COMMA,

    // 其他
    END_OF_FILE
};

// Token类
class Token {
public:
    TokenType type;
    std::string value;
    int line;

    Token(TokenType t, const std::string& v, int l) : type(t), value(v), line(l) {}

    std::string toString() const {
        const char* typeNames[] = {
            "INT", "IF", "ELSE", "WHILE", "RETURN", "PRINT",
            "IDENTIFIER", "NUMBER", "STRING",
            "PLUS", "MINUS", "MULTIPLY", "DIVIDE", "ASSIGN",
            "EQ", "NE", "LT", "GT", "LE", "GE",
            "LPAREN", "RPAREN", "LBRACE", "RBRACE", "SEMICOLON", "COMMA",
            "END_OF_FILE"
        };

        std::string typeName = typeNames[static_cast<int>(type)];
        return "Token(" + typeName + ", '" + value + "', line " + std::to_string(line) + ")";
    }
};

// 抽象语法树节点基类
class ASTNode {
public:
    virtual ~ASTNode() = default;
    virtual void print(int indent = 0) const = 0;
};

// 程序节点
class Program : public ASTNode {
public:
    std::vector<std::unique_ptr<ASTNode>> statements;

    void print(int indent = 0) const override {
        std::wstring indentation(indent, L' ');
        std::wcout << indentation << L"Program:" << std::endl;
        for (const auto& stmt : statements) {
            stmt->print(indent + 2);
        }
    }
};

// 变量声明节点
class VariableDeclaration : public ASTNode {
public:
    std::string var_type;
    std::string name;

    VariableDeclaration(const std::string& t, const std::string& n) : var_type(t), name(n) {}

    void print(int indent = 0) const override {
        std::wstring indentation(indent, L' ');
        std::wcout << indentation << L"VariableDeclaration: " << var_type.c_str() << L" " << name.c_str() << std::endl;
    }
};

// 赋值节点
class Assignment : public ASTNode {
public:
    std::string name;
    std::unique_ptr<ASTNode> value;

    Assignment(const std::string& n, std::unique_ptr<ASTNode> v) : name(n), value(std::move(v)) {}

    void print(int indent = 0) const override {
        std::wstring indentation(indent, L' ');
        std::wcout << indentation << L"Assignment: " << name.c_str() << L" = " << std::endl;
        if (value) {
            value->print(indent + 2);
        }
    }
};

// 二元操作节点
class BinaryOp : public ASTNode {
public:
    std::unique_ptr<ASTNode> left;
    std::string op;
    std::unique_ptr<ASTNode> right;

    BinaryOp(std::unique_ptr<ASTNode> l, const std::string& o, std::unique_ptr<ASTNode> r)
        : left(std::move(l)), op(o), right(std::move(r)) {
    }

    void print(int indent = 0) const override {
        std::wstring indentation(indent, L' ');
        std::wcout << indentation << L"BinaryOp: " << op.c_str() << std::endl;
        if (left) {
            left->print(indent + 2);
        }
        if (right) {
            right->print(indent + 2);
        }
    }
};

// 数字字面量节点
class Number : public ASTNode {
public:
    int value;

    Number(int v) : value(v) {}

    void print(int indent = 0) const override {
        std::wstring indentation(indent, L' ');
        std::wcout << indentation << L"Number: " << value << std::endl;
    }
};

// 字符串字面量节点
class StringLiteral : public ASTNode {
public:
    std::string value;

    StringLiteral(const std::string& v) : value(v) {}

    void print(int indent = 0) const override {
        std::wstring indentation(indent, L' ');
        std::wcout << indentation << L"StringLiteral: \"" << value.c_str() << L"\"" << std::endl;
    }
};

// 变量节点
class Variable : public ASTNode {
public:
    std::string name;

    Variable(const std::string& n) : name(n) {}

    void print(int indent = 0) const override {
        std::wstring indentation(indent, L' ');
        std::wcout << indentation << L"Variable: " << name.c_str() << std::endl;
    }
};

// print语句节点
class PrintStatement : public ASTNode {
public:
    std::unique_ptr<ASTNode> expression;
    bool is_string;

    PrintStatement(std::unique_ptr<ASTNode> expr, bool is_str = false)
        : expression(std::move(expr)), is_string(is_str) {
    }

    void print(int indent = 0) const override {
        std::wstring indentation(indent, L' ');
        std::wcout << indentation << L"PrintStatement:" << (is_string ? L" (字符串)" : L" (数字)") << std::endl;
        if (expression) {
            expression->print(indent + 2);
        }
    }
};

// if语句节点
class IfStatement : public ASTNode {
public:
    std::unique_ptr<ASTNode> condition;
    std::unique_ptr<ASTNode> then_branch;
    std::unique_ptr<ASTNode> else_branch;

    IfStatement(std::unique_ptr<ASTNode> c, std::unique_ptr<ASTNode> t, std::unique_ptr<ASTNode> e = nullptr)
        : condition(std::move(c)), then_branch(std::move(t)), else_branch(std::move(e)) {
    }

    void print(int indent = 0) const override {
        std::wstring indentation(indent, L' ');
        std::wcout << indentation << L"IfStatement:" << std::endl;
        std::wcout << indentation << L"  Condition:" << std::endl;
        if (condition) condition->print(indent + 4);
        std::wcout << indentation << L"  Then:" << std::endl;
        if (then_branch) then_branch->print(indent + 4);
        if (else_branch) {
            std::wcout << indentation << L"  Else:" << std::endl;
            else_branch->print(indent + 4);
        }
    }
};

// while循环节点
class WhileStatement : public ASTNode {
public:
    std::unique_ptr<ASTNode> condition;
    std::unique_ptr<ASTNode> body;

    WhileStatement(std::unique_ptr<ASTNode> c, std::unique_ptr<ASTNode> b)
        : condition(std::move(c)), body(std::move(b)) {
    }

    void print(int indent = 0) const override {
        std::wstring indentation(indent, L' ');
        std::wcout << indentation << L"WhileStatement:" << std::endl;
        std::wcout << indentation << L"  Condition:" << std::endl;
        if (condition) condition->print(indent + 4);
        std::wcout << indentation << L"  Body:" << std::endl;
        if (body) body->print(indent + 4);
    }
};

// return语句节点
class ReturnStatement : public ASTNode {
public:
    std::unique_ptr<ASTNode> value;

    ReturnStatement(std::unique_ptr<ASTNode> v = nullptr) : value(std::move(v)) {}

    void print(int indent = 0) const override {
        std::wstring indentation(indent, L' ');
        std::wcout << indentation << L"ReturnStatement:" << std::endl;
        if (value) {
            value->print(indent + 2);
        }
    }
};

// 词法分析器
class Lexer {
private:
    std::string source;
    size_t position;
    int line;

    std::map<std::string, TokenType> keywords = {
        {"int", TokenType::INT},
        {"if", TokenType::IF},
        {"else", TokenType::ELSE},
        {"while", TokenType::WHILE},
        {"return", TokenType::RETURN},
        {"print", TokenType::PRINT}
    };

    std::map<std::string, TokenType> operators = {
        {"+", TokenType::PLUS},
        {"-", TokenType::MINUS},
        {"*", TokenType::MULTIPLY},
        {"/", TokenType::DIVIDE},
        {"=", TokenType::ASSIGN},
        {"==", TokenType::EQ},
        {"!=", TokenType::NE},
        {"<", TokenType::LT},
        {">", TokenType::GT},
        {"<=", TokenType::LE},
        {">=", TokenType::GE},
        {"(", TokenType::LPAREN},
        {")", TokenType::RPAREN},
        {"{", TokenType::LBRACE},
        {"}", TokenType::RBRACE},
        {";", TokenType::SEMICOLON},
        {",", TokenType::COMMA}
    };

public:
    Lexer(const std::string& src) : source(src), position(0), line(1) {}

    std::vector<Token> tokenize() {
        std::vector<Token> tokens;

        while (position < source.length()) {
            char current_char = source[position];

            if (std::isspace(current_char)) {
                if (current_char == '\n') line++;
                position++;
                continue;
            }

            // 处理字符串字面量
            else if (current_char == '"') {
                tokens.push_back(readString());
            }

            // 处理数字
            else if (std::isdigit(current_char)) {
                tokens.push_back(readNumber());
            }

            // 处理标识符和关键字
            else if (std::isalpha(current_char) || current_char == '_') {
                tokens.push_back(readIdentifier());
            }

            // 处理运算符
            else if (operators.find(std::string(1, current_char)) != operators.end()) {
                tokens.push_back(readOperator());
            }

            else {
                throw std::runtime_error("未知字符: '" + std::string(1, current_char) + "' at line " + std::to_string(line));
            }
        }

        tokens.push_back(Token(TokenType::END_OF_FILE, "", line));
        return tokens;
    }

private:
    Token readNumber() {
        size_t start_pos = position;
        while (position < source.length() && std::isdigit(source[position])) {
            position++;
        }
        std::string value = source.substr(start_pos, position - start_pos);
        return Token(TokenType::NUMBER, value, line);
    }

    Token readString() {
        position++; // 跳过开头的双引号
        size_t start_pos = position;

        // 读取直到遇到结束的双引号
        while (position < source.length() && source[position] != '"') {
            position++;
        }

        if (position >= source.length()) {
            throw std::runtime_error("未终止的字符串字面量 at line " + std::to_string(line));
        }

        std::string value = source.substr(start_pos, position - start_pos);
        position++; // 跳过结尾的双引号
        return Token(TokenType::STRING, value, line);
    }

    Token readIdentifier() {
        size_t start_pos = position;
        while (position < source.length() &&
            (std::isalnum(source[position]) || source[position] == '_')) {
            position++;
        }
        std::string value = source.substr(start_pos, position - start_pos);

        // 检查是否是关键字
        auto it = keywords.find(value);
        TokenType type = (it != keywords.end()) ? it->second : TokenType::IDENTIFIER;
        return Token(type, value, line);
    }

    Token readOperator() {
        // 检查双字符运算符
        if (position + 1 < source.length()) {
            std::string two_chars = source.substr(position, 2);
            auto it = operators.find(two_chars);
            if (it != operators.end()) {
                position += 2;
                return Token(it->second, two_chars, line);
            }
        }

        // 单字符运算符
        std::string single_char = std::string(1, source[position]);
        auto it = operators.find(single_char);
        if (it != operators.end()) {
            position++;
            return Token(it->second, single_char, line);
        }

        throw std::runtime_error("未知运算符");
    }
};

// 语法分析器
class Parser {
private:
    std::vector<Token> tokens;
    size_t position;

public:
    Parser(const std::vector<Token>& t) : tokens(t), position(0) {}

    std::unique_ptr<Program> parse() {
        auto program = std::make_unique<Program>();

        while (!isAtEnd()) {
            if (match({ TokenType::INT })) {
                program->statements.push_back(parseDeclaration());
            }
            else if (match({ TokenType::IF })) {
                program->statements.push_back(parseIfStatement());
            }
            else if (match({ TokenType::WHILE })) {
                program->statements.push_back(parseWhileStatement());
            }
            else if (match({ TokenType::RETURN })) {
                program->statements.push_back(parseReturnStatement());
            }
            else if (match({ TokenType::PRINT })) {
                program->statements.push_back(parsePrintStatement());
            }
            else if (check(TokenType::IDENTIFIER)) {
                program->statements.push_back(parseAssignment());
            }
            else {
                throw std::runtime_error("意外的token: " + peek().value + " at line " + std::to_string(peek().line));
            }
        }

        return program;
    }

private:
    std::unique_ptr<ASTNode> parseDeclaration() {
        Token name_token = consume(TokenType::IDENTIFIER, "期望变量名");
        consume(TokenType::SEMICOLON, "期望分号");
        return std::make_unique<VariableDeclaration>("int", name_token.value);
    }

    std::unique_ptr<ASTNode> parseAssignment() {
        Token name_token = consume(TokenType::IDENTIFIER, "期望变量名");
        consume(TokenType::ASSIGN, "期望等号");
        auto value = parseExpression();
        consume(TokenType::SEMICOLON, "期望分号");
        return std::make_unique<Assignment>(name_token.value, std::move(value));
    }

    std::unique_ptr<ASTNode> parsePrintStatement() {
        consume(TokenType::LPAREN, "期望左括号");

        std::unique_ptr<ASTNode> expression;
        bool is_string = false;

        if (check(TokenType::STRING)) {
            Token string_token = advance();
            expression = std::make_unique<StringLiteral>(string_token.value);
            is_string = true;
        }
        else {
            expression = parseExpression();
            is_string = false;
        }

        consume(TokenType::RPAREN, "期望右括号");
        consume(TokenType::SEMICOLON, "期望分号");
        return std::make_unique<PrintStatement>(std::move(expression), is_string);
    }

    std::unique_ptr<ASTNode> parseIfStatement() {
        consume(TokenType::LPAREN, "期望左括号");
        auto condition = parseExpression();
        consume(TokenType::RPAREN, "期望右括号");
        consume(TokenType::LBRACE, "期望左大括号");

        auto then_branch = parseStatementBlock();

        std::unique_ptr<ASTNode> else_branch = nullptr;
        if (match({ TokenType::ELSE })) {
            consume(TokenType::LBRACE, "期望左大括号");
            else_branch = parseStatementBlock();
        }

        return std::make_unique<IfStatement>(std::move(condition), std::move(then_branch), std::move(else_branch));
    }

    std::unique_ptr<ASTNode> parseWhileStatement() {
        consume(TokenType::LPAREN, "期望左括号");
        auto condition = parseExpression();
        consume(TokenType::RPAREN, "期望右括号");
        consume(TokenType::LBRACE, "期望左大括号");
        auto body = parseStatementBlock();
        return std::make_unique<WhileStatement>(std::move(condition), std::move(body));
    }

    std::unique_ptr<ASTNode> parseReturnStatement() {
        std::unique_ptr<ASTNode> value = nullptr;
        if (!check(TokenType::SEMICOLON)) {
            value = parseExpression();
        }
        consume(TokenType::SEMICOLON, "期望分号");
        return std::make_unique<ReturnStatement>(std::move(value));
    }

    std::unique_ptr<Program> parseStatementBlock() {
        auto program = std::make_unique<Program>();

        while (!check(TokenType::RBRACE) && !isAtEnd()) {
            if (match({ TokenType::INT })) {
                program->statements.push_back(parseDeclaration());
            }
            else if (match({ TokenType::IF })) {
                program->statements.push_back(parseIfStatement());
            }
            else if (match({ TokenType::WHILE })) {
                program->statements.push_back(parseWhileStatement());
            }
            else if (match({ TokenType::RETURN })) {
                program->statements.push_back(parseReturnStatement());
            }
            else if (match({ TokenType::PRINT })) {
                program->statements.push_back(parsePrintStatement());
            }
            else if (check(TokenType::IDENTIFIER)) {
                program->statements.push_back(parseAssignment());
            }
            else {
                throw std::runtime_error("意外的token in block: " + peek().value);
            }
        }
        consume(TokenType::RBRACE, "期望右大括号");
        return program;
    }

    std::unique_ptr<ASTNode> parseExpression() {
        return parseComparison();
    }

    std::unique_ptr<ASTNode> parseComparison() {
        auto expr = parseAddition();

        while (match({ TokenType::EQ, TokenType::NE, TokenType::LT, TokenType::GT, TokenType::LE, TokenType::GE })) {
            std::string op = previous().value;
            auto right = parseAddition();
            expr = std::make_unique<BinaryOp>(std::move(expr), op, std::move(right));
        }

        return expr;
    }

    std::unique_ptr<ASTNode> parseAddition() {
        auto expr = parseMultiplication();

        while (match({ TokenType::PLUS, TokenType::MINUS })) {
            std::string op = previous().value;
            auto right = parseMultiplication();
            expr = std::make_unique<BinaryOp>(std::move(expr), op, std::move(right));
        }

        return expr;
    }

    std::unique_ptr<ASTNode> parseMultiplication() {
        auto expr = parsePrimary();

        while (match({ TokenType::MULTIPLY, TokenType::DIVIDE })) {
            std::string op = previous().value;
            auto right = parsePrimary();
            expr = std::make_unique<BinaryOp>(std::move(expr), op, std::move(right));
        }

        return expr;
    }

    std::unique_ptr<ASTNode> parsePrimary() {
        if (match({ TokenType::NUMBER })) {
            return std::make_unique<Number>(std::stoi(previous().value));
        }

        if (match({ TokenType::IDENTIFIER })) {
            return std::make_unique<Variable>(previous().value);
        }

        if (match({ TokenType::STRING })) {
            return std::make_unique<StringLiteral>(previous().value);
        }

        if (match({ TokenType::LPAREN })) {
            auto expr = parseExpression();
            consume(TokenType::RPAREN, "期望右括号");
            return expr;
        }

        throw std::runtime_error("期望表达式");
    }

    bool match(const std::vector<TokenType>& types) {
        for (TokenType type : types) {
            if (check(type)) {
                advance();
                return true;
            }
        }
        return false;
    }

    bool check(TokenType type) {
        if (isAtEnd()) return false;
        return peek().type == type;
    }

    Token consume(TokenType type, const std::string& message) {
        if (check(type)) return advance();
        throw std::runtime_error(message + " at line " + std::to_string(peek().line));
    }

    Token advance() {
        if (!isAtEnd()) position++;
        return previous();
    }

    Token peek() {
        return tokens[position];
    }

    Token previous() {
        return tokens[position - 1];
    }

    bool isAtEnd() {
        return peek().type == TokenType::END_OF_FILE;
    }
};

// 代码生成器
class CodeGenerator {
private:
    std::map<std::string, int> variables;
    std::map<std::string, std::string> string_constants;
    int label_count;
    int string_count;
    std::vector<std::string> assembly_code;
    int indent_level;

public:
    CodeGenerator() : label_count(0), string_count(0), indent_level(0) {}

    std::string generate(std::unique_ptr<Program>& program) {
        // 首先收集所有变量声明和字符串常量
        collectVariablesAndStrings(program.get());

        // 然后生成代码
        generateProgram(program.get());

        std::string result;
        for (const auto& line : assembly_code) {
            result += line + "\n";
        }
        return result;
    }

private:
    void collectVariablesAndStrings(Program* program) {
        for (auto& stmt : program->statements) {
            if (auto decl = dynamic_cast<VariableDeclaration*>(stmt.get())) {
                // 为变量分配空间（每个变量4字节）
                if (variables.find(decl->name) == variables.end()) {
                    int offset = static_cast<int>(variables.size()) * 4;
                    variables[decl->name] = offset;
                }
            }
            else if (auto print_stmt = dynamic_cast<PrintStatement*>(stmt.get())) {
                // 收集字符串常量
                if (print_stmt->is_string) {
                    if (auto str_lit = dynamic_cast<StringLiteral*>(print_stmt->expression.get())) {
                        std::string label = "str_" + std::to_string(++string_count);
                        string_constants[label] = str_lit->value;
                    }
                }
            }
            else if (auto if_stmt = dynamic_cast<IfStatement*>(stmt.get())) {
                // 递归收集if语句中的变量和字符串
                if (auto then_program = dynamic_cast<Program*>(if_stmt->then_branch.get())) {
                    collectVariablesAndStrings(then_program);
                }
                if (if_stmt->else_branch) {
                    if (auto else_program = dynamic_cast<Program*>(if_stmt->else_branch.get())) {
                        collectVariablesAndStrings(else_program);
                    }
                }
            }
            else if (auto while_stmt = dynamic_cast<WhileStatement*>(stmt.get())) {
                // 递归收集while语句中的变量和字符串
                if (auto body_program = dynamic_cast<Program*>(while_stmt->body.get())) {
                    collectVariablesAndStrings(body_program);
                }
            }
            else if (auto program_stmt = dynamic_cast<Program*>(stmt.get())) {
                collectVariablesAndStrings(program_stmt);
            }
        }
    }

    void generateProgram(Program* program) {
        // 生成数据段
        emit(".data");

        // 为变量分配空间
        for (const auto& var : variables) {
            emit(var.first + ": .word 0");
        }

        // 字符串常量
        for (const auto& str : string_constants) {
            emit(str.first + ": .asciiz \"" + str.second + "\"");
        }

        // 换行符常量
        emit("newline: .asciiz \"\\n\"");

        emit("");
        emit(".text");
        emit(".globl main");
        emit("main:");
        increaseIndent();

        for (auto& stmt : program->statements) {
            generateStatement(stmt);
        }

        // 程序结束
        emit("li $v0, 10");
        emit("syscall");
    }

    void generateStatement(std::unique_ptr<ASTNode>& stmt) {
        if (auto decl = dynamic_cast<VariableDeclaration*>(stmt.get())) {
            generateDeclaration(decl);
        }
        else if (auto assign = dynamic_cast<Assignment*>(stmt.get())) {
            generateAssignment(assign);
        }
        else if (auto print_stmt = dynamic_cast<PrintStatement*>(stmt.get())) {
            generatePrintStatement(print_stmt);
        }
        else if (auto if_stmt = dynamic_cast<IfStatement*>(stmt.get())) {
            generateIfStatement(if_stmt);
        }
        else if (auto while_stmt = dynamic_cast<WhileStatement*>(stmt.get())) {
            generateWhileStatement(while_stmt);
        }
        else if (auto return_stmt = dynamic_cast<ReturnStatement*>(stmt.get())) {
            generateReturnStatement(return_stmt);
        }
        else if (auto program = dynamic_cast<Program*>(stmt.get())) {
            generateProgram(program);
        }
    }

    void generateDeclaration(VariableDeclaration* decl) {
        // 变量已经在collectVariables中处理了
        // 这里不需要生成具体代码，因为.data段已经分配了空间
    }

    void generateAssignment(Assignment* assign) {
        // 计算右侧表达式
        generateExpression(assign->value);

        // 存储到变量
        int offset = variables[assign->name];
        emit("sw $t0, " + std::to_string(offset) + "($zero)");
    }

    void generatePrintStatement(PrintStatement* print_stmt) {
        if (print_stmt->is_string) {
            // 打印字符串 - 查找对应的标签
            if (auto str_lit = dynamic_cast<StringLiteral*>(print_stmt->expression.get())) {
                // 找到对应的字符串标签
                std::string label;
                for (const auto& str : string_constants) {
                    if (str.second == str_lit->value) {
                        label = str.first;
                        break;
                    }
                }

                if (!label.empty()) {
                    emit("la $a0, " + label);
                    emit("li $v0, 4");
                    emit("syscall");
                }
            }
        }
        else {
            // 打印数字或表达式结果
            generateExpression(print_stmt->expression);
            emit("move $a0, $t0");
            emit("li $v0, 1");  // 打印整数系统调用
            emit("syscall");
        }

        // 打印换行
        emit("la $a0, newline");
        emit("li $v0, 4");
        emit("syscall");
    }

    void generateExpression(std::unique_ptr<ASTNode>& expr) {
        if (auto num = dynamic_cast<Number*>(expr.get())) {
            emit("li $t0, " + std::to_string(num->value));
        }
        else if (auto var = dynamic_cast<Variable*>(expr.get())) {
            int offset = variables[var->name];
            emit("lw $t0, " + std::to_string(offset) + "($zero)");
        }
        else if (auto bin_op = dynamic_cast<BinaryOp*>(expr.get())) {
            generateBinaryOp(bin_op);
        }
        else if (auto str_lit = dynamic_cast<StringLiteral*>(expr.get())) {
            // 字符串字面量不应该出现在这里
            throw std::runtime_error("字符串字面量不能用于算术表达式");
        }
    }

    void generateBinaryOp(BinaryOp* bin_op) {
        // 生成左操作数
        generateExpression(bin_op->left);
        emit("move $t1, $t0");

        // 生成右操作数
        generateExpression(bin_op->right);
        emit("move $t2, $t0");

        // 生成运算指令
        if (bin_op->op == "+") {
            emit("add $t0, $t1, $t2");
        }
        else if (bin_op->op == "-") {
            emit("sub $t0, $t1, $t2");
        }
        else if (bin_op->op == "*") {
            emit("mult $t1, $t2");
            emit("mflo $t0");
        }
        else if (bin_op->op == "/") {
            emit("div $t1, $t2");
            emit("mflo $t0");
        }
        else if (bin_op->op == "==") {
            std::string label = newLabel();
            std::string end_label = newLabel();
            emit("beq $t1, $t2, " + label);
            emit("li $t0, 0");
            emit("j " + end_label);
            emit(label + ":");
            emit("li $t0, 1");
            emit(end_label + ":");
        }
        else if (bin_op->op == "<") {
            emit("slt $t0, $t1, $t2");
        }
        else if (bin_op->op == ">") {
            emit("slt $t0, $t2, $t1");
        }
        else if (bin_op->op == "<=") {
            std::string label = newLabel();
            std::string end_label = newLabel();
            emit("slt $t0, $t2, $t1");  // $t0 = ($t1 <= $t2) 等价于 !($t2 < $t1)
            emit("xori $t0, $t0, 1");    // 取反
        }
        else if (bin_op->op == ">=") {
            std::string label = newLabel();
            std::string end_label = newLabel();
            emit("slt $t0, $t1, $t2");  // $t0 = ($t1 >= $t2) 等价于 !($t1 < $t2)
            emit("xori $t0, $t0, 1");    // 取反
        }
        else if (bin_op->op == "!=") {
            std::string label = newLabel();
            std::string end_label = newLabel();
            emit("beq $t1, $t2, " + label);
            emit("li $t0, 1");
            emit("j " + end_label);
            emit(label + ":");
            emit("li $t0, 0");
            emit(end_label + ":");
        }
    }

    void generateIfStatement(IfStatement* if_stmt) {
        // 生成条件表达式
        generateExpression(if_stmt->condition);

        std::string else_label = newLabel();
        std::string end_label = newLabel();

        // 条件跳转
        emit("beq $t0, $zero, " + else_label);

        // then 分支
        generateStatement(if_stmt->then_branch);
        emit("j " + end_label);

        // else 分支
        emit(else_label + ":");
        if (if_stmt->else_branch) {
            generateStatement(if_stmt->else_branch);
        }

        emit(end_label + ":");
    }

    void generateWhileStatement(WhileStatement* while_stmt) {
        std::string start_label = newLabel();
        std::string end_label = newLabel();

        emit(start_label + ":");

        // 生成条件表达式
        generateExpression(while_stmt->condition);

        emit("beq $t0, $zero, " + end_label);

        // 循环体
        generateStatement(while_stmt->body);
        emit("j " + start_label);

        emit(end_label + ":");
    }

    void generateReturnStatement(ReturnStatement* return_stmt) {
        if (return_stmt->value) {
            generateExpression(return_stmt->value);
            emit("move $v0, $t0");
        }
        emit("jr $ra");
    }

    std::string newLabel() {
        return "label_" + std::to_string(++label_count);
    }

    void emit(const std::string& instruction) {
        assembly_code.push_back(std::string(static_cast<size_t>(indent_level) * 4, ' ') + instruction);
    }

    void increaseIndent() {
        indent_level++;
    }

    void decreaseIndent() {
        if (indent_level > 0) indent_level--;
    }
};

// 预处理函数：将中文关键字替换为英文关键字
std::string preprocessChineseKeywords(const std::string& source_code) {
    std::string result = source_code;

    // 定义中文关键字到英文关键字的映射
    std::vector<std::pair<std::string, std::string>> keyword_mappings = {
        {"输出", "print"},
        {"整数", "int"},
        {"如果", "if"},
        {"否则", "else"},
        {"当", "while"},
        {"返回", "return"}
    };

    // 逐个替换关键字
    for (const auto& mapping : keyword_mappings) {
        size_t pos = 0;
        const std::string& chinese_keyword = mapping.first;
        const std::string& english_keyword = mapping.second;

        while ((pos = result.find(chinese_keyword, pos)) != std::string::npos) {
            // 检查前后字符，确保是完整的关键字（不是标识符的一部分）
            bool is_isolated = true;
            if (pos > 0 && (isalnum(result[pos - 1]) || result[pos - 1] == '_')) {
                is_isolated = false;
            }
            if (pos + chinese_keyword.length() < result.length() &&
                (isalnum(result[pos + chinese_keyword.length()]) ||
                    result[pos + chinese_keyword.length()] == '_')) {
                is_isolated = false;
            }

            if (is_isolated) {
                result.replace(pos, chinese_keyword.length(), english_keyword);
                pos += english_keyword.length();
            }
            else {
                pos += chinese_keyword.length();
            }
        }
    }

    return result;
}

// 保存汇编代码到文件
bool saveAssemblyToFile(const std::string& filename, const std::string& assembly_code) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        return false;
    }
    file << assembly_code;
    file.close();
    return true;
}

// 读取文件内容
std::string readFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("无法打开文件: " + filename);
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

// 编译器主函数
std::string compileToAssembly(const std::string& source_code) {
    try {
        // 预处理：将中文关键字替换为英文关键字
        std::string processed_code = preprocessChineseKeywords(source_code);

        std::wcout << L"=== 预处理后的代码 ===" << std::endl;
        std::wcout << processed_code.c_str() << std::endl;

        // 词法分析
        Lexer lexer(processed_code);
        auto tokens = lexer.tokenize();

        std::wcout << L"=== 词法分析结果 ===" << std::endl;
        for (const auto& token : tokens) {
            std::wcout << token.toString().c_str() << std::endl;
        }
        std::wcout << std::endl;

        // 语法分析
        Parser parser(tokens);
        auto ast = parser.parse();

        std::wcout << L"=== 语法分析完成 ===" << std::endl;
        std::wcout << L"=== 抽象语法树 ===" << std::endl;
        ast->print();
        std::wcout << std::endl;

        // 代码生成
        CodeGenerator generator;
        std::string assembly_code = generator.generate(ast);

        return assembly_code;

    }
    catch (const std::exception& e) {
        return std::string("编译错误: ") + e.what();
    }
}

// 编译文件
void compileFile(const std::string& filename) {
    try {
        std::wcout << L"正在读取文件: " << filename.c_str() << std::endl;
        std::string source_code = readFile(filename);

        std::wcout << L"=== 源代码 ===" << std::endl;
        std::wcout << source_code.c_str() << std::endl;

        // 预处理
        std::string processed_code = preprocessChineseKeywords(source_code);

        std::wcout << L"=== 预处理后的代码 ===" << std::endl;
        std::wcout << processed_code.c_str() << std::endl;

        std::wcout << L"=== 编译中... ===" << std::endl;
        std::string assembly = compileToAssembly(processed_code);

        // 生成输出文件名
        std::string output_filename;
        size_t dot_pos = filename.find_last_of('.');
        if (dot_pos != std::string::npos) {
            output_filename = filename.substr(0, dot_pos) + ".asm";
        }
        else {
            output_filename = filename + ".asm";
        }

        // 保存汇编文件
        if (saveAssemblyToFile(output_filename, assembly)) {
            std::wcout << L"=== 编译完成 ===" << std::endl;
            std::wcout << L"汇编代码已保存到: " << output_filename.c_str() << std::endl;
        }
        else {
            std::wcout << L"保存汇编文件失败！" << std::endl;
        }
    }
    catch (const std::exception& e) {
        std::wcout << L"编译错误: " << e.what() << std::endl;
    }
}

// 显示帮助信息
void showHelp() {
    std::wcout << L"用法:" << std::endl;
    std::wcout << L"  直接运行程序: 进入交互模式" << std::endl;
    std::wcout << L"  -f <文件名>: 编译指定的文件" << std::endl;
    std::wcout << L"  -h: 显示此帮助信息" << std::endl;
    std::wcout << std::endl;
    std::wcout << L"支持的文件扩展名: .c, .txt 或其他文本文件" << std::endl;
    std::wcout << L"支持的中文关键字: 输出, 整数, 如果, 否则, 当, 返回" << std::endl;
}

// 交互式主程序
void runInteractive() {
    std::wcout << L"=== 中文语言到汇编编译器 ===" << std::endl;
    std::wcout << L"支持语法: 变量声明、赋值、算术运算、if-else、while循环、print语句" << std::endl;
    std::wcout << L"支持中文关键字: 输出, 整数, 如果, 否则, 当, 返回" << std::endl;
    std::wcout << L"输入示例:" << std::endl;
    std::wcout << L"整数 x;" << std::endl;
    std::wcout << L"x = 10;" << std::endl;
    std::wcout << L"输出(x);" << std::endl;
    std::wcout << L"输出(\"Hello World\");" << std::endl;
    std::wcout << L"如果 (x < 20) { x = x + 1; }" << std::endl;
    std::wcout << L"输入'quit'退出" << std::endl;
    std::wcout << L"=" << std::wstring(50, L'=') << std::endl;

    while (true) {
        try {
            std::wcout << L"\n请输入代码 (多行输入，以空行结束):" << std::endl;
            std::vector<std::string> lines;
            std::string line;

            while (true) {
                std::getline(std::cin, line);
                if (line == "quit") return;
                if (line.empty()) break;
                lines.push_back(line);
            }

            if (lines.empty()) continue;

            std::string source_code;
            for (const auto& l : lines) {
                source_code += l + "\n";
            }

            std::wcout << L"\n=== 生成的汇编代码 ===" << std::endl;
            std::string assembly = compileToAssembly(source_code);
            std::wcout << assembly.c_str() << std::endl;

            // 询问是否保存文件
            std::wcout << L"\n是否保存为汇编文件？(y/n): ";
            std::string response;
            std::getline(std::cin, response);

            if (response == "y" || response == "Y") {
                std::wcout << L"请输入要保存的文件名（不含扩展名）: ";
                std::string filename;
                std::getline(std::cin, filename);

                if (!filename.empty()) {
                    std::string full_filename = filename + ".asm";
                    if (saveAssemblyToFile(full_filename, assembly)) {
                        std::wcout << L"文件已保存为: " << full_filename.c_str() << std::endl;
                    }
                    else {
                        std::wcout << L"保存文件失败！" << std::endl;
                    }
                }
            }

        }
        catch (...) {
            std::wcout << L"发生错误" << std::endl;
            break;
        }
    }
}

int main(int argc, char* argv[]) {
    // 设置本地化环境以支持中文输出
    std::locale::global(std::locale(""));
    std::wcout.imbue(std::locale());

    // 处理命令行参数
    if (argc > 1) {
        std::string arg1 = argv[1];

        if (arg1 == "-h" || arg1 == "--help") {
            showHelp();
            return 0;
        }
        else if (arg1 == "-f" && argc > 2) {
            std::string filename = argv[2];
            compileFile(filename);
            return 0;
        }
        else if (arg1 == "-f") {
            std::wcout << L"错误: 请指定要编译的文件名" << std::endl;
            showHelp();
            return 1;
        }
        else {
            // 如果没有指定选项，直接编译文件
            compileFile(arg1);
            return 0;
        }
    }

    // 如果没有命令行参数，进入交互模式
    runInteractive();
    return 0;
}
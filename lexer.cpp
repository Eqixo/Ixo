// Include necessary libraries for input/output, string manipulation, and data structures
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <cctype>

// Define an enumeration for different token types
enum class TokenType {
    START,           // Start of a function, class, or statement
    END,             // End of a function, class, or statement
    COMMENT,         // Line comment
    KEYWORD,         // Keyword (e.g. "fn", "class", etc.)
    TYPE,            // Data type (e.g. "int", "float", etc.)
    IDENTIFIER,      // Identifier (e.g. variable name)
    OPERATOR,        // Operator (e.g. "+", "-", etc.)
    NUMBER,          // Number (e.g. "123", "3.14", etc.)
    STRING,          // String (e.g. "hello", etc.)
    DELIMITER,       // Delimiter character (e.g. "(", ")", etc.)
    PUNCTUATION,     // Punctuation character (e.g. ":", ";", etc.)
    UNKNOWN          // Unknown token type
};

// Create a map to store the string representation of each token type
std::unordered_map<TokenType, std::string> tokenTypeStrings = {
    {TokenType::START, "START          "}, {TokenType::END, "END            "}, {TokenType::COMMENT, "COMMENT        "},
    {TokenType::KEYWORD, "KEYWORD        "}, {TokenType::TYPE, "TYPE           "}, {TokenType::IDENTIFIER, "IDENTIFIER     "},
    {TokenType::OPERATOR, "OPERATOR       "}, {TokenType::NUMBER, "NUMBER         "}, {TokenType::STRING, "STRING         "},
    {TokenType::DELIMITER, "DELIMITER      "}, {TokenType::PUNCTUATION, "PUNCTUATION    "}, {TokenType::UNKNOWN, "UNKNOWN        "}
};

// Define a struct to represent a token
struct Token {
    TokenType type;  // Token type
    std::string value;  // Token value
};

// Define a class to perform lexical analysis (tokenization)
class Lexer {
public:
    // Constructor to initialize the lexer with input string
    Lexer(const std::string& input) : input(input), pos(0) {}

    // Method to tokenize the input string
    std::vector<Token> tokenize() {
        // Create a vector to store the tokens
        std::vector<Token> tokens;

        // Iterate through the input string
        while (pos < input.size()) {
            // Get the current character
            char c = input[pos];

            // Check if the character is a comment
            if (c == '#') {
                // Consume the comment and add it to the tokens vector
                tokens.push_back(consumeComment());
            }
            // Check if the character is a letter (start of a word)
            else if (isalpha(c)) {
                // Consume the word and add it to the tokens vector
                tokens.push_back(consumeWord());
            }
            // Check if the character is a digit or decimal point (start of a number)
            else if (isdigit(c) || c == '.') {
                // Consume the number and add it to the tokens vector
                tokens.push_back(consumeNumber());
            }
            // Check if the character is a quote (start of a string)
            else if (c == '"' || c == '\'') {
                // Consume the string and add it to the tokens vector
                tokens.push_back(consumeString());
            }
            // Check if the character is a delimiter (e.g. "(", ")", etc.)
            else if (strchr("()[]{}", c)) {
                // Create a token for the delimiter and add it to the tokens vector
                tokens.push_back({ c == '{' ? TokenType::START : (c == '}' ? TokenType::END : TokenType::DELIMITER), std::string(1, input[pos++]) });
            }
            // Check if the character is punctuation (e.g. ":", ";", etc.)
            else if (strchr(":,;", c)) {
                // Create a token for the punctuation and add it to the tokens vector
                tokens.push_back({ TokenType::PUNCTUATION, std::string(1, input[pos++]) });
            }
            // Check if the character is whitespace
            else if (isspace(c)) {
                // Increment the position to skip the whitespace
                pos++;
            }
            // If none of the above conditions match, treat the character as an operator
            else {
                // Create a token for the operator and add it to the tokens vector
                size_t start = pos;
                while (pos < input.size() && (strchr("+-*/%^&|!<>=~", input[pos]) || input[pos] == '.')) pos++;
                tokens.push_back({ TokenType::OPERATOR, input.substr(start, pos - start) });
            }
        }

        // Return the vector of tokens
        return tokens;
    }

private:
    // Input string to be tokenized
    std::string input;

    // Current position in the input string
    size_t pos;

    // Method to consume a comment
    Token consumeComment() {
        // Record the start position of the comment
        size_t start = pos++;

        // Consume the comment until the end of the line
        while (pos < input.size() && input[pos] != '\n') pos++;

        // Create a token for the comment and return it
        return { TokenType::COMMENT, input.substr(start, pos - start) };
    }

    // Method to consume a word (identifier or keyword)
    Token consumeWord() {
        // Record the start position of the word
        size_t start = pos++;

        // Consume the word until a non-alphanumeric character is encountered
        while (pos < input.size() && (isalnum(input[pos]) || input[pos] == '_')) pos++;

        // Extract the word from the input string
        std::string value = input.substr(start, pos - start);

        // Check if the word is a keyword
        if (keywords.count(value)) {
            // Create a token for the keyword and return it
            return { TokenType::KEYWORD, value };
        }
        // Check if the word is a data type
        else if (types.count(value)) {
            // Create a token for the data type and return it
            return { TokenType::TYPE, value };
        }
        // Otherwise, create a token for an identifier and return it
        else {
            return { TokenType::IDENTIFIER, value };
        }
    }

    // Method to consume a number
    Token consumeNumber() {
        // Record the start position of the number
        size_t start = pos++;

        // Consume the number until a non-digit character is encountered
        while (pos < input.size() && (isdigit(input[pos]) || input[pos] == '.')) pos++;

        // Create a token for the number and return it
        return { TokenType::NUMBER, input.substr(start, pos - start) };
    }

    // Method to consume a string
    Token consumeString() {
        // Record the quote character
        char quote = input[pos++];

        // Record the start position of the string
        size_t start = pos;

        // Consume the string until the closing quote is encountered
        while (pos < input.size() && input[pos] != quote) pos++;

        // Create a token for the string and return it
        return { TokenType::STRING, input.substr(start, pos++ - start) };
    }

    // Map of keywords
    std::unordered_map<std::string, TokenType> keywords = {
        {"fn", TokenType::KEYWORD}, {"class", TokenType::KEYWORD}, {"return", TokenType::KEYWORD},
        {"if", TokenType::KEYWORD}, {"else", TokenType::KEYWORD}, {"while", TokenType::KEYWORD},
        {"for", TokenType::KEYWORD}, {"switch", TokenType::KEYWORD}, {"case", TokenType::KEYWORD},
        {"default", TokenType::KEYWORD}, {"do", TokenType::KEYWORD}, {"with", TokenType::KEYWORD},
        {"try", TokenType::KEYWORD}, {"except", TokenType::KEYWORD}, {"in", TokenType::KEYWORD},
        {"not", TokenType::KEYWORD}, {"and", TokenType::KEYWORD}, {"or", TokenType::KEYWORD}
    };

    // Map of data types
    std::unordered_map<std::string, TokenType> types = {
        {"int", TokenType::TYPE}, {"float", TokenType::TYPE}, {"str", TokenType::TYPE},
        {"bool", TokenType::TYPE}, {"tuple", TokenType::TYPE}, {"list", TokenType::TYPE},
        {"dict", TokenType::TYPE}
    };
};

int main() {
    // Input string to be tokenized
    std::string input = R"(
        # Ixo Language Examples
        fn foo(a: int, b: int): int {
            a += b
            if a > 0 {
                return a
            } else {
                b**
                return b
            }
        }
    )";

    // Create a lexer object
    Lexer lexer(input);

    // Tokenize the input string
    std::vector<Token> tokens = lexer.tokenize();

    // Print the tokens
    for (const auto& token : tokens) {
        std::cout << "Token: " << tokenTypeStrings[token.type] << "Value = " << token.value << std::endl;
    }

    return 0;
}
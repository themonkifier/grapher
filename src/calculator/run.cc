#include <iostream>

#include "parser.hh"

using std::cout, std::cin, std::endl, std::flush;

int main(int argc, char* argv[]) {
    (void) argc;
    (void) argv;

    while (true) {
        try {
            cout << ">> " << flush;
            string input;

            std::getline(cin, input);

            if (input == "q" || input == "quit") {
                break;
            }

            Lexer lexer(input);
            vector<Token> tokens = *( lexer.get_tokens() );

            Parser parser(tokens);
            unique_ptr<AST> ast = parser.Parse_Exp();

            if (ast != nullptr) {
                cout << ">> " << to_string(ast->eval()) << endl;
            }
        } catch (const std::runtime_error& e) {
            cout << ">> " << e.what() << endl;
        }
    }
}

// Mini LEX Tool (C++17)
// A small educational lexical-analyzer generator driven by a .lex specification.
// Group 9 - Micro Project: Design LEX Tool Using C/C++/JAVA

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <regex>
#include <map>
#include <iomanip>

using namespace std;

struct Token {
    string type, lexeme;
    int line = 1, column = 1;
};

struct LexRule {
    string name, pattern;
    regex compiled;
    bool skip = false;
    int priority = 0;
};

static string trim(const string& s) {
    size_t a = s.find_first_not_of(" \t\r\n");
    if (a == string::npos) return "";
    size_t b = s.find_last_not_of(" \t\r\n");
    return s.substr(a, b - a + 1);
}

class Lexer {
    vector<LexRule> rules;
public:
    // Reads lines of the form:  TOKEN NAME REGEX   or   SKIP NAME REGEX
    bool loadRules(const string& path) {
        ifstream in(path);
        if (!in) { cerr << "Cannot open LEX specification: " << path << "\n"; return false; }
        string line; int priority = 0, lineNo = 0;
        while (getline(in, line)) {
            ++lineNo; line = trim(line);
            if (line.empty() || line[0] == '#') continue;
            string kind, name;
            stringstream ss(line); ss >> kind >> name;
            string pattern; getline(ss, pattern); pattern = trim(pattern);
            if ((kind != "TOKEN" && kind != "SKIP") || name.empty() || pattern.empty()) {
                cerr << "Invalid LEX rule at line " << lineNo << "\n"; return false;
            }
            try {
                LexRule r{name, pattern, regex(pattern), kind == "SKIP", priority++};
                rules.push_back(r);
            } catch (const regex_error& e) {
                cerr << "Invalid regex at line " << lineNo << ": " << e.what() << "\n"; return false;
            }
        }
        return !rules.empty();
    }

    size_t ruleCount() const { return rules.size(); }

    // Longest-prefix match; on a tie the rule that appears first in the spec wins.
    bool scan(const string& source, vector<Token>& tokens, vector<string>& errors) const {
        size_t pos = 0; int line = 1, col = 1;
        while (pos < source.size()) {
            size_t bestLen = 0; const LexRule* best = nullptr;
            string remaining = source.substr(pos);
            for (const auto& r : rules) {
                smatch m;
                if (regex_search(remaining, m, r.compiled, regex_constants::match_continuous)) {
                    size_t len = m.length();
                    if (len > bestLen) { bestLen = len; best = &r; }
                }
            }
            if (!best || bestLen == 0) {
                ostringstream msg;
                msg << "Lexical error at line " << line << ", column " << col
                    << ": unexpected character '" << source[pos] << "'";
                errors.push_back(msg.str());
                if (source[pos] == '\n') { ++line; col = 1; } else ++col;
                ++pos; continue;
            }
            string lexeme = source.substr(pos, bestLen);
            if (!best->skip) tokens.push_back({best->name, lexeme, line, col});
            for (char c : lexeme) {
                if (c == '\n') { ++line; col = 1; } else ++col;
            }
            pos += bestLen;
        }
        return errors.empty();
    }
};

static string readFile(const string& path) {
    ifstream in(path);
    if (!in) throw runtime_error("Cannot open input file: " + path);
    stringstream buffer; buffer << in.rdbuf(); return buffer.str();
}

static void printTokens(const vector<Token>& tokens) {
    cout << "\nLEXICAL ANALYSIS\n";
    cout << left << setw(22) << "LEXEME" << setw(18) << "TOKEN" << "POSITION\n";
    cout << string(54, '-') << "\n";
    for (const auto& t : tokens)
        cout << left << setw(22) << t.lexeme << setw(18) << t.type
             << t.line << ":" << t.column << "\n";
    cout << "\nTotal tokens: " << tokens.size() << "\n";

    map<string, int> freq;
    for (const auto& t : tokens) ++freq[t.type];
    cout << "\nTOKEN SUMMARY\n";
    for (const auto& kv : freq)
        cout << left << setw(18) << kv.first << kv.second << "\n";
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        cout << "Mini LEX Tool (C++17)\n"
             << "Usage:\n"
             << "  mini_lex <lex-spec> <source-file>\n";
        return argc == 1 ? 0 : 1;
    }
    try {
        Lexer lexer;
        if (!lexer.loadRules(argv[1])) return 1;
        string source = readFile(argv[2]);
        vector<Token> tokens; vector<string> errors;
        lexer.scan(source, tokens, errors);
        printTokens(tokens);
        for (auto& e : errors) cerr << e << "\n";
        if (errors.empty()) cout << "\nRESULT: Lexical analysis completed with no errors.\n";
        else cout << "\nRESULT: " << errors.size() << " lexical error(s) found.\n";
        return errors.empty() ? 0 : 2;
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << "\n"; return 1;
    }
}

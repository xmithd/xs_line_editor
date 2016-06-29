#include <regex>
#include <iostream>
#include "Command.h"

//using namespace std;

Command::Command(const size_t current_line, const size_t last_line) : _current_line(current_line),
    _last_line(last_line), _line_number(1), _range_start(1), _range_end(1), _type(INVALID) { }

bool Command::parse(const string& input) {
    // remove all spaces and tabs.
    string sanitized(input);
    replace_all(sanitized, " ", "");
    replace_all(sanitized, "\t", "");
    
    if (sanitized.size() == 0) { // empty string.
        _type = MOVE_DOWN;
        _line_number = 1;
        return true;
    }

    // Parsing is simple if it's a single character.
    // Since parse_single_character doesn't recognize digits,
    // we let the regular expression handle it after.
    if (sanitized.size() == 1 && parse_single_character(sanitized[0]))
        return true;
    
    // use regex for the more complicated commands:
    regex pattern("^(?:"
                  //group 1 - capture 2 and 3.
                  "(([0-9]+),?[0-9]*([u|d]))|"
                  // group 4 - capture 5, 6, 7
                  "(([0-9]+|\\.|\\$),([0-9]+|\\.|\\$)([r|p|c|n]))|"
                  // group 8 - capture 9, 10
                  "(([0-9]+|\\.|\\$)([r|p|c|n]))|"
                  // group 11 - capture 12, 13
                  "(,([0-9]+|\\.|\\$)([r|p|c|n]))|"
                  // group 14 - capture 15, 16
                  "(([0-9]+|\\.|\\$),([r|p|c|n]))|"
                  // group 17 - capture 18, 19
                  "(([0-9]+|\\.|\\$),([0-9]+|\\.|\\$))|"
                  // group 20 - capture 21
                  "(([0-9]+))|"
                  // group 22, capture 23
                  "(([0-9]+|\\.|\\$),)|"
                  // group 24, capture 25, 26
                  "(([0-9]+|\\.|\\$),?[0-9]*([a|i|]))" // command like 1,1a or 5a or $a
                  ")$");
    smatch result;
    regex_match(sanitized, result, pattern);

    // if a valid command was issued, it will match one of the groups
    // described in the regular expression.
    if (result.size() > 0) {
        if (result.str(1).size() > 0) {
            // group 1
            _type = get_type_from_character(result.str(3));
            _line_number = get_numerical_value(result.str(2));
        } else if (result.str(4).size() > 0) {
            // group 4
            _type = get_type_from_character(result.str(7));
            _range_start = get_numerical_value(result.str(5));
            _range_end = get_numerical_value(result.str(6));
        } else if (result.str(8).size()>0) {
            // group 8
            _type = get_type_from_character(result.str(10));
            _range_start = get_numerical_value(result.str(9));
            _range_end = _range_start;
        } else if (result.str(11).size() > 0) {
            // group 11
            _type = get_type_from_character(result.str(13));
            _range_start = _current_line;
            _range_end = get_numerical_value(result.str(12));
        } else if (result.str(14).size() > 0) {
            // group 14
            _type = get_type_from_character(result.str(16));
            _range_start = get_numerical_value(result.str(15));
            _range_end = _current_line;
        } else if (result.str(17).size() > 0) {
            // group 17
            _type = PRINT;
            _range_start = get_numerical_value(result.str(18));
            _range_end = get_numerical_value(result.str(19));
        } else if (result.str(20).size() > 0) {
            // group 20
            _type = PRINT;
            _range_start = get_numerical_value(result.str(21));
            _range_end = _range_start;
        } else if (result.str(22).size() > 0) {
            // group 22
            _type = PRINT;
            _range_start = get_numerical_value(result.str(23));
            _range_end = _current_line;
        } else if (result.str(24).size() > 0) {
            // group 24
            _type = get_type_from_character(result.str(26));
            _range_start = get_numerical_value(result.str(25));
            _range_end = _range_start;
        } else {
            return false;
        }
        return true;
    }
    return false;
}

size_t Command::get_numerical_value(const string& val) {
    if (val == ".")
        return _current_line;
    if (val == "$")
        return _last_line;
    return stoul(val);
}

CommandType Command::get_type_from_character(const string& character) {
    if (character.size() > 1) {
        return INVALID;
    } else if (character.size() == 0) {
        return PRINT_CURRENT_LINE;
    }
    switch (character[0]) {
        case 'a':
            return APPEND;
        case 'i':
            return INSERT;
        case 'r':
            return REMOVE;
        case 'p':
            return PRINT;
        case 'n':
            return PRINT_WITH_LINE_NUM;
        case 'c':
            return CHANGE;
        case 'u':
            return MOVE_UP;
        case 'd':
            return MOVE_DOWN;
        case 'w':
            return WRITE;
        case 'q':
            return QUIT;
        case '=':
            return PRINT_CURRENT_LINE;
        default:
            return INVALID;
    }
}

bool Command::parse_single_character(char input) {
    switch (input) {
        case '=':
            _type = PRINT_CURRENT_LINE;
            _line_number = _current_line;
            return true;
        case ',':
            _type = PRINT;
            _range_start = 1;
            _range_end = _last_line;
            return true;
        case '$':
            _type = PRINT;
            _range_start = _last_line;
            _range_end = _last_line;
            return true;
        case '.':
            _type = PRINT;
            _range_start = _current_line;
            _range_end = _current_line;
            return true;
        case 'w':
            _type = WRITE;
            return true;
        case 'q':
            _type = QUIT;
            return true;
        case 'i':
            _type = INSERT;
            //_line_number = _current_line;
            _range_start = _current_line;
            _range_end = _range_start;
            return true;
        case 'a':
            _type = APPEND;
            //_line_number = _current_line;
            _range_start = _current_line;
            _range_end = _range_start;
            return true;
        case 'p':
            _type = PRINT;
            _range_start = _current_line;
            _range_end = _current_line;
            return true;
        case 'n':
            _type = PRINT_WITH_LINE_NUM;
            _range_start = _current_line;
            _range_end = _current_line;
            return true;
        case 'c':
            _type = CHANGE;
            _range_start = _current_line;
            _range_end = _current_line;
            return true;
        case 'u':
            _type = MOVE_UP;
            _line_number = 1;
            return true;
        case 'd':
            _type = MOVE_DOWN;
            _line_number = 1;
            return true;
        case 'r':
            _type = REMOVE;
            _range_start = _current_line;
            _range_end = _current_line;
            return true;
        default:
            return false;
    }
}



CommandType Command::getType() const {
    return _type;
}

size_t Command::getRangeStart() const {
    return _range_start;
}

size_t Command::getRangeEnd() const {
    return _range_end;
}

size_t Command::getLineNumber() const {
    return _line_number;
}

size_t Command::getNumberOfLines() const {
    return _line_number;
}

size_t Command::getCurrentLine() const {
    return _current_line;
}

bool Command::replace_all(string &input, const string &from, const string &to) {
    bool changed = false;
    if (input.empty() || from.empty()) {
        return changed;
    }
    string str;
    str.reserve(input.length());
    size_t start_pos(0);
    size_t found_pos;
    while ((found_pos = input.find(from, start_pos)) != string::npos) {
        changed = true;
        str += input.substr(start_pos, found_pos - start_pos);
        str += to;
        found_pos += from.length();
        start_pos = found_pos;
    }
    str += input.substr(start_pos, input.length()-start_pos);
    input.swap(str);
    return changed;
}

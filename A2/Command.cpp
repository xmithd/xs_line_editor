#include "Command.h"

Command::Command(const size_t current_line) : _current_line(current_line), _line_number(1), _range_start(1), _range_end(1), _type(INVALID) { }

bool Command::parse(const string& input) {
    // TODO
    return false;
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

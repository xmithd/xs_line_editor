#ifndef Command_h
#define Command_h

#include <string>

using namespace std;

enum CommandType {
    PRINT,
    QUIT,
    WRITE,
    INSERT,
    APPEND,
    REMOVE,
    PRINT_CURRENT_LINE,
    MOVE_UP,
    MOVE_DOWN,
    CHANGE,
    INVALID
};

/**
 * Class to translate a user command input into something the LineEditor class will understand.
 */
class Command {
private:
    
    //Some commands don't necessarily include a line number.
    //In those cases, we need to keep track of the current line number.
    size_t _current_line;
    
    // represents the line number or the number of lines depending on the command.
    size_t _line_number;
    
    // for range commands, it represents the start.
    size_t _range_start;
    
    // Represents the end range.
    size_t _range_end;
    
    // Represents the command type.
    CommandType _type;

public:
    // All default constructors and destructors.
    Command()=delete;
    Command(const size_t current_line);
    ~Command()=default;
    Command(const Command&)=default;

    /**
     * Parses the line input into a command.
     * Returns true on success and false
     * on failure.
     *
     */
    bool parse(const string& input);
    
    //Below are the getters for use after parsing is done.
    CommandType getType() const;
    
    size_t getRangeStart() const;
    
    size_t getRangeEnd() const;
    
    size_t getLineNumber() const;
    
    size_t getNumberOfLines() const;
    
    size_t getCurrentLine() const;
    
};

#endif /* Command_h */

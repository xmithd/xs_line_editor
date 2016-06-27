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
    PRINT_WITH_LINE_NUM,
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
    
    /**
     * Some commands don't necessarily include a line number.
     * In those cases, we need to keep track of the current
     * line number and last line.
     */
    size_t _current_line, _last_line;
    
    /**
     * represents the line number or the number of lines
     * depending on the command.
     */
    size_t _line_number;
    
    /**
     * for range commands, it represents the start.
     */
    size_t _range_start;
    
    /**
     * Represents the end range.
     */
    size_t _range_end;
    
    /**
     * Represents the command type.
     */
    CommandType _type;
    
    /**
     * Function to take care of the simple 1 character commands
     */
    bool parse_single_character(char input);
    
    /**
     * Given a string input, returns the command type.
     */
    CommandType get_type_from_character(const string&);
    
    /**
     * Gets the right numerical value given a string.
     * Valid string are only ".", "$" or a string representing
     * a positive integer. Unexpected results may occur otherwise.
     */
    size_t get_numerical_value(const string& val);

public:
    // All default constructors and destructors.
    Command()=delete;
    Command(const size_t current_line, const size_t last_line);
    ~Command()=default;
    Command(const Command&)=default;

    /**
     * Parses the line input into a command.
     * Returns true on success and false
     * on failure. It does not validate (i.e does not check correct range.)
     */
    bool parse(const string& input);
    
    //Below are the getters for use after parsing is done.
    CommandType getType() const;
    
    size_t getRangeStart() const;
    
    size_t getRangeEnd() const;
    
    size_t getLineNumber() const;
    
    size_t getNumberOfLines() const;
    
    size_t getCurrentLine() const;
    
    /**
     * Utility function that replaces all occurrences
     * of a string to another one.
     * @param input The string to search in.
     * @param from The string to replace.
     * @param to The string to replace to.
     * @return true if there is a match, false otherwise.
     */
    static bool replace_all(string& input, const string& from, const string& to);
    
};

#endif /* Command_h */

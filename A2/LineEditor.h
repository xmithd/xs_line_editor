#ifndef LineEditor_h
#define LineEditor_h

#include <iostream>
#include <list>
#include <string>
#include "Command.h"

using namespace std;

/**
 * Class representing the line editor.
 */
class LineEditor {
    
private:
    list<string> _buffer;
    /**
     * The current line (zero-based indexed)
     */
    size_t _current;
    
    /**
     * Variable to keep track if the buffer has been written.
     */
    bool _is_written;
    
    /**
     * Stores the filename
     */
    const string _filename;
    
    /**
     * Helper function to read from an input stream and add
     * lines to a buffer (could be a temporary one).
     * Note, _current needs to be tracked outside this function.
     * Use with ignorePeriod to true for reading files.
     */
    static void read_lines(istream&, list<string>& buffer, bool ignore_period=false);
    
    /**
     * Helper function to insert a temporary buffer into the main
     * buffer at the index specified by _current.
     * It also updates _current.
     */
    void insert_buffer(const list<string>& temp);
    
    /**
     * Exits the editor. Prompts the user to save file if it
     * has not yet been written.
     */
    void quit();
    
    /**
     * Appends the text input after the given line number.
     */
    void append(const size_t line_number);
    
    /**
     * Saves the buffer to file, line by line.
     */
    void write();
    
    /**
     * Inserts a the input text before the given line number.
     */
    void insert(const size_t line_number);
    
    /**
     * Removes the lines from the given range. Edges are inclusive. Line number must be valid.
     */
    void remove(const size_t from, const size_t to);
    
    /**
     * Displays current line number.
     */
    void print_current_line_number() const;
    
    /**
     * Moves up by the given number of lines
     */
    void move_up(const size_t number_of_lines);
    
    /**
     * Moves down by the given number of lines.
     * User is responsible for giving a valid number.
     */
    void move_down(const size_t number_of_lines);
    
    /**
     * Prints the lines. Set line_number to true for printing the line number
     * followed by a tab character.
     * The current line is moved to the last line printed.
     */
    void print(const size_t from, const size_t to, bool line_number=false);
    
    /**
     * Replaces the occurences of a given input by
     * another given input between the given [range]
     */
    void change(const size_t from, const size_t to);
    
    void print_empty_buffer_error();


public:
    LineEditor(const string& filename);
    
    /**
     * Call this to start the program.
     */
    void run();
    
    /**
     * The parser will generate a Command object
     * that can be passed to this function.
     * It will validate the values and output a proper
     * error message if there is any error.
     *
     */
    void executeCommand(const Command&);
};


#endif /* LineEditor_h */

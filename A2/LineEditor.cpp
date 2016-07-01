#include "LineEditor.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iterator>
#include <cstdlib>

void LineEditor::read_lines(istream & input_stream,
                            list<string>& buffer,
                            bool ignore_period) {
    string line;
    getline(input_stream, line);
    while (input_stream.good() && (ignore_period || line != ".")) {
        buffer.push_back(line);
        getline(input_stream, line);
    }
}

LineEditor::LineEditor(const string& filename) :
_current(0), _is_written(true), _filename(filename) {
    ifstream input_file(_filename, ios::in);
    
    if (!input_file) {
        cout << "Unable to open file " << _filename << endl;
        cout << "\"" << _filename << "\" " << "[New File]" << endl;
        //_is_written = false; // prompt before exiting in case of writing a new file.
    } else {
        read_lines(input_file, _buffer, true);
        _current = _buffer.size() ? (_buffer.size() - 1) : 0;
        cout << "\"" << _filename << "\" " << (_current + 1) << " line" << (_current ? "s" : "") << endl;
    }
    input_file.close();
}

void LineEditor::quit() {
    if (_is_written) {
        // nothing to do.
        exit(EXIT_SUCCESS);
    } else {
        bool understood(false);
        while (!understood) {
            string response;
            cout << "Save changes to " << _filename << " (y/n)? ";
            cin >> response;
            if (!cin) {
                cerr << "Something went wrong!";
                exit(EXIT_FAILURE);
            }
            if (response == "y" || response == "Y") {
                write();
                understood = true;
            } else if (response == "n" || response == "N") {
                understood = true;
            } else {
                cout << "Only 'y' and 'n' are valid responses." << endl;
            }
        }
        exit(EXIT_SUCCESS);
    }
}

void LineEditor::append(const size_t line_number) {
    list<string> temp;
    _current = line_number;
    read_lines(cin, temp);
    // insert the temporary buffer into the current buffer at the position indicated by _current.
    if (temp.size() > 0)
      insert_buffer(temp);
    else //put the cursor one back (to the original position)
      _current = line_number - 1;
}

void LineEditor::insert_buffer(const list<string> &temp) {
    if (temp.size() > 0) {
        auto origIt = next(begin(_buffer), _current);
        _buffer.insert(origIt, begin(temp), end(temp));
        _is_written = false;
        move_down(temp.size()-1, false);
    }
}

void LineEditor::write() {
    ofstream file(_filename, ios::out);
    if (!file) {
        cerr << "Fatal error when writing to file" << endl;
        exit(EXIT_FAILURE);
    }
    for (auto it = begin(_buffer); it != end(_buffer); ++it) {
        file << *it << endl;
    }
    file.close();
    _is_written = true;
    size_t size = _buffer.size();
    cout << "\"" << _filename << "\" " << size << " line" << (size > 1 ? "s " : " ") << "written" << endl;
}

void LineEditor::insert(const size_t line_number) {
    list<string> temp;
    _current = line_number-1;
    read_lines(cin, temp);
    insert_buffer(temp);
}

void LineEditor::print_empty_buffer_error() {
    cerr << "error: file empty - enter 'q' to quit, 'a' to append, or 'i' to insert." << endl;
}

void LineEditor::remove(const size_t from, const size_t to) {
    if (_buffer.size() == 0) {
        print_empty_buffer_error();
        return;
    }
    // place the 'cursor' after the deleted lines if there are any, otherwise before.
    if (to >= _buffer.size()) { // after the last line
        _current = from > 1 ? from-2 : 0; // place cursor in the line before (0 indexed)
    } else {
        _current = from-1;
    }
    _buffer.erase(next(begin(_buffer), from-1), next(begin(_buffer), to));
    _is_written = false;
}

void LineEditor::print_current_line_number() const {
    cout << _current + 1 << endl;
}

void LineEditor::move_up(const size_t number_of_lines, bool print_bof) {
    long long current = _current;
     current -= number_of_lines;
    if (current < 0) {
        if (print_bof)
            cout << "BOF reached" << endl;
        _current = 0;
    } else {
        _current = current;
    }
}

void LineEditor::move_down(const size_t number_of_lines, bool print_eof) {
    _current += number_of_lines;
    if (_current >= _buffer.size()) {
        if (print_eof)
            cout << "EOF reached" << endl;
        _current = _buffer.size() ? (_buffer.size()-1) : 0;
    }
}

void LineEditor::print(const size_t from, const size_t to, bool line_number) {
    if (_buffer.size() == 0) {
        print_empty_buffer_error();
        return;
    }
    size_t current = from;
    for (auto it = next(begin(_buffer), from-1); it != next(begin(_buffer), to); ++it) {
        ostringstream oss;
        oss << current << "\t";
        cout << (line_number? oss.str() : "") << *it << endl;
        ++current;
    }
    _current = to - 1;
}

void LineEditor::change(const size_t from, const size_t to) {
    if (_buffer.size() == 0) {
        print_empty_buffer_error();
        return;
    }
    string from_what, to_what;
    cout << "Change what? ";
    getline(cin, from_what);
    if (!cin.good()) {
        cerr << "Something went wrong!";
        exit(EXIT_FAILURE);
    }
    cout << "    to what? ";
    getline(cin, to_what);
    if (!cin.good()) {
        cerr << "Something went wrong!";
        exit(EXIT_FAILURE);
    }
    size_t current(from-1);
    for (auto it = next(begin(_buffer), from-1); it != next(begin(_buffer), to); ++it) {
        if (Command::replace_all(*it, from_what, to_what)) {
            _current = current;
        }
        ++current;
    }

}

void LineEditor::run() {
    cout << "Entering command mode." << endl;
    while(true) {
        string input;
        const size_t last_line = _buffer.size() == 0 ? 1 : _buffer.size();
        Command cmd(_current+1, last_line);
        cout << ":";
        //cin >> input;
        getline(cin, input);
        if (!cin.good()) {
            cerr << "Something went wrong!." << endl;
            exit(EXIT_FAILURE);
        }
        if (!cmd.parse(input)) {
            cerr << "error: invalid command" << endl;
        } else {
            executeCommand(cmd);
        }
    }
}

void LineEditor::executeCommand(const Command & command) {
    // command's current line is indexed starting at 0 while _current here is 0 based index.
    // The following line is useful for debugging
    // cout << "Current line: " << _current << " and command's: " << command.getCurrentLine() << endl;
    if (command.getRangeStart() > command.getRangeEnd()
        || (_buffer.size() > 0 && (command.getRangeEnd() > _buffer.size()))
        || command.getRangeStart() < 1
        || (_buffer.size() == 0 && (command.getRangeStart() > 1 || command.getRangeEnd() > 1))
        || command.getNumberOfLines() < 1
        )
    {
        cerr << "error: invalid range " << command.getRangeStart() << " through " << command.getRangeEnd() << endl;
        return;
    }
    
    switch (command.getType()) {
        case PRINT:
            print(command.getRangeStart(), command.getRangeEnd());
            break;
        case QUIT:
            quit();
            break;
        case WRITE:
            write();
            break;
        case INSERT:
            //insert(command.getLineNumber());
            insert(command.getRangeStart());
            break;
        case APPEND:
            //append(command.getLineNumber());
            append(command.getRangeStart());
            break;
        case REMOVE:
            remove(command.getRangeStart(), command.getRangeEnd());
            break;
        case PRINT_CURRENT_LINE:
            print_current_line_number();
            break;
        case PRINT_WITH_LINE_NUM:
            print(command.getRangeStart(), command.getRangeEnd(), true);
            break;
        case MOVE_UP:
            move_up(command.getNumberOfLines());
            break;
        case MOVE_DOWN:
            move_down(command.getNumberOfLines());
            break;
        case CHANGE:
            change(command.getRangeStart(), command.getRangeEnd());
            break;
        case INVALID:
        default:
            cerr << "An invalid command was issued." << endl;
            break;
    }
}



#include "LineEditor.h"
#include <string>
#include <fstream>

LineEditor::LineEditor(const string& filename) :
_filename(filename), _current(0), _is_written(true)  {
    ifstream input_file(_filename, ios::in);
    
    if (!input_file) {
        cout << "Unable to open " << _filename << endl;
        cout << "\"" << _filename << "\" " << "[New File]" << endl;
        _is_written = false;
    } else {
        read_lines(input_file, _buffer, true);
        _current = _buffer.size() - 1;
        cout << "\"" << _filename << "\"" << (_current + 1) << " line" << (_current ? "s" : "") << endl;
    }
    input_file.close();
}

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

void LineEditor::quit() {
    if (_is_written) {
        // nothing to do.
        exit(EXIT_SUCCESS);
    } else {
        bool understood(false);
        while (!understood) {
            cout << "Save changes to " << _filename << " (y/n)? ";
            string response;
            cin >> response;
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
    insert_buffer(temp);
}

void LineEditor::insert_buffer(const list<string> &temp) {
    if (temp.size() > 0) {
        auto origIt = next(begin(_buffer), _current);
        _buffer.insert(origIt, begin(temp), end(temp));
        _is_written = false;
        _current += temp.size();
    }
}

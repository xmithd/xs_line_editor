#include "LineEditor.h"
#include <string>
#include <fstream>

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
// TODO test this...
void LineEditor::replace_all(string &input, const string &from, const string &to) {
    if (input.empty() || from.empty()) {
        return;
    }
    string str;
    str.reserve(input.length());
    size_t start_pos(0);
    size_t found_pos;
    while ((found_pos = input.find(from, start_pos)) != string::npos) {
        str += input.substr(start_pos, found_pos - start_pos);
        str += to;
        found_pos += from.length();
        start_pos = found_pos;
    }
    input.swap(str);
}

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

// TODO check when buffer is empty
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

// TODO check when buffer is empty
void LineEditor::insert(const size_t line_number) {
    list<string> temp;
    _current = line_number-1;
    read_lines(cin, temp);
    insert_buffer(temp);
}

void LineEditor::remove(const size_t from, const size_t to) {
    // place the 'cursor' after the deleted lines if there are any, otherwise before.
    if (to >= _buffer.size()) {
        _current = from;
    } else {
        _current = from-1;
    }
    _buffer.erase(next(begin(_buffer), from-1), next(begin(_buffer), to));
    _is_written = false;
}

void LineEditor::current() const {
    cout << _current + 1 << endl;
}

void LineEditor::move_up(const size_t number_of_lines) {
    _current -= number_of_lines;
}

void LineEditor::move_down(const size_t number_of_lines) {
    _current += number_of_lines;
}

void LineEditor::print(const size_t from, const size_t to) const {
    for (auto it = next(begin(_buffer), from-1); it != next(begin(_buffer), to); ++it) {
        cout << *it << endl;
    }
}

void LineEditor::change(const size_t from, const size_t to) {
    string from_what, to_what;
    cout << "Change what? ";
    cin >> from_what;
    cout << "    to what? ";
    cin >> to_what;
    for (auto it = next(begin(_buffer), from-1); it != next(begin(_buffer), to); ++it) {
        replace_all(*it, from_what, to_what);
    }

}

void LineEditor::run() {
    cout << "Entering command mode." << endl;
    while(true) {
        string input;
        Command cmd(_current+1);
        cout << ".";
        cin >> input;
        
        if (!cmd.parse(input)) {
            cerr << "error: invalid command" << endl;
        } else {
            executeCommand(cmd);
        }
    }
}

void LineEditor::executeCommand(const Command & command) {
    //TODO
    quit();
}



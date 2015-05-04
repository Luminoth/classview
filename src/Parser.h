/*
==========
Copyright 2002 Energon Software

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software Foundation,
Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
==========
*/


#if !defined PARSER_H
#define PARSER_H


#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>


namespace Parser
{
    /*
     *  exceptions
     *
     */

    // exception thrown by the parser
    class ParserException : public std::exception
    {
    public:
        ParserException(const std::string& what) throw() : _what(what) {}
        virtual ~ParserException() throw() {}
        virtual const char* what() const throw() { return _what.c_str(); }
    private:
        std::string _what;
    };


    /*
     *  LineInfo class contains info about an individual line in the file
     *
     */

    class LineInfo
    {
    public:
        // returns true if the line is a toplevel entry
        const bool isTopLevel() const { return (type == 'n' || type == 'c' || type == 's' || type == 'u'); }

    public:
        friend std::ostream& operator<<(std::ostream& out, const LineInfo& lineInfo);
        friend const bool operator<(const LineInfo& lhs, const LineInfo& rhs);

    public:
        std::string name;
        std::string file;
        std::string description;
        char type;

        std::string owner;
        char ownerType;
    };


    /*
     *  FileInfo structure is a vector of lines in the file
     *
     */

    struct FileInfo
    {
        std::vector<LineInfo> lines;
    };

    // parses the tags info from a file, returns a 'new' FileInfo that must be deleted after use
    FileInfo* parse() throw(ParserException);
};


#endif

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


#include <cstdlib>
#include <algorithm>
#include <string>
#include <fstream>
#include <iostream>

#include <errno.h>

#include "Parser.h"
#include "Config.h"


/*
 *  external globals
 *
 */

extern Config::ConfigMap g_configMap;
extern int errno;


namespace Parser
{
    // cleans up the description (removes characters added by ctags, etc)
    void stripDescription(std::string* const description)
    {
        if(description->empty())
            return;

        // pull the /^ from the front
        if(description->substr(0, 2) == "/^")
            description->erase(0, 2);

        // pull spaces from front
        while(isspace((*description)[0]))
            description->erase(0, 1);

        // pull the /$ from the back
        if(description->substr(description->length()-2, 2) == "$/")
            description->erase(description->length()-2, 2);

        // replace all tabs with a single space
        std::replace(description->begin(), description->end(), '\t', ' ');
    }


    // parses the owner info the of the line
    void parseOwner(Parser::LineInfo* const lineInfo, const std::string& ownerInfo)
    {
        if(ownerInfo.empty()) {
            lineInfo->owner = "Globals";
            lineInfo->ownerType = 'n';
            return;
        }

        unsigned int pos = ownerInfo.find(':');
        if(pos != std::string::npos) {
            lineInfo->ownerType = ownerInfo[0];
            lineInfo->owner = ownerInfo.substr(pos+1);
        }

        // handle special cases
        if(lineInfo->ownerType == 'f') {
            lineInfo->owner = "Globals";
            lineInfo->ownerType = 'n';
        }
        if(lineInfo->owner.empty() && lineInfo->ownerType == 'u')
            lineInfo->owner = "Globals";
    }


    std::ostream& operator<<(std::ostream& out, const Parser::LineInfo& lineInfo)
    {
        out << "Name: " << lineInfo.name << std::endl;
        out << "File: " << lineInfo.file << std::endl;
        out << "Description: " << lineInfo.description << std::endl;
        out << "Type: " << lineInfo.type << std::endl;
        out << "Owner: " << lineInfo.owner << std::endl;
        out << "Owner Type: " << lineInfo.ownerType;
        return out;
    }


    const bool operator<(const Parser::LineInfo& lhs, const Parser::LineInfo& rhs)
    {
/* FIXME: wtf? this is backwards? */
        if(lhs.type == rhs.type)
            return lhs.name < rhs.name;

        switch(lhs.type)
        {
        case 'n':   // namespace
            return true;
        case 'c':   // class
            if(rhs.type == 'n')
                return false;
            return true;
        case 's':   // struct
            if(rhs.type == 'n' || rhs.type == 'c')
                return false;
            return true;
        case 'u':   // union
            if(rhs.type == 'n' || rhs.type == 'c' || rhs.type == 's')
                return false;
            return true;
        case 'e':   // enum
        case 'g':   // global ???
            if(rhs.type == 'n' || rhs.type == 'c' || rhs.type == 's' || rhs.type == 'u')
                return false;
            return true;
        case 't':   // typedef
            if(rhs.type == 'n' || rhs.type == 'c' || rhs.type == 's' || rhs.type == 'u' || rhs.type == 'e' || rhs.type == 'g')
                return false;
            return true;
        case 'f':   // function
            if(rhs.type == 'n' || rhs.type == 'c' || rhs.type == 's' || rhs.type == 'u' || rhs.type == 'e' || rhs.type == 'g' || rhs.type == 't')
                return false;
            return true;
        case 'v':   // variable
        case 'm':   // member
            if(rhs.type == 'n' || rhs.type == 'c' || rhs.type == 's' || rhs.type == 'u' || rhs.type == 'e' || rhs.type == 'g' || rhs.type == 't' || rhs.type == 'f')
                return false;
            return true;
        }

#if defined DEBUG
        std::cout << "Unknown type " << lhs.type << " - " << lhs << std::endl << std::endl;
#endif
        return true;
    }


    FileInfo* parse() throw(ParserException)
    {
        using std::string;
        using std::ifstream;
        using std::cerr;
        using std::endl;

        const string filename = g_configMap[Config::TAGS_FILE[0]];
        ifstream f(filename.c_str());
        if(!f)
            throw ParserException("Unable to open file '" + filename + "': " + strerror(errno));

        FileInfo inf;
        string line;

        // the first six lines are special
        for(int i=0; i<6; i++) {
            if(f.eof())
                throw ParserException(filename + " is not a valid tags file");
            std::getline(f, line);
            if(line.empty())
                throw ParserException(filename + " is not a valid tags file");

/* FIXME: when gcc fixes string::compare(), change this */

            switch(i)
            {
            case 0:
                if(strcmp(line.substr(0, 19).c_str(), "!_TAG_FILE_FORMAT\t2"))
                    throw ParserException("tags file format must be 2");
                break;
            case 1:
                if(strcmp(line.substr(0, 19).c_str(), "!_TAG_FILE_SORTED\t1"))
                    throw ParserException("tags file must be sorted");
                break;
            case 2:
                if(strcmp(line.substr(0, 20).c_str(), "!_TAG_PROGRAM_AUTHOR"))
                    throw ParserException(filename + " is not a valid tags file");
                break;
            case 3:
                if(strcmp(line.substr(0, 18).c_str(), "!_TAG_PROGRAM_NAME"))
                    throw ParserException(filename + " is not a valid tags file");
                break;
            case 4:
                if(strcmp(line.substr(0, 17).c_str(), "!_TAG_PROGRAM_URL"))
                    throw ParserException(filename + " is not a valid tags file");
                break;
            case 5:
                if(strcmp(line.substr(0, 21).c_str(), "!_TAG_PROGRAM_VERSION"))
                    throw ParserException(filename + " is not a valid tags file");
                break;
            default:
                throw ParserException("Unknown error");
            }
        }

        // parse the remaining lines
        while(!f.eof()) {
            std::getline(f, line);
            if(line.empty())
                continue;

            LineInfo t;
            string ownerInfo;
            unsigned int i = 0;
            while(line[i] != '\t' && i < line.length())
                t.name += line[i++];
            i++;
            while(line[i] != '\t' && i < line.length())
                t.file += line[i++];
            i++;
            while(line.substr(i, 2) != ";\"" && i < line.length())
                t.description += line[i++];
            i+=3;
            t.type = line[i++];

            // throw out makefile garbage, preprocessor directives, and assembly junk
            if((t.type == 'm' && line[line.length()] == line[i]) || t.type == 'd' || t.type == 'l')
                continue;

            if(i < line.length()) {
                i++;
                while(line[i] != '\t' && i < line.length())
                    ownerInfo += line[i++];
            }

            stripDescription(&t.description);
            parseOwner(&t, ownerInfo);
            inf.lines.push_back(t);
        }
        return new FileInfo(inf);
    }
};

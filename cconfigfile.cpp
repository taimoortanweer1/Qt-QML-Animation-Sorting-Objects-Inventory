
using namespace std;

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "cconfigfile.h"

// -----------------------------------------------------------------------------
// CConfigFile Constructor
// Public member of CConfigFile class
//
// Loads the provided config file into memory.
// -----------------------------------------------------------------------------
CConfigFile::CConfigFile(const char* cfgfile)
{
        char temp;
        int i;
        fstream fin1(cfgfile, ios::in | ios::binary);   // For getting num lines
        fstream fin2(cfgfile, ios::in | ios::binary);   // For data retrieval

        // If error...
        if(fin1.fail() || fin2.fail())
        {
                fin1.close();
                fin2.close();
                return;
        }

        // Get the number of line feeds in the file
        numLines = 0;
        while(1)
        {
                fin1.read(&temp, 1);

                // if reached end of file
                if(fin1.eof() == true)
                {
                        break;
                }

                if(temp == 10)
                {
                        numLines++;
                }
        }
        numLines++;
        fin1.close();

        // allocate memory for local file storage
        lines = new string[numLines];

        // load file
        i = 0;
        while(1)
        {
                fin2.read(&temp, 1);

                if(fin2.eof() == true || i>=numLines)
                {
                        break;
                }

                if(temp == 10)
                {
                        i++;
                }else
                {
                        if(temp != 13)
                        {
                                lines[i] += temp;
                        }
                }
        }
        fin2.close();
}

// -----------------------------------------------------------------------------
// CConfigFile Copy Constructor
// Public member of CConfigFile class
//
// Creates an exact, independent copy of an existing CConfigFile object.
// -----------------------------------------------------------------------------
CConfigFile::CConfigFile(const CConfigFile& cfg)
{
        // perform deep copy
        numLines = cfg.numLines;
        lines = new string[numLines];

        for(int i=0 ; i<numLines ; i++)
        {
                lines[i] = cfg.lines[i];
        }
}

// -----------------------------------------------------------------------------
// CConfigFile Destructor
// Public member of CConfigFile class
//
// Cleans up dynamically allocated memory.
// -----------------------------------------------------------------------------
CConfigFile::~CConfigFile()
{
        delete[] lines;
}

// -----------------------------------------------------------------------------
// getInt()
// Public member of CConfigFile class
//
// Retrieves an integer value from a specific section/variable.
//
// If value is non-existent or malformed, function returns false.
// -----------------------------------------------------------------------------
bool CConfigFile::getInt(const char* section, const char* varName,
int* output) const
{
    string s;
    if(getString(section, varName, &s) == false) return false;

// Convert to int
    istringstream iss(s);

    if((iss >> *output).fail())
    {
        cout << "Error loading Int: [" << section << "] " << varName << endl;
        return(false);
    }

    return(true);
}


// -----------------------------------------------------------------------------
// getLong()
// Public member of CConfigFile class
//
// Retrieves a long value from a specific section/variable.
//
// If value is non-existent or malformed, function returns false.
// -----------------------------------------------------------------------------
bool CConfigFile::getLong(const char* section, const char* varName,
long* output) const
{
    string s;
    if(getString(section, varName, &s) == false) return false;

// Convert to long
    istringstream iss(s);

    if((iss >> *output).fail())
    {
        cout << "Error loading Long: [" << section << "] " << varName << endl;
        return(false);
    }

    return(true);
}


// -----------------------------------------------------------------------------
// getDouble()
// Public member of CConfigFile class
//
// Retrieves an double value from a specific section/variable.
//
// If value is non-existent or malformed, function returns false.
// -----------------------------------------------------------------------------
bool CConfigFile::getDouble(const char* section, const char* varName,
double* output) const
{
    string s;
    if(getString(section, varName, &s) == false) return false;

// Convert to double
    istringstream iss(s);

    if((iss >> *output).fail())
    {
        cout << "Error loading Double: [" << section << "] " << varName << endl;
        return(false);
    }

    return(true);
}


// -----------------------------------------------------------------------------
// getString()
// Public member of CConfigFile class
//
// Retrieves a string from a specific section/variable.
//
// If value is non-existent or malformed, function returns false.
// -----------------------------------------------------------------------------
bool CConfigFile::getString(const char* section, const char* varName,
string* output) const
{
    int i;
    string str;
    size_t find1, find2;

    i = 0;
    str = section;
    str = "[" + str + "]";

// search for section
    for(i=0 ; i<numLines ; i++)
    {
// if is the section then
        if(lines[i].find(str) != string::npos &&
            lines[i].find("#") == string::npos)
        {
            break;
        }
    }

// search for the var name
    str = varName;
    while(1)
    {
        i++;
        if(i >= numLines)
        {
// if out of bounds
            cout << "Error loading String: [" << section << "] " << varName << endl;
            cout << "Reached end of file." << endl;
            return(false);
        }

// if found next section
        if(lines[i].find("[") != string::npos &&
            lines[i].find("#") == string::npos)
        {
            cout << "Error loading String: [" << section << "] " << varName << endl;
            cout << "Reached end of section." << endl;
            return(false);
        }

// if not a comment
        if(lines[i].find("#") == string::npos)
        {
            find1 = lines[i].find(str);
            find2 = lines[i].find("=");

// if found var name
            if(find1 != string::npos &&
                find2 != string::npos &&
                find1 < find2)
            {
// if var entry has quotes
                if(lines[i].find("\"") != string::npos)
                {
                    find1 = lines[i].find("\"");
                    find2 = lines[i].rfind("\"");

// if quotes are properly formatted
                    if(find1 != string::npos &&
                        find2 != string::npos &&
                        find1 < find2)
                    {
// return just the content
// inside the quotes
                        *output = lines[i].substr(find1+1,
                            find2-find1-1);
                    }
                    else
                    {
                        cout << "Error loading String: [" << section << "] " << varName << endl;
                        cout << "Quotes not formatted correctly." << endl;
                        return(false);
                    }
                }
                else
                {
// return everything after the
// equal sign
                    *output = lines[i].substr(find2+1,
                        lines[i].length()-find2-1);
                }
                return true;
            }
        }
    }
}

// -----------------------------------------------------------------------------
// setInt()
// Public member of CConfigFile class
//
// Updates an integer value to a specific section/variable.
//
// If section/variable is non-existent, function returns false.
// -----------------------------------------------------------------------------
bool CConfigFile::setInt(const char* section, const char* varName, int value)
{
        ostringstream oss;
        string s;

        // Convert to string
        oss << value;
        s = oss.str();

        if(setValString(section, varName, s) == false) return false;
        return true;
}

// -----------------------------------------------------------------------------
// setLong()
// Public member of CConfigFile class
//
// Updates a long value to a specific section/variable.
//
// If section/variable is non-existent, function returns false.
// -----------------------------------------------------------------------------
bool CConfigFile::setLong(const char* section, const char* varName, long value)
{
        ostringstream oss;
        string s;

        // Convert to string
        oss << value;
        s = oss.str();

        if(setValString(section, varName, s) == false) return false;
        return true;
}

// -----------------------------------------------------------------------------
// setDouble()
// Public member of CConfigFile class
//
// Updates a double value to a specific section/variable.
//
// If section/variable is non-existent, function returns false.
// -----------------------------------------------------------------------------
bool CConfigFile::setDouble(const char* section, const char* varName,
                            double value)
{
        ostringstream oss;
        string s;

        // Convert to string
        oss << value;
        s = oss.str();

        if(setValString(section, varName, s) == false) return false;
        return true;
}

// -----------------------------------------------------------------------------
// setString()
// Public member of CConfigFile class
//
// Updates a string value to a specific section/variable.
//
// If section/variable is non-existent, function returns false.
// -----------------------------------------------------------------------------
bool CConfigFile::setString(const char* section, const char* varName,
                            string value)
{
        // Place quotes around string
        return setValString(section, varName, "\"" + value + "\"");
}

// -----------------------------------------------------------------------------
// writeFile()
// Public member of CConfigFile class
//
// Writes all config data in memory back to a file with the provided name.
//
// Returns false on failure.
// -----------------------------------------------------------------------------
bool CConfigFile::writeFile(const char* cfgfile) const
{
        // If no lines...
        if(numLines<1) return false;

        fstream fout(cfgfile, ios::binary | ios::out | ios::trunc);

        // If file didn't open...
        if(fout.fail())
        {
                fout.close();
                return false;
        }

        // Write all lines with end of line characters except last line
        fout << lines[0];
        for(int i=1 ; i<numLines ; i++)
        {
                fout << "\r\n" << lines[i];
        }

        fout.close();
        return true;
}

// -----------------------------------------------------------------------------
// setValString()
// Private member of CConfigFile class
//
// Updates a "formatted" string value to a specific section/variable.
// Is used in all of the "set" functions.
//
// If section/variable is non-existent, function returns false.
// -----------------------------------------------------------------------------
bool CConfigFile::setValString(const char* section, const char* varName,
                               string value)
{
        int i;
        string str;
        size_t find1, find2;

        i = 0;
        str = section;
        str = "[" + str + "]";

        // search for section
        for(i=0 ; i<numLines ; i++)
        {
                // if is the section then
                if(lines[i].find(str) != string::npos &&
                   lines[i].find("#") == string::npos)
                {
                        break;
                }
        }

        // search for the var name
        str = varName;
        while(1)
        {
                i++;
                if(i >= numLines) return false;         // if out of bounds...

                // if not a comment and found next section...
                if(lines[i].find("[") != string::npos &&
                   lines[i].find("#") == string::npos) return false;

                // if not a comment...
                if(lines[i].find("#") == string::npos)
                {
                        find1 = lines[i].find(str);
                        find2 = lines[i].find("=");

                        // if found var name...
                        if(find1 != string::npos &&
                           find2 != string::npos &&
                           find1 < find2)
                        {
                                // assign the new value to the line
                                lines[i] = str + " = " + value;
                                return true;
                        }
                }
        }
}

#ifndef CCONFIGFILE_H_INCLUDED
#define CCONFIGFILE_H_INCLUDED
#include <string>
using namespace std;

class CConfigFile
{
        public:
                CConfigFile(const char* cfgfile);
                CConfigFile(const CConfigFile& cfg);    // copy constructor
                ~CConfigFile();

                // "Get" methods
                bool getInt(const char* section, const char* varName,
                            int* output) const;

                bool getLong(const char* section, const char* varName,
                             long* output) const;

                bool getDouble(const char* section, const char* varName,
                               double* output) const;

                bool getString(const char* section, const char* varName,
                               string* output) const;

                // "Set" methods
                bool setInt(const char* section, const char* varName,
                            int value);

                bool setLong(const char* section, const char* varName,
                             long value);

                bool setDouble(const char* section, const char* varName,
                               double value);

                bool setString(const char* section, const char* varName,
                               string value);


                bool writeFile(const char* cfgfile) const;
        private:
                string* lines;  // stores the text lines from the file
                int numLines;   // the number of lines in the array

                bool setValString(const char* section, const char* varName,
                                  string value);
};

#endif // CCONFIGFILE_H_INCLUDED

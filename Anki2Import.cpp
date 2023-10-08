#include <iostream>
#include <string>
#include <vector>
#include <fstream>

#include <stdio.h>
#include <sqlite3.h>
#include <string.h>

#include "CommonXml.h"
#include "Utils.h"

std::vector<std::string> ProcessedValues;

std::string Process(std::string value)
{
    std::string result;

    bool is_ok = false;
    for (int i = 0; i < value.size(); i++)
    {
        char next = value[i];
        if (next == 0x1f)
        {
            is_ok = true;
            break;
        }
        if (next == '<')
        {
            is_ok = true;
            break;
        }
        else
        {
            result.push_back(next);
            // std::cout << std::hex << (int)next << std::endl;
        }
    }

    if (!is_ok) std::cout << "Not ok!" << std::endl;
    
    std::cout << "Result size:" << result.size() << " value:" << result << std::endl;

    return result;
}

void PrintResults()
{
    std::fstream fresult;
    fresult.open("test/test_result.txt", std::fstream::out);

    for (int i = 0; i < ProcessedValues.size(); i++)
    {
        std::cout << ProcessedValues[i] << std::endl;
        fresult << ProcessedValues[i] << std::endl;
    }

    fresult.close();
}

static int callback(void *NotUsed, int argc, char **argv, char **colName)
{
    for (int i = 0; i < argc; i++)
    {
        // printf("%s = %s\n", colName[i], argv[i] ? argv[i] : "NULL");
        if (strcmp(colName[i], "flds") == 0)
        {
            std::string entry = Process(argv[i]);

            ProcessedValues.push_back(entry);
        }
    }

    // printf("\n");
    return 0;
}

int main(int argc, char **argv)
{
    sqlite3 *db = NULL;
    char *zErrMsg = 0;
    int rc = 0;

    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s DATABASE SQL-STATEMENT\n", argv[0]);
        return(1);
    }
    rc = sqlite3_open(argv[1], &db);
    if (rc)
    {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return(1);
    }

    char * query = "select * from notes;";
    if (argc == 3)
    {
        query = argv[2];
    }

    rc = sqlite3_exec(db, query, callback, 0, &zErrMsg);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    sqlite3_close(db);

    PrintResults();

    DictionaryParse * Handler = new DictionaryParse();
    const char* xmlFile = "JMdict_e";
    FillDictionary(xmlFile, Handler);

    std::cout << "Parsed dictionary size:" << Handler->Dictionary.size() << std::endl;

    std::vector<Entry > dictionary;
    for (int i = 0; i < ProcessedValues.size(); i++)
    {
        std::string value = ProcessedValues[i];

        Entry entry = FindValueInDictionary(Handler->Dictionary, value);

        if (entry.Kanji == "") 
            std::cout << "value:" << value << " not found in dictionary" << std::endl;
        else
            dictionary.push_back(entry);
    }

    SaveDictionaryToFile(dictionary, "imported.xml");

    return 0;
}

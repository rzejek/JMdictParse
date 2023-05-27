#include "Utils.h"

using namespace std;

void SaveDictionaryToFile(std::vector<Entry > dictionary, const char* xmlFile)
{
    std::fstream file;
    file.open(xmlFile, std::fstream::out | std::fstream::trunc);
    file << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << endl;
    file << "<JMdict>" << endl;

    for (int i = 0; i < dictionary.size(); i++)
    {
        Entry & entry = dictionary[i];
        file << "<entry>" << endl;
        file << "<k_ele>" << endl;
        file << "<keb>" << entry.Kanji;
        file << "</keb>" << endl;
        file << "</k_ele>" << endl;
        file << "<r_ele>" << endl;
        file << "<reb>" << entry.Romaji;
        file << "</reb>" << endl;
        file << "</r_ele>" << endl;
        file << "<sense>" << endl;
        for (int j = 0; j < entry.Glossary.size(); j++)
        {
            std::string gloss = entry.Glossary[j];
            file << "<gloss>"<< gloss<<"</gloss>"<<endl;
        }
        file << "</sense>" << endl;
        file << "</entry>" << endl;
    }

    file << "</JMdict>" << endl;
}


#include <xercesc/parsers/SAXParser.hpp>
#include <xercesc/sax/HandlerBase.hpp>
#include <xercesc/util/XMLString.hpp>

#include "MySaxHandler.hpp"

#include <iostream>
#include <fstream>

using namespace std;
using namespace xercesc;

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

int main (int argc, char* args[])
{
    try {
        XMLPlatformUtils::Initialize();
    }
    catch (const XMLException& toCatch) {
        char* message = XMLString::transcode(toCatch.getMessage());
        cout << "Error during initialization! :\n"
            << message << "\n";
        XMLString::release(&message);
        return 1;
    }

    const char* xmlFile = "JMdict_e";
    SAXParser* parser = new SAXParser();
//    parser->setDoValidation(true);
//    parser->setDoNamespaces(true);    // optional

    MySaxHandler * handler = new MySaxHandler();
    DocumentHandler* docHandler = handler;
    ErrorHandler* errHandler = (ErrorHandler*) handler;
    parser->setDocumentHandler(docHandler);
    parser->setErrorHandler(errHandler);

    try {
        parser->parse(xmlFile);
    }
    catch (const XMLException& toCatch) {
        char* message = XMLString::transcode(toCatch.getMessage());
        cout << "Exception message is: \n"
            << message << "\n";
        XMLString::release(&message);
        return -1;
    }
    catch (const SAXParseException& toCatch) {
        char* message = XMLString::transcode(toCatch.getMessage());
        cout << "Exception message is: \n"
            << message << "\n";
        XMLString::release(&message);
        return -1;
    }
    catch (...) {
        cout << "Unexpected Exception \n" ;
        return -1;
    }

    const char* xmlResultFile = "short.xml";
    SaveDictionaryToFile(handler->Dictionary, xmlResultFile);

    delete parser;
    delete docHandler;
    return 0;
}

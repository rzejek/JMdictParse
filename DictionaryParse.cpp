#include "DictionaryParse.h"
#include <iostream>

using namespace std;

DictionaryParse::DictionaryParse()
{
    isEntry = false;
    isK_ele = false;
    isKeb = false;
    isSense = false;
    isReb = false;
    isGloss = false;
    to_save = false;
}

DictionaryParse::~DictionaryParse()
{
}

void DictionaryParse::startElement(const XMLCh* const name,
                           AttributeList& attributes)
{
    char* message = XMLString::transcode(name);

    if (strcmp(message, "entry") == 0) 
    {
        isEntry = true;
        if (isEntry)
        {
            isEntry = false;
            to_save = false;
            CurrentValue.Glossary.clear();
            CurrentValue.Kanji = "";
            CurrentValue.Romaji = "";

            Dictionary.push_back(CurrentValue);
        }
    }
    if (strcmp(message, "k_ele") == 0) 
        isK_ele = true;
    if (strcmp(message, "keb") == 0) 
        isKeb = true;
    if (strcmp(message, "sense") == 0) 
        isSense = true;
    if (strcmp(message, "reb") == 0) 
        isReb = true;
    if (strcmp(message, "gloss") == 0) 
        isGloss = true;

    XMLString::release(&message);
}

void DictionaryParse::fatalError(const SAXParseException& exception)
{
    char* message = XMLString::transcode(exception.getMessage());
    cout << "Fatal Error: " << message
         << " at line: " << exception.getLineNumber()
         << endl;
    XMLString::release(&message);
}

void DictionaryParse::characters(const XMLCh *const chars, const XMLSize_t length)
{
    char* message = XMLString::transcode(chars);
    if (isKeb)
    {
        int len = strlen(message);
//        if (len == 3)
        {
            Dictionary[Dictionary.size() - 1].Kanji = std::string(message);
//            cout << message << endl;
            to_save = true;
        }
        isKeb = false;
    }
    if (isReb)
    {
        Dictionary[Dictionary.size() - 1].Romaji = std::string(message);
        isReb = false;
    }
    if (isGloss)
    {
        Dictionary[Dictionary.size() - 1].Glossary.push_back(std::string(message));
        isGloss = false;
    }

    XMLString::release(&message);
}

void DictionaryParse::endEntity(const XMLCh* const name)
{
    char* message = XMLString::transcode(name);
    cout <<"endEntity: "<<message<<endl;

    if (strcmp(message, "entry") == 0) 
        isEntry = false;
    if (strcmp(message, "k_ele") == 0) 
        isK_ele = false;
    if (strcmp(message, "keb") == 0) 
        isKeb = false;
    if (strcmp(message, "sense") == 0) 
        isSense = false;

    XMLString::release(&message);
}

void DictionaryParse::endElement (const XMLCh *const uri, const XMLCh *const localname, const XMLCh *const qname)
{
    char* message = XMLString::transcode(qname);
    cout <<"endElement: "<<message<<endl;

    if (strcmp(message, "entry") == 0) 
        isEntry = false;
    if (strcmp(message, "k_ele") == 0) 
        isK_ele = false;
    if (strcmp(message, "keb") == 0) 
        isKeb = false;
    if (strcmp(message, "sense") == 0) 
        isSense = false;

    XMLString::release(&message);
}


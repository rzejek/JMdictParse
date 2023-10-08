#include "DictionaryParse.h"
#include <iostream>

using namespace std;

DictionaryParse::DictionaryParse()
{
    isEntry = false;
    isK_ele = false;
    isKeb = false;
    isKeb2 = false;
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
            
            isKeb2 = false;
            isKeb = false;
            isK_ele = false;
            isSense = false;
            isReb = false;
            isGloss = false;

            CurrentValue.Glossary.clear();
            CurrentValue.Kanji = "";
            CurrentValue.Kanji2 = "";
            CurrentValue.Romaji = "";

            Dictionary.push_back(CurrentValue);
        }
    }
    if (strcmp(message, "k_ele") == 0) 
        isK_ele = true;
    if (strcmp(message, "keb") == 0)
    {
        if (isKeb) isKeb2 = true;
        isKeb = true;
    }
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
        if (isKeb2)
        {
            int len = strlen(message);
            Dictionary[Dictionary.size() - 1].Kanji2 = std::string(message);
        }
        else
        {
            Dictionary[Dictionary.size() - 1].Kanji = std::string(message);
            isKeb2 = true;
        }
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
    {
    }
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
    {
    }
    if (strcmp(message, "sense") == 0) 
        isSense = false;

    XMLString::release(&message);
}


#include "CommonXml.h"
#include <iostream>

#include <xercesc/parsers/SAXParser.hpp>
#include <xercesc/sax/HandlerBase.hpp>
#include <xercesc/util/XMLString.hpp>

using namespace std;

int FillDictionary(const char * xmlFile, DictionaryParse * Handler)
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

//    const char* xmlFile = "JMdict_e";
    SAXParser* parser = new SAXParser();
//    parser->setDoValidation(true);
//    parser->setDoNamespaces(true);    // optional

    DocumentHandler* docHandler = Handler;
    ErrorHandler* errHandler = (ErrorHandler*)Handler;
    parser->setDocumentHandler(docHandler);
    parser->setErrorHandler(errHandler);

    int err = 0;

    try {
        parser->parse(xmlFile);
    }
    catch (const XMLException& toCatch) {
        char* message = XMLString::transcode(toCatch.getMessage());
        cout << "Exception message is: \n"
            << message << "\n";
        XMLString::release(&message);
        err = 1;
    }
    catch (const SAXParseException& toCatch) {
        char* message = XMLString::transcode(toCatch.getMessage());
        cout << "Exception message is: \n"
            << message << "\n";
        XMLString::release(&message);
        err = 2;
    }
    catch (...) {
        cout << "Unexpected Exception \n" ;
        err = 3;
    }

    delete parser;
    return err;

}


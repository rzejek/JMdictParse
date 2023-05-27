#include <xercesc/parsers/SAXParser.hpp>
#include <xercesc/sax/HandlerBase.hpp>
#include <xercesc/util/XMLString.hpp>

#include "MySaxHandler.hpp"

#include <iostream>
#include <fstream>

#include "Utils.h"

using namespace std;
using namespace xercesc;

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

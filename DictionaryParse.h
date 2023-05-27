#include <xercesc/sax/HandlerBase.hpp>

#include "Entry.h"

using namespace xercesc;

class DictionaryParse : public HandlerBase 
{
public:
    void startElement(const XMLCh* const, AttributeList&);
    void fatalError(const SAXParseException&);

    void characters(const XMLCh *const chars, const XMLSize_t length);
    virtual void endElement (const XMLCh *const uri, const XMLCh *const localname, const XMLCh *const qname);
    virtual void endEntity(const XMLCh* const name);

    DictionaryParse();
    virtual ~DictionaryParse();

    std::vector<Entry > Dictionary;

private:
    bool isEntry;
    bool isK_ele;
    bool isKeb;
    bool isSense;
    bool isReb;
    bool isGloss;

    Entry CurrentValue;

    bool to_save;
};


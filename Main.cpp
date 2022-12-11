#include <iostream>
#include <gtk/gtk.h>
#include <vector>

#include "Entry.h"
#include "DictionaryParse.h"

#include <xercesc/parsers/SAXParser.hpp>
#include <xercesc/sax/HandlerBase.hpp>
#include <xercesc/util/XMLString.hpp>

GtkLabel * LabelKanji = NULL;
GtkLabel * LabelRomaji = NULL;
GtkLabel * LabelGloss1= NULL;
GtkLabel * LabelGloss2= NULL;
GtkLabel * LabelGloss3= NULL;

DictionaryParse * Handler = NULL;


using namespace std;

int FillDictionary(const char * xmlFile)
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

    Handler = new DictionaryParse();
    DocumentHandler* docHandler = Handler;
    ErrorHandler* errHandler = (ErrorHandler*)Handler;
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

}

void SetEntry(Entry & entry)
{
    gtk_label_set_text_with_mnemonic(LabelKanji, entry.Kanji.c_str());
    gtk_label_set_text_with_mnemonic(LabelRomaji, entry.Romaji.c_str());
    if (entry.Glossary.size() > 0) 
        gtk_label_set_text_with_mnemonic(LabelGloss1, entry.Glossary[0].c_str());
    else
        gtk_label_set_text_with_mnemonic(LabelGloss1, "");
    if (entry.Glossary.size() > 1) 
        gtk_label_set_text_with_mnemonic(LabelGloss2, entry.Glossary[1].c_str());
    else
        gtk_label_set_text_with_mnemonic(LabelGloss2, "");
    if (entry.Glossary.size() > 2) 
        gtk_label_set_text_with_mnemonic(LabelGloss3, entry.Glossary[2].c_str());
    else
        gtk_label_set_text_with_mnemonic(LabelGloss3, "");
}

void next_clicked(GtkWidget *widget, gpointer data) 
{
//    std::cout << "Handler: " << Handler << " size: " << Handler->Dictionary.size() << endl;
    int num = (rand() % Handler->Dictionary.size());
    Entry & entry = Handler->Dictionary[num];
    SetEntry(entry);
}

int main(int argc, char** argv)
{
    srand(time(NULL));

    char* xmlFile = "JMdict_e";
    std::cout << "argc:"  << argc << endl;
    if (argc == 2) 
    {
        xmlFile = argv[1];
        std::cout << "argv:"  << argv[1] << endl;
    }
    FillDictionary(xmlFile);

    gtk_init(&argc, &argv);

    // Create a builder object that will load the file.
    GtkBuilder* builder = gtk_builder_new();

    // Load the XML from a file.
    gtk_builder_add_from_file(builder, "GtkWindow.glade", NULL);

    // Get the object called 'main_window' from the file and show it.
    GObject* window = gtk_builder_get_object(builder, "main_window");
    gtk_widget_show(GTK_WIDGET(window));

//    gtk_window_set_default_size(GTK_WINDOW(window), 400, 200);

    GtkWidget * btnNext = GTK_WIDGET(gtk_builder_get_object(builder, "NextButton"));
    LabelKanji = GTK_LABEL(gtk_builder_get_object(builder, "KanjiLabel"));
    LabelRomaji = GTK_LABEL(gtk_builder_get_object(builder, "RomajiLabel"));
    LabelGloss1 = GTK_LABEL(gtk_builder_get_object(builder, "Gloss1"));
    LabelGloss2 = GTK_LABEL(gtk_builder_get_object(builder, "Gloss2"));
    LabelGloss3 = GTK_LABEL(gtk_builder_get_object(builder, "Gloss3"));

    // Connect the signal handlers defined in the glade file.
    // (Note: if you're looking for the c++ way to do this, there's no
    // support for binding C++ signal handlers. You must use 'extern
    // "C"' functions as handlers.)
//    int my_user_data = 0xDEADBEEF;
//    gtk_builder_connect_signals(builder, &my_user_data);


    g_signal_connect(G_OBJECT(btnNext), "clicked", G_CALLBACK(next_clicked), NULL);

    // Quit the app when the window is closed.
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Main loop.
    gtk_main();

    return 0;
}

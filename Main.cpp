#include <iostream>
#include <gtk/gtk.h>
#include <vector>

#include "Entry.h"
#include "DictionaryParse.h"
#include "Utils.h"

#include <xercesc/parsers/SAXParser.hpp>
#include <xercesc/sax/HandlerBase.hpp>
#include <xercesc/util/XMLString.hpp>

GtkLabel * LabelKanji = NULL;
GtkLabel * LabelRomaji = NULL;
GtkLabel * LabelGloss1= NULL;
GtkLabel * LabelGloss2= NULL;
GtkLabel * LabelGloss3= NULL;

DictionaryParse * Handler = NULL;

GtkButton * SearchButton = NULL;
GtkButton * RememberButton = NULL;
GObject* SearchWindow = NULL;
GtkEntry * SearchEntry = NULL;
GtkListBox * EntryList = NULL;
bool ShowSearch = false;

std::vector<Entry > FoundEntries;
Entry CurrentEntry;
std::vector<Entry > RememberedEntries;
DictionaryParse * RememberHandler = NULL;

GObject* MainWindow = NULL;

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

void SetEntry(Entry & entry)
{
    CurrentEntry = entry;

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

void RememberEntry(Entry & entry)
{
    RememberedEntries.push_back(entry);
    SaveDictionaryToFile(RememberedEntries, "remember.xml");
}

void next_clicked(GtkWidget *widget, gpointer data) 
{
    int num = (rand() % Handler->Dictionary.size());
    Entry & entry = Handler->Dictionary[num];
    SetEntry(entry);

//    std::cout << "Handler->Dictionary.size() == " << Handler->Dictionary.size() << std::endl;
}

void search_click(GtkWidget *widget, gpointer data)
{
    ShowSearch = !ShowSearch;
    if (ShowSearch)
        gtk_widget_show(GTK_WIDGET(SearchWindow));
    else
        gtk_widget_hide(GTK_WIDGET(SearchWindow));
}

void remember_click(GtkWidget *widget, gpointer data)
{
    RememberEntry(CurrentEntry);
}

void search_activate(GtkWidget *widget, gpointer data)
{
    int size = Handler->Dictionary.size();
    std::vector<Entry >::iterator it = Handler->Dictionary.begin();
    
    const gchar *text = gtk_entry_get_text(GTK_ENTRY(widget));

    FoundEntries.clear();

    GList *children, *iter;
    children = gtk_container_get_children(GTK_CONTAINER(EntryList));
    for(iter = children; iter != NULL; iter = g_list_next(iter))
        gtk_widget_destroy(GTK_WIDGET(iter->data));
    g_list_free(children);

    int i = 0;
    while (it != Handler->Dictionary.end())
    {
        Entry & entry = *it;

        for (int l = 0; l < entry.Glossary.size(); l++)
        {
            std::string gloss = entry.Glossary[l];
            if (gloss.find((char *)text) != string::npos) 
            {
                GtkLabel * caption = GTK_LABEL(gtk_label_new_with_mnemonic(gloss.c_str()));
                gtk_widget_set_size_request(GTK_WIDGET(caption), -1, 25);
                gtk_list_box_insert(EntryList, GTK_WIDGET(caption), -1);

                FoundEntries.push_back(entry);
                i++;
                break;
            }
        }
        it++;
        if (i > 100) break;
    }
    gtk_widget_show_all(GTK_WIDGET(EntryList));
}

void found_row_selected(GtkListBox* self, GtkListBoxRow* row, gpointer user_data)
{
    int idx = gtk_list_box_row_get_index(row);
    if (idx >= 0 && idx < FoundEntries.size())
    {
        SetEntry(FoundEntries[idx]);
    }
    gtk_window_present(GTK_WINDOW(MainWindow));
//    gtk_window_set_keep_above(GTK_WINDOW(MainWindow), TRUE);
}

int main(int argc, char** argv)
{
    srand(time(NULL));

    const char* xmlFile = "JMdict_e";
    std::cout << "argc:"  << argc << endl;
    if (argc == 2) 
    {
        xmlFile = argv[1];
        std::cout << "argv:"  << argv[1] << endl;
    }

    Handler = new DictionaryParse();
    FillDictionary(xmlFile, Handler);

    RememberHandler = new DictionaryParse();
    FillDictionary("remember.xml", RememberHandler);
    RememberedEntries = RememberHandler->Dictionary;


    gtk_init(&argc, &argv);

    // Create a builder object that will load the file.
    GtkBuilder* builder = gtk_builder_new();

    // Load the XML from a file.
    gtk_builder_add_from_file(builder, "GtkWindow.glade", NULL);

    // Get the object called 'main_window' from the file and show it.
    MainWindow = gtk_builder_get_object(builder, "main_window");
    gtk_widget_show(GTK_WIDGET(MainWindow));

//    gtk_window_set_default_size(GTK_WINDOW(window), 400, 200);

    GtkWidget * btnNext = GTK_WIDGET(gtk_builder_get_object(builder, "NextButton"));
    LabelKanji = GTK_LABEL(gtk_builder_get_object(builder, "KanjiLabel"));
    LabelRomaji = GTK_LABEL(gtk_builder_get_object(builder, "RomajiLabel"));
    LabelGloss1 = GTK_LABEL(gtk_builder_get_object(builder, "Gloss1"));
    LabelGloss2 = GTK_LABEL(gtk_builder_get_object(builder, "Gloss2"));
    LabelGloss3 = GTK_LABEL(gtk_builder_get_object(builder, "Gloss3"));

    SearchButton = GTK_BUTTON(gtk_builder_get_object(builder, "SearchButton"));
    SearchWindow = gtk_builder_get_object(builder, "SearchWindow");
    SearchEntry = GTK_ENTRY(gtk_builder_get_object(builder, "SearchEntry"));
    EntryList = GTK_LIST_BOX(gtk_builder_get_object(builder, "EntryList"));
    g_signal_connect(G_OBJECT(SearchButton), "clicked", G_CALLBACK(search_click), NULL);
    g_signal_connect(G_OBJECT(SearchEntry), "activate", G_CALLBACK(search_activate), NULL);
    g_signal_connect(G_OBJECT(EntryList), "row-selected", G_CALLBACK(found_row_selected), NULL);

    RememberButton = GTK_BUTTON(gtk_builder_get_object(builder, "RememberButton"));
    g_signal_connect(G_OBJECT(RememberButton), "clicked", G_CALLBACK(remember_click), NULL);

    // Connect the signal handlers defined in the glade file.
    // (Note: if you're looking for the c++ way to do this, there's no
    // support for binding C++ signal handlers. You must use 'extern
    // "C"' functions as handlers.)
//    int my_user_data = 0xDEADBEEF;
//    gtk_builder_connect_signals(builder, &my_user_data);


    g_signal_connect(G_OBJECT(btnNext), "clicked", G_CALLBACK(next_clicked), NULL);

    // Quit the app when the window is closed.
    g_signal_connect(MainWindow, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Main loop.
    gtk_main();

    return 0;
}

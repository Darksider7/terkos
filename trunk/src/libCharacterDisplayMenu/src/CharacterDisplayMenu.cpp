/*
 * Chris Bartley (bartley@cmu.edu)
 */

#include "CharacterDisplayMenu.h"

const string CharacterDisplayMenu::EMPTY_STRING = "";
const unsigned int CharacterDisplayMenu::DYNAMIC_OBJECT_ARGUMENT_ARRAY_SIZE = 4;

CharacterDisplayMenu::CharacterDisplayMenu(const string& xmlFilename, MenuStatusManager* menuStatusManager, CharacterDisplay* characterDisplay)
   {
   menuItem = NULL;

   // Turn off condensing of white space
   TiXmlBase::SetCondenseWhiteSpace(false);

   // First find the root element in the XML document
   ticpp::Element* rootElement = NULL;

   try
      {
      // Load a document
      ticpp::Document doc(xmlFilename);
      doc.LoadFile();

      cout << "CharacterDisplayMenu: Menu XML file loaded" << endl;

      // look for the <menu> element (FirstChild() might return the xml or doctype element)
      rootElement = doc.FirstChildElement(false);

      if (!rootElement || (rootElement->Value() != "menu") || rootElement->NoChildren())
         {
         cout << "ERROR: Invalid XML: The <menu> element is missing or contains no child elements!" << endl;
         rootElement = NULL;
         }

      cout << "CharacterDisplayMenu: building menu tree" << endl;

      // now build the tree
      if (rootElement != NULL)
         {
         welcomeText = rootElement->GetAttribute("welcome-text");
         menuItem = this->buildMenuItemTree(rootElement, menuStatusManager, characterDisplay);
         }
      }
   catch (ticpp::Exception& ex)
      {
      cout << "Exception while procesing the menu XML!" << endl;
      cout << ex.what() << endl;
      }
   }

MenuItemImpl* CharacterDisplayMenu::buildMenuItemTree(ticpp::Element* parentElement, MenuStatusManager* menuStatusManager,
                                                      CharacterDisplay* characterDisplay)
   {
   if (parentElement != NULL)
      {
      string text = parentElement->GetAttribute("text");

      cout << "CharacterDisplayMenu::buildMenuItemTree: processing element <" << parentElement->Value() << " text=\"" << text << "\">" << endl;

      bool hasItemChildElements = (NULL != parentElement->FirstChildElement("item", false));

      if (hasItemChildElements)
         {
         cout << "CharacterDisplayMenu::buildMenuItemTree: parent has <item> children" << endl;

         // recursively build the children
         vector<MenuItemImpl*> children;
         ticpp::Element* itemElement = NULL;
         for (itemElement = parentElement->FirstChildElement("item", false); itemElement; itemElement
                  = itemElement->NextSiblingElement("item", false))
            {

            children.push_back(this->buildMenuItemTree(itemElement, menuStatusManager, characterDisplay));
            }

         // create the MenuItem
         MenuItemImpl* menuItem = new MenuItemImpl(text, children);

         // create an instance of the MenuItemAction implementation class, or default
         // to the CharacterDisplayMenuItemAction upon failure
         MenuItemAction* menuItemAction = createMenuItemAction(parentElement, menuItem, menuStatusManager, characterDisplay);

         menuItem->setMenuItemAction(menuItemAction);

         // set up the parent and sibling references for each of the children
         unsigned int lastIndex = children.size() - 1;
         for (unsigned int menuIndex = 0; menuIndex < children.size(); menuIndex++)
            {
            MenuItem* previousSibling = children[menuIndex > 0 ? menuIndex - 1 : lastIndex];
            MenuItem* nextSibling = children[menuIndex < lastIndex ? menuIndex + 1 : 0];
            MenuItemImpl* child = children[menuIndex];
            child->setParent(menuItem);
            child->setPreviousSibling(previousSibling);
            child->setNextSibling(nextSibling);
            }

         return menuItem;
         }
      else
         {
         cout << "CharacterDisplayMenu::buildMenuItemTree: parent has NO <item> children" << endl;

         // create the MenuItem
         MenuItemImpl* menuItem = new MenuItemImpl(text);

         // create an instance of the MenuItemAction implementation class, or default
         // to the CharacterDisplayMenuItemAction upon failure
         MenuItemAction* menuItemAction = createMenuItemAction(parentElement, menuItem, menuStatusManager, characterDisplay);

         menuItem->setMenuItemAction(menuItemAction);

         return menuItem;
         }
      }

   return NULL;
   }

MenuItemAction* CharacterDisplayMenu::createMenuItemAction(ticpp::Element* parentElement, MenuItemImpl* menuItem,
                                                           MenuStatusManager* menuStatusManager, CharacterDisplay* characterDisplay)
   {
   // create an instance of the MenuItemAction implementation class, or default
   // to the CharacterDisplayMenuItemAction upon failure
   MenuItemAction* menuItemAction = NULL;
   ticpp::Element* implementationClassElement = parentElement->FirstChildElement("implementation-class", false);
   if (implementationClassElement != NULL)
      {
      // See if the <implementation-class> element has any <property> child
      // elements.  If so, store them in a map of strings.
      map<string, string> properties;
      ticpp::Element* propertyElement = NULL;
      for (propertyElement = implementationClassElement->FirstChildElement("property", false); propertyElement; propertyElement
               = propertyElement->NextSiblingElement("property", false))
         {
         string key = propertyElement->GetAttribute("key");

         // Use the element's text contents if the attribute isn't specified.
         string value;
         if (propertyElement->HasAttribute("value"))
            {
            value = propertyElement->GetAttribute("value");
            }
         else
            {
            value = propertyElement->GetText(false);
            }

         properties[key] = value;
         }

      // fetch the implementation class name
      string menuItemActionImplementationClassName = implementationClassElement->GetAttribute("name");

      // fetch the implementation class's library name (if not specified, default to the class name)
      string menuItemActionLibraryName = implementationClassElement->GetAttributeOrDefault("library", "lib" + menuItemActionImplementationClassName
               + ".so");

      // instantiate the class
      if (menuItemActionImplementationClassName != CharacterDisplayMenu::EMPTY_STRING)
         {
         cout << "impl class='" << menuItemActionImplementationClassName << "' and library='" << menuItemActionLibraryName << "'" << endl;

         menuItemAction
                  = this->instantiateMenuItemAction(menuItemActionLibraryName, menuItemActionImplementationClassName, menuItem, menuStatusManager, characterDisplay, properties);
         }
      }

   if (menuItemAction == NULL)
      {
      cout << "CharacterDisplayMenu::createMenuItemAction(): MenuItemAction implementation "
               << "unspecified or invalid, defaulting to using CharacterDisplayMenuItemAction" << endl;
      menuItemAction = new CharacterDisplayMenuItemAction(NULL, menuItem, menuStatusManager, characterDisplay);
      }

   return menuItemAction;
   }

MenuItemAction* CharacterDisplayMenu::instantiateMenuItemAction(const string& sharedLibraryName, const string& className, MenuItem* menuItem,
                                                                MenuStatusManager* menuStatusManager, CharacterDisplay* characterDisplay, map<string,
                                                                         string>& properties)
   {
   DynamicLibrary* dynamicLibrary = DynamicLoader::loadObjectFile(sharedLibraryName.c_str(), RTLD_NOW);

   if (dynamicLibrary == NULL)
      {
      cerr << "CharacterDisplayMenu::instantiateMenuItemAction: Couldn't load dynamic library [" << sharedLibraryName << "]" << endl;
      }
   else
      {
      cout << "CharacterDisplayMenu::instantiateMenuItemAction: Loaded dynamic library [" << sharedLibraryName << "]" << endl;

      // keep track of the created dynamic libraries so we can delete them in the destructor
      dynamicLibraries.push_back(dynamicLibrary);

      // create the argument array
      void* args[CharacterDisplayMenu::DYNAMIC_OBJECT_ARGUMENT_ARRAY_SIZE];
      args[0] = menuItem;
      args[1] = menuStatusManager;
      args[2] = characterDisplay;
      args[3] = &properties;

      // create the MenuItemAction
      MenuItemAction
               * menuItemAction =
                        dynamic_cast<MenuItemAction*> (dynamicLibrary->newObject(className.c_str(), CharacterDisplayMenu::DYNAMIC_OBJECT_ARGUMENT_ARRAY_SIZE, args));

      if (menuItemAction == NULL)
         {
         cerr << "Couldn't create MenuItemAction [" << className << "]" << endl;
         }

      return menuItemAction;
      }

   return NULL;
   }

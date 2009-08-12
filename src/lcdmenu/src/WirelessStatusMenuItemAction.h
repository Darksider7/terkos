/*
 * Chris Bartley (bartley@cmu.edu)
 */

#ifndef WIRELESSSTATUSMENUITEMACTION_H_
#define WIRELESSSTATUSMENUITEMACTION_H_

#include <algorithm>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <pstream.h>
#include <json/json.h>
#include <CharacterDisplayMenuItemAction.h>
#include <TwoOptionMenuItemAction.h>
#include <StringUtilities.h>
#include "WirelessStatusCheckingMenuItemAction.h"

using namespace std;
using namespace redi;

class WirelessStatusMenuItemAction : public WirelessStatusCheckingMenuItemAction
   {
   public:

      static const string CLASS_NAME;

      static const string STATUS_FAILURE_PROPERTY_ACTION_PROMPT;
      static const string STATUS_FAILURE_DEFAULT_ACTION_PROMPT;

      static const string STATUS_UNPLUGGED_PROPERTY_ACTION_PROMPT;
      static const string STATUS_UNPLUGGED_DEFAULT_ACTION_PROMPT;

      static const string WIRELESS_DISABLED_PROPERTY_ACTION_PROMPT;
      static const string WIRELESS_DISABLED_PROPERTY_ACTION_CHOSE_CANCEL;
      static const string WIRELESS_DISABLED_PROPERTY_CHOICE_OPTION1;
      static const string WIRELESS_DISABLED_PROPERTY_CHOICE_OPTION2;
      static const string WIRELESS_DISABLED_PROPERTY_ACTION_CHOSE_OPTION1;
      static const string WIRELESS_DISABLED_PROPERTY_ACTION_CHOSE_OPTION2;

      static const string WIRELESS_DISABLED_DEFAULT_ACTION_PROMPT;
      static const string WIRELESS_DISABLED_DEFAULT_ACTION_CHOSE_CANCEL;
      static const string WIRELESS_DISABLED_DEFAULT_LABEL_OPTION1;
      static const string WIRELESS_DISABLED_DEFAULT_LABEL_OPTION2;
      static const string WIRELESS_DISABLED_DEFAULT_ACTION_CHOSE_OPTION1;
      static const string WIRELESS_DISABLED_DEFAULT_ACTION_CHOSE_OPTION2;

      static const string WIRELESS_ENABLED_PROPERTY_ACTION_PROMPT;
      static const string WIRELESS_ENABLED_PROPERTY_ACTION_CHOSE_CANCEL;
      static const string WIRELESS_ENABLED_PROPERTY_CHOICE_OPTION1;
      static const string WIRELESS_ENABLED_PROPERTY_CHOICE_OPTION2;
      static const string WIRELESS_ENABLED_PROPERTY_ACTION_CHOSE_OPTION2;
      static const string WIRELESS_ENABLED_PROPERTY_OPTION1_FAILURE;

      static const string WIRELESS_ENABLED_DEFAULT_ACTION_PROMPT;
      static const string WIRELESS_ENABLED_DEFAULT_ACTION_CHOSE_CANCEL;
      static const string WIRELESS_ENABLED_DEFAULT_LABEL_OPTION1;
      static const string WIRELESS_ENABLED_DEFAULT_LABEL_OPTION2;
      static const string WIRELESS_ENABLED_DEFAULT_ACTION_CHOSE_OPTION2;
      static const string WIRELESS_ENABLED_DEFAULT_OPTION1_FAILURE;

      WirelessStatusMenuItemAction(void(*delObj)(void*), MenuItem* menuItem, MenuStatusManager* menuStatusManager,
                                   CharacterDisplay* characterDisplay, map<string, string>& properties) :
         WirelessStatusCheckingMenuItemAction(delObj, menuItem, menuStatusManager, characterDisplay, properties)
         {
         map<string, string> wirelessDisabledProperties;
         wirelessDisabledProperties[TwoOptionMenuItemAction::PROPERTY_ACTION_PROMPT]
                  = getProperty(WIRELESS_DISABLED_PROPERTY_ACTION_PROMPT, WIRELESS_DISABLED_DEFAULT_ACTION_PROMPT);
         wirelessDisabledProperties[TwoOptionMenuItemAction::PROPERTY_ACTION_CHOSE_CANCEL]
                  = getProperty(WIRELESS_DISABLED_PROPERTY_ACTION_CHOSE_CANCEL, WIRELESS_DISABLED_DEFAULT_ACTION_CHOSE_CANCEL);
         wirelessDisabledProperties[TwoOptionMenuItemAction::PROPERTY_CHOICE_OPTION1]
                  = getProperty(WIRELESS_DISABLED_PROPERTY_CHOICE_OPTION1, WIRELESS_DISABLED_DEFAULT_LABEL_OPTION1);
         wirelessDisabledProperties[TwoOptionMenuItemAction::PROPERTY_CHOICE_OPTION2]
                  = getProperty(WIRELESS_DISABLED_PROPERTY_CHOICE_OPTION2, WIRELESS_DISABLED_DEFAULT_LABEL_OPTION2);
         wirelessDisabledProperties[TwoOptionMenuItemAction::PROPERTY_ACTION_CHOSE_OPTION1]
                  = getProperty(WIRELESS_DISABLED_PROPERTY_ACTION_CHOSE_OPTION1, WIRELESS_DISABLED_DEFAULT_ACTION_CHOSE_OPTION1);
         wirelessDisabledProperties[TwoOptionMenuItemAction::PROPERTY_ACTION_CHOSE_OPTION2]
                  = getProperty(WIRELESS_DISABLED_PROPERTY_ACTION_CHOSE_OPTION2, WIRELESS_DISABLED_DEFAULT_ACTION_CHOSE_OPTION2);

         map<string, string> wirelessEnabledProperties;
         wirelessEnabledProperties[TwoOptionMenuItemAction::PROPERTY_ACTION_PROMPT]
                  = getProperty(WIRELESS_ENABLED_PROPERTY_ACTION_PROMPT, WIRELESS_ENABLED_DEFAULT_ACTION_PROMPT);
         wirelessEnabledProperties[TwoOptionMenuItemAction::PROPERTY_ACTION_CHOSE_CANCEL]
                  = getProperty(WIRELESS_ENABLED_PROPERTY_ACTION_CHOSE_CANCEL, WIRELESS_ENABLED_DEFAULT_ACTION_CHOSE_CANCEL);
         wirelessEnabledProperties[TwoOptionMenuItemAction::PROPERTY_CHOICE_OPTION1]
                  = getProperty(WIRELESS_ENABLED_PROPERTY_CHOICE_OPTION1, WIRELESS_ENABLED_DEFAULT_LABEL_OPTION1);
         wirelessEnabledProperties[TwoOptionMenuItemAction::PROPERTY_CHOICE_OPTION2]
                  = getProperty(WIRELESS_ENABLED_PROPERTY_CHOICE_OPTION2, WIRELESS_ENABLED_DEFAULT_LABEL_OPTION2);
         wirelessEnabledProperties[TwoOptionMenuItemAction::PROPERTY_ACTION_CHOSE_OPTION2]
                  = getProperty(WIRELESS_ENABLED_PROPERTY_ACTION_CHOSE_OPTION2, WIRELESS_ENABLED_DEFAULT_ACTION_CHOSE_OPTION2);
         wirelessEnabledProperties[WIRELESS_ENABLED_PROPERTY_OPTION1_FAILURE]
                  = getProperty(WIRELESS_ENABLED_PROPERTY_OPTION1_FAILURE, WIRELESS_ENABLED_DEFAULT_OPTION1_FAILURE);

         wirelessFailureMenuItemAction
                  = new NoOpMenuItemAction(getProperty(STATUS_FAILURE_PROPERTY_ACTION_PROMPT, STATUS_FAILURE_DEFAULT_ACTION_PROMPT), NULL, menuItem, menuStatusManager, characterDisplay, properties);
         wirelessUnpluggedMenuItemAction
                  = new NoOpMenuItemAction(getProperty(STATUS_UNPLUGGED_PROPERTY_ACTION_PROMPT, STATUS_UNPLUGGED_DEFAULT_ACTION_PROMPT), NULL, menuItem, menuStatusManager, characterDisplay, properties);
         wirelessDisabledMenuItemAction
                  = new WirelessDisabledMenuItemAction(NULL, menuItem, menuStatusManager, characterDisplay, wirelessDisabledProperties);
         wirelessEnabledMenuItemAction
                  = new WirelessEnabledMenuItemAction(this, NULL, menuItem, menuStatusManager, characterDisplay, wirelessEnabledProperties);
         }

      virtual ~WirelessStatusMenuItemAction()
         {
         currentMenuItemAction = NULL;
         delete wirelessFailureMenuItemAction;
         delete wirelessUnpluggedMenuItemAction;
         delete wirelessDisabledMenuItemAction;
         delete wirelessEnabledMenuItemAction;
         }

      void activate();
      void start();
      void stop();
      void upEvent();
      void downEvent();
      void rightEvent();
      void leftEvent();

   protected:

      virtual void handleWirelessEnabled()
         {
         currentMenuItemAction = wirelessEnabledMenuItemAction;
         }

      virtual void handleWirelessDisabled()
         {
         currentMenuItemAction = wirelessDisabledMenuItemAction;
         }

      virtual void handleWirelessUnplugged()
         {
         currentMenuItemAction = wirelessUnpluggedMenuItemAction;
         }

      virtual void handleWirelessStatusFailure()
         {
         currentMenuItemAction = wirelessFailureMenuItemAction;
         }

   private:

      MenuItemAction* currentMenuItemAction;
      MenuItemAction* wirelessFailureMenuItemAction;
      MenuItemAction* wirelessUnpluggedMenuItemAction;
      MenuItemAction* wirelessDisabledMenuItemAction;
      MenuItemAction* wirelessEnabledMenuItemAction;

      friend class WirelessEnabledMenuItemAction;

      class NoOpMenuItemAction : public CharacterDisplayMenuItemAction
         {
         public:

            NoOpMenuItemAction(string message, void(*delObj)(void*), MenuItem* menuItem, MenuStatusManager* menuStatusManager,
                               CharacterDisplay* characterDisplay, map<string, string>& properties) :
               CharacterDisplayMenuItemAction(delObj, menuItem, menuStatusManager, characterDisplay, properties), message(message)
               {
               // nothing to do
               }

            virtual ~NoOpMenuItemAction()
               {
               // nothing to do
               }

            void activate()
               {
               getCharacterDisplay()->setText(message);
               }

            void start()
               {
               // start and stop should do the same thing, so just call stop()
               stop();
               }

            void stop()
               {
               // call stop on CharacterDisplayMenuItemAction so that we pop back up to the parent menu item
               CharacterDisplayMenuItemAction::stop();
               }

            void upEvent()
               {
               // do nothing, just swallow the event
               }

            void downEvent()
               {
               // do nothing, just swallow the event
               }

         private:

            string message;

         };

      class WirelessDisabledMenuItemAction : public TwoOptionMenuItemAction
         {
         public:

            WirelessDisabledMenuItemAction(void(*delObj)(void*), MenuItem* menuItem, MenuStatusManager* menuStatusManager,
                                           CharacterDisplay* characterDisplay, map<string, string>& properties) :
               TwoOptionMenuItemAction(delObj, menuItem, menuStatusManager, characterDisplay, properties)
               {
               // nothing to do
               }

            virtual ~WirelessDisabledMenuItemAction()
               {
               // nothing to do
               }

         protected:

            bool shouldOption1BeSelectedUponActivation() const;
            void executeOption1Action();
            void executeOption2Action();

         };

      class WirelessEnabledMenuItemAction : public TwoOptionMenuItemAction
         {
         public:

            WirelessEnabledMenuItemAction(WirelessStatusMenuItemAction* parentMenuItemAction, void(*delObj)(void*), MenuItem* menuItem,
                                          MenuStatusManager* menuStatusManager, CharacterDisplay* characterDisplay, map<string, string>& properties) :
               TwoOptionMenuItemAction(delObj, menuItem, menuStatusManager, characterDisplay, properties), parentMenuItemAction(parentMenuItemAction)
               {
               // nothing to do
               }

            virtual ~WirelessEnabledMenuItemAction()
               {
               // nothing to do
               }

            void activate()
               {
               TwoOptionMenuItemAction::activate();
               isOnChoiceScreen = true;
               }

            void start()
               {
               if (isOnChoiceScreen)
                  {
                  TwoOptionMenuItemAction::start();
                  }
               else
                  {
                  activate();
                  }
               }

            void stop()
               {
               if (isOnChoiceScreen)
                  {
                  TwoOptionMenuItemAction::stop();
                  }
               else
                  {
                  activate();
                  }
               }

            void upEvent()
               {
               if (isOnChoiceScreen)
                  {
                  TwoOptionMenuItemAction::upEvent();
                  }
               }

            void downEvent()
               {
               if (isOnChoiceScreen)
                  {
                  TwoOptionMenuItemAction::downEvent();
                  }
               }

            void rightEvent()
               {
               if (isOnChoiceScreen)
                  {
                  TwoOptionMenuItemAction::rightEvent();
                  }
               }

            void leftEvent()
               {
               if (isOnChoiceScreen)
                  {
                  TwoOptionMenuItemAction::leftEvent();
                  }
               }

         protected:

            bool shouldOption1BeSelectedUponActivation() const;
            void executeOption1Action();
            void executeOption2Action();

            virtual bool shouldDisplayTextThenExitToParentWhenAction1IsComplete()
               {
               return false;
               }

            virtual bool shouldDisplayTextThenExitToParentWhenAction2IsComplete()
               {
               return true;
               }

            virtual int getMillisecondsToSleepBeforeExitingToParentAfterAction1IsComplete()
               {
               return 0;
               }

            virtual int getMillisecondsToSleepBeforeExitingToParentAfterCancel()
               {
               return 0;
               }

         private:

            WirelessStatusMenuItemAction* parentMenuItemAction;
            bool isOnChoiceScreen;

         };
   };

#endif /* WIRELESSSTATUSMENUITEMACTION_H_ */
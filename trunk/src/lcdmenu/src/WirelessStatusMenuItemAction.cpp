/*
 * Chris Bartley (bartley@cmu.edu)
 */

#include "WirelessStatusMenuItemAction.h"

const string WirelessStatusMenuItemAction::CLASS_NAME = "WirelessStatusMenuItemAction";

const string WirelessStatusMenuItemAction::STATUS_FAILURE_PROPERTY_ACTION_PROMPT = "status.failure";
const string WirelessStatusMenuItemAction::STATUS_FAILURE_DEFAULT_ACTION_PROMPT = "Failed to get   wireless status.";

const string WirelessStatusMenuItemAction::STATUS_UNPLUGGED_PROPERTY_ACTION_PROMPT = "status.unplugged";
const string WirelessStatusMenuItemAction::STATUS_UNPLUGGED_DEFAULT_ACTION_PROMPT = "Wireless adapteris unplugged.";

const string WirelessStatusMenuItemAction::WIRELESS_DISABLED_PROPERTY_ACTION_PROMPT = "wireless-disabled.action.prompt";
const string WirelessStatusMenuItemAction::WIRELESS_DISABLED_PROPERTY_ACTION_CHOSE_CANCEL = "wireless-disabled.action.cancel";
const string WirelessStatusMenuItemAction::WIRELESS_DISABLED_PROPERTY_CHOICE_OPTION1 = "wireless-disabled.choice.option1";
const string WirelessStatusMenuItemAction::WIRELESS_DISABLED_PROPERTY_CHOICE_OPTION2 = "wireless-disabled.choice.option2";
const string WirelessStatusMenuItemAction::WIRELESS_DISABLED_PROPERTY_ACTION_CHOSE_OPTION1 = "wireless-disabled.action.option1";
const string WirelessStatusMenuItemAction::WIRELESS_DISABLED_PROPERTY_OPTION1_FAILURE = "wireless-disabled.option1.failure";
const string WirelessStatusMenuItemAction::WIRELESS_DISABLED_PROPERTY_ACTION_CHOSE_OPTION1_BEFORE = "wireless-disabled.action.option1.before";
const string WirelessStatusMenuItemAction::WIRELESS_DISABLED_DEFAULT_ACTION_PROMPT = "Wireless is off.";
const string WirelessStatusMenuItemAction::WIRELESS_DISABLED_DEFAULT_ACTION_CHOSE_CANCEL = "Cancelled -- No settings changed";
const string WirelessStatusMenuItemAction::WIRELESS_DISABLED_DEFAULT_LABEL_OPTION1 = "Start   ";
const string WirelessStatusMenuItemAction::WIRELESS_DISABLED_DEFAULT_LABEL_OPTION2 = "OK";
const string WirelessStatusMenuItemAction::WIRELESS_DISABLED_DEFAULT_ACTION_CHOSE_OPTION1 = "Wireless is now turned on.";
const string WirelessStatusMenuItemAction::WIRELESS_DISABLED_DEFAULT_OPTION1_FAILURE = "Failed to start wireless.";
const string WirelessStatusMenuItemAction::WIRELESS_DISABLED_DEFAULT_ACTION_CHOSE_OPTION1_BEFORE = "Starting        wireless...";

const string WirelessStatusMenuItemAction::WIRELESS_ENABLED_PROPERTY_ACTION_PROMPT = "wireless-enabled.action.prompt";
const string WirelessStatusMenuItemAction::WIRELESS_ENABLED_PROPERTY_ACTION_CHOSE_CANCEL = "wireless-enabled.action.cancel";
const string WirelessStatusMenuItemAction::WIRELESS_ENABLED_PROPERTY_CHOICE_OPTION1 = "wireless-enabled.choice.option1";
const string WirelessStatusMenuItemAction::WIRELESS_ENABLED_PROPERTY_CHOICE_OPTION2 = "wireless-enabled.choice.option2";
const string WirelessStatusMenuItemAction::WIRELESS_ENABLED_PROPERTY_ACTION_CHOSE_OPTION2 = "wireless-enabled.action.option2";
const string WirelessStatusMenuItemAction::WIRELESS_ENABLED_PROPERTY_OPTION1_FAILURE = "wireless-enabled.option1.failure";
const string WirelessStatusMenuItemAction::WIRELESS_ENABLED_PROPERTY_OPTION2_FAILURE = "wireless-enabled.option2.failure";
const string WirelessStatusMenuItemAction::WIRELESS_ENABLED_PROPERTY_ACTION_CHOSE_OPTION2_BEFORE = "wireless-enabled.action.option2.before";
const string WirelessStatusMenuItemAction::WIRELESS_ENABLED_DEFAULT_ACTION_PROMPT = "Wireless is on.";
const string WirelessStatusMenuItemAction::WIRELESS_ENABLED_DEFAULT_ACTION_CHOSE_CANCEL = "Cancelled -- No settings changed";
const string WirelessStatusMenuItemAction::WIRELESS_ENABLED_DEFAULT_LABEL_OPTION1 = "Info  ";
const string WirelessStatusMenuItemAction::WIRELESS_ENABLED_DEFAULT_LABEL_OPTION2 = "Stop";
const string WirelessStatusMenuItemAction::WIRELESS_ENABLED_DEFAULT_ACTION_CHOSE_OPTION2 = "Wireless is now turned off.";
const string WirelessStatusMenuItemAction::WIRELESS_ENABLED_DEFAULT_OPTION1_FAILURE = "Could not obtainwireless details";
const string WirelessStatusMenuItemAction::WIRELESS_ENABLED_DEFAULT_OPTION2_FAILURE = "Failed to stop  wireless.";
const string WirelessStatusMenuItemAction::WIRELESS_ENABLED_DEFAULT_ACTION_CHOSE_OPTION2_BEFORE = "Stopping        wireless...";

void WirelessStatusMenuItemAction::activate()
   {
   WirelessStatusCheckingMenuItemAction::activate();
   currentMenuItemAction->activate();
   }

void WirelessStatusMenuItemAction::start()
   {
   currentMenuItemAction->start();
   }

void WirelessStatusMenuItemAction::stop()
   {
   currentMenuItemAction->stop();
   }

void WirelessStatusMenuItemAction::upEvent()
   {
   currentMenuItemAction->upEvent();
   }

void WirelessStatusMenuItemAction::downEvent()
   {
   currentMenuItemAction->downEvent();
   }

void WirelessStatusMenuItemAction::rightEvent()
   {
   currentMenuItemAction->rightEvent();
   }

void WirelessStatusMenuItemAction::leftEvent()
   {
   currentMenuItemAction->leftEvent();
   }

// ---------------------------------------------------------------------------------------------------------------------

bool WirelessStatusMenuItemAction::WirelessDisabledMenuItemAction::shouldOption1BeSelectedUponActivation() const
   {
   return true;
   }

void WirelessStatusMenuItemAction::WirelessDisabledMenuItemAction::executeOption1Action()
   {
   isOnChoiceScreen = false;

   getCharacterDisplay()->setText(getProperty(WIRELESS_DISABLED_PROPERTY_ACTION_CHOSE_OPTION1_BEFORE, WIRELESS_DISABLED_DEFAULT_ACTION_CHOSE_OPTION1_BEFORE));

   // call the Perl script which enables wireless
   try
      {
      // execute the script and return the results as a stream
      redi::ipstream is("perl -I/opt/scripts /opt/scripts/enableWirelessNetworking.pl");

      // parse the stream
      Json::Reader reader;
      Json::Value wirelessNetworkingStatusJson;
      bool parsingSuccessful = reader.parse(is, wirelessNetworkingStatusJson, true);
      is.close();

      // make sure the enabling was successful
      if (parsingSuccessful)
         {
         if (wirelessNetworkingStatusJson != Json::Value::null)
            {
            Json::Value isInstalled = wirelessNetworkingStatusJson["wireless-networking-status"]["is-installed"];
            if (isInstalled != Json::Value::null)
               {
               if (isInstalled.asBool())
                  {
                  Json::Value wirelessInterface = wirelessNetworkingStatusJson["wireless-networking-status"]["wireless-interface"];
                  if (wirelessInterface != Json::Value::null)
                     {
                     Json::Value isEnabled = wirelessInterface["is-enabled"];
                     if (isEnabled != Json::Value::null && isEnabled.asBool())
                        {
                        getCharacterDisplay()->setText(getProperty(WIRELESS_DISABLED_PROPERTY_ACTION_CHOSE_OPTION1, WIRELESS_DISABLED_DEFAULT_ACTION_CHOSE_OPTION1));
                        sleepThenPopUpToParentMenuItem(TwoOptionMenuItemAction::DEFAULT_MILLISECONDS_TO_SLEEP);

                        return;
                        }
                     }
                  }
               }
            }
         }
      else
         {
         cerr << "WirelessDisabledMenuItemAction::executeOption1Action(): failed to parse wireless networking status JSON:" << endl
                  << reader.getFormatedErrorMessages();
         }
      }
   catch (...)
      {
      cerr << "WirelessDisabledMenuItemAction::executeOption1Action(): failed to call script to enable wireless networking." << endl;
      }

   getCharacterDisplay()->setText(getProperty(WIRELESS_DISABLED_PROPERTY_OPTION1_FAILURE, WIRELESS_DISABLED_DEFAULT_OPTION1_FAILURE));
   sleepThenPopUpToParentMenuItem(TwoOptionMenuItemAction::DEFAULT_MILLISECONDS_TO_SLEEP);
   }

void WirelessStatusMenuItemAction::WirelessDisabledMenuItemAction::executeOption2Action()
   {
   isOnChoiceScreen = false;
   }

// ---------------------------------------------------------------------------------------------------------------------

bool WirelessStatusMenuItemAction::WirelessEnabledMenuItemAction::shouldOption1BeSelectedUponActivation() const
   {
   return true;
   }

void WirelessStatusMenuItemAction::WirelessEnabledMenuItemAction::executeOption1Action()
   {
   isOnChoiceScreen = false;
   Json::Value status = parentMenuItemAction->getWirelessNetworkingStatus();
   Json::Value accessPointJson = status["wireless-networking-status"]["wireless-interface"]["access-point"];
   if (accessPointJson != Json::Value::null)
      {
      getCharacterDisplay()->setLine(0, accessPointJson["ssid"].asString());
      getCharacterDisplay()->setLine(1, accessPointJson["ip-address"].asString());
      }
   else
      {
      getCharacterDisplay()->setText(getProperty(WIRELESS_ENABLED_PROPERTY_OPTION1_FAILURE, WIRELESS_ENABLED_DEFAULT_OPTION1_FAILURE));
      }
   }

void WirelessStatusMenuItemAction::WirelessEnabledMenuItemAction::executeOption2Action()
   {
   getCharacterDisplay()->setText(getProperty(WIRELESS_ENABLED_PROPERTY_ACTION_CHOSE_OPTION2_BEFORE, WIRELESS_ENABLED_DEFAULT_ACTION_CHOSE_OPTION2_BEFORE));

   isOnChoiceScreen = false;

   // call the Perl script which disables wireless
   try
      {
      // execute the script and return the results as a stream
      redi::ipstream is("perl -I/opt/scripts /opt/scripts/disableWirelessNetworking.pl");

      // In this case, we don't really care about the returned JSON status.  Just display
      // a message to the user and then pop up to the parent
      is.close();

      getCharacterDisplay()->setText(getProperty(WIRELESS_ENABLED_PROPERTY_ACTION_CHOSE_OPTION2, WIRELESS_ENABLED_DEFAULT_ACTION_CHOSE_OPTION2));
      }
   catch (...)
      {
      cerr << "WirelessEnabledMenuItemAction::executeOption2Action(): failed to call script to disable wireless networking." << endl;
      getCharacterDisplay()->setText(getProperty(WIRELESS_ENABLED_PROPERTY_OPTION2_FAILURE, WIRELESS_ENABLED_DEFAULT_OPTION2_FAILURE));
      }

   sleepThenPopUpToParentMenuItem(TwoOptionMenuItemAction::DEFAULT_MILLISECONDS_TO_SLEEP);
   }
// ---------------------------------------------------------------------------------------------------------------------

// required definitions for dynamic loading
extern "C"
   {
   void deleteWirelessStatusMenuItemActionObject(void* obj)
      {
      delete reinterpret_cast<DynamicObject*> (obj);
      }

   void* loadWirelessStatusMenuItemActionObject(const char* name, int argc, void** argv)
      {
      int numCharsToCompare = std::min(strlen(name), strlen(WirelessStatusMenuItemAction::CLASS_NAME.c_str()));
      if (strncmp(name, WirelessStatusMenuItemAction::CLASS_NAME.c_str(), numCharsToCompare) == 0)
         {
         map<string, string> properties;
         return new WirelessStatusMenuItemAction(deleteWirelessStatusMenuItemActionObject, (MenuItem*) argv[0], (MenuStatusManager*) argv[1], (CharacterDisplay*) argv[2], *((map<
                  string, string>*) argv[3]));
         }

      return NULL;
      }
   }

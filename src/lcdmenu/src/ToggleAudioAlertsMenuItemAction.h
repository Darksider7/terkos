//
// This file is part of Terk and TerkOS.
//
// All Terk and TerkOS source code is provided under the terms of the
// GNU General Public License v2 (http://www.gnu.org/licenses/gpl-2.0.html).
// Those wishing to use Terk and TerkOS source code, software and/or
// technologies under different licensing terms should contact us at
// telepresencerobotkit@cs.cmu.edu. Such licensing terms are available for
// all portions of the Terk and TerkOS codebase presented here.
//

/*
 * Chris Bartley (bartley@cmu.edu)
 */

#ifndef TOGGLEAUDIOALERTSMENUITEMACTION_H_
#define TOGGLEAUDIOALERTSMENUITEMACTION_H_

#include "TwoOptionMenuItemAction.h"
#include "AudioConfigManager.h"

class ToggleAudioAlertsMenuItemAction : public TwoOptionMenuItemAction
   {
   public:

      static const string CLASS_NAME;

      ToggleAudioAlertsMenuItemAction(void(*delObj)(void*), MenuItem* menuItem, MenuStatusManager* menuStatusManager,
                                      CharacterDisplay* characterDisplay, map<string, string>& properties) :
         TwoOptionMenuItemAction(delObj, menuItem, menuStatusManager, characterDisplay, properties)
         {
         // nothing to do
         }

      virtual ~ToggleAudioAlertsMenuItemAction()
         {
         // nothing to do
         }

   protected:

      bool shouldOption1BeSelectedUponActivation() const;

      void executeOption1Action();

      void executeOption2Action();

   private:

      AudioConfigManager configManager;
      bool areAlertsEnabled;

      void setAlertsEnabled(const bool isAlertsEnabled);
   };

#endif /* TOGGLEAUDIOALERTSMENUITEMACTION_H_ */

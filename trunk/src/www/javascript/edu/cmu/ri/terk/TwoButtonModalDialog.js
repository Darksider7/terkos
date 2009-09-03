//======================================================================================================================
// Class for a two-button modal dialog.
//
// Dependencies:
// * jQuery (http://jquery.com/)
// * jQuery UI (http://jqueryui.com/)
// * jquery.mousecapture.js (http://benanne.net/code/?p=238)
// * main.css
//
// Author: Chris Bartley (bartley@cmu.edu)
//======================================================================================================================

//======================================================================================================================
// VERIFY NAMESPACE
//======================================================================================================================
// Create the global symbol "edu" if it doesn't exist.  Throw an error if it does exist but is not an object.
var edu;
if (!edu)
   {
   edu = {};
   }
else if (typeof edu != "object")
   {
   var eduExistsMessage = "Error: failed to create edu namespace: edu already exists and is not an object";
   alert(eduExistsMessage);
   throw new Error(eduExistsMessage);
   }

// Repeat the creation and type-checking for the next level
if (!edu.cmu)
   {
   edu.cmu = {};
   }
else if (typeof edu.cmu != "object")
   {
   var eduCmuExistsMessage = "Error: failed to create edu.cmu namespace: edu.cmu already exists and is not an object";
   alert(eduCmuExistsMessage);
   throw new Error(eduCmuExistsMessage);
   }

// Repeat the creation and type-checking for the next level
if (!edu.cmu.ri)
   {
   edu.cmu.ri = {};
   }
else if (typeof edu.cmu.ri != "object")
   {
   var eduCmuRiExistsMessage = "Error: failed to create edu.cmu.ri namespace: edu.cmu.ri already exists and is not an object";
   alert(eduCmuRiExistsMessage);
   throw new Error(eduCmuRiExistsMessage);
   }

// Repeat the creation and type-checking for the next level
if (!edu.cmu.ri.terk)
   {
   edu.cmu.ri.terk = {};
   }
else if (typeof edu.cmu.ri.terk != "object")
   {
   var eduCmuRiTerkExistsMessage = "Error: failed to create edu.cmu.ri.terk namespace: edu.cmu.ri.terk already exists and is not an object";
   alert(eduCmuRiTerkExistsMessage);
   throw new Error(eduCmuRiTerkExistsMessage);
   }
//======================================================================================================================

//======================================================================================================================
// DEPENDECIES
//======================================================================================================================
if (!window['$'])
   {
   var noJQuery = "The jQuery library is required by edu.cmu.ri.terk.TwoButtonModalDialog.js";
   alert(noJQuery);
   throw new Error(noJQuery);
   }
//======================================================================================================================

//======================================================================================================================
// CODE
//======================================================================================================================
(function()
   {
   // ==================================================================================================================
   var jQuery = window['$'];

   edu.cmu.ri.terk.TwoButtonModalDialog = function(title, message, buttonLabel1, buttonLabel2)
      {
      var eventListeners = new Array();
      var dialog = jQuery('<div></div>');
      var dialogMessage = jQuery('<div>' + message + '</div>');
      var button1 = jQuery('<button type="button" style="float:right" class="ui-state-default ui-corner-all button text-button state-has-no-dependency">' + buttonLabel1 + '</button>');
      var button2 = jQuery('<button type="button" style="float:right" class="ui-state-default ui-corner-all button text-button state-has-no-dependency">' + buttonLabel2 + '</button>');

      jQuery(dialog).append(dialogMessage);
      jQuery(dialog).append('<div class="ui-widget-content divider" style="margin-top: 50px"></div>');
      jQuery(dialog).append(button1);
      jQuery(dialog).append(button2);

      jQuery(dialog)['dialog']({
         bgiframe: true,
         autoOpen: false,
         modal: true,
         draggable: false,
         resizable: false,
         closeOnEscape : false,
         title : title,
         dialogClass: 'non_closable_alert'});

      jQuery(button1).mousecapture({
         "down": function()
            {
            jQuery(this).addClass('ui-state-active');
            },
         "up": function()
            {
            jQuery(this).removeClass('ui-state-active');
            }
      }).click(
            function()
               {
               notifyEventListeners('onButton1Click');
               });

      jQuery(button2).mousecapture({
         "down": function()
            {
            jQuery(this).addClass('ui-state-active');
            },
         "up": function()
            {
            jQuery(this).removeClass('ui-state-active');
            }
      }).click(
            function()
               {
               notifyEventListeners('onButton2Click');
               });

      this.show = function()
         {
         notifyEventListeners('onBeforeShow');

         // Show the dialog and set the min-height.  I don't know why, but I *had* to set the min-height
         // here...setting it in the HTML or on load didn't work--it kept getting overwritten to
         // something like 112 or 114 pixels.
         jQuery(dialog)['dialog']('open').css('min-height', '1px');

         notifyEventListeners('onAfterShow');
         };

      this.hide = function()
         {
         notifyEventListeners('onBeforeHide');
         jQuery(dialog)['dialog']('close');
         notifyEventListeners('onAfterHide');
         };

      this.setMessage = function(message)
         {
         jQuery(dialogMessage).html(message);
         };

      /**
       * Registers the given event listener.  The event listener should implement some or all of the following methods:
       *
       *    onBeforeShow()
       *    onAfterShow()
       *    onButton1Click()
       *    onButton2Click()
       *    onBeforeHide()
       *    onAfterHide()
       */
      this.addEventListener = function(listener)
         {
         if (listener)
            {
            eventListeners[eventListeners.length] = listener;
            }
         };

      var notifyEventListeners = function(eventName)
         {
         jQuery.each(eventListeners, function(i, listener)
            {
            if (listener && listener[eventName])
               {
               listener[eventName]();
               }
            });
         };

      // ---------------------------------------------------------------------------------------------------------------
      };

   // ==================================================================================================================
   })();

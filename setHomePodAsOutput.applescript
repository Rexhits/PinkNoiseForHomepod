-- Script to set sound output - "Internal Speakers" hardcoded as I just keep multiple scripts for each output source (e.g. Internal Speakers, Airplay, bluetooth ) and use it with a quicklaunch app such as quicksilver.
-- Based off of Pierre L's answer in https://discussions.apple.com/thread/4629093?tstart=0 , which includes a selection dialog

set asrc to "Wang's HomePod"

tell application "System Preferences"

	reveal anchor "output" of pane id "com.apple.preference.sound"
	activate
	
	tell application "System Events"
		tell process "System Preferences"
			select (row 1 of table 1 of scroll area 1 of tab group 1 of window "Sound" whose value of text field 1 is asrc)
		end tell
	end tell
	
	quit
	
end tell
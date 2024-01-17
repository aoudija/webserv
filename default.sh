# defaults write -g AppleInterfaceStyle Dark && defaults write -g InitialKeyRepeat -int 15 && defaults write -g KeyRepeat -int 2
osascript -e 'tell application "System Preferences" to activate' -e 'delay 1'
 -e 'tell application "System Events" to tell process "System Preferences" to keystroke "Keyboard"' 
 -e 'delay 1' -e 'tell application "System Events" to tell process "System Preferences" to click button 
 "Keyboard" of window "Keyboard"' -e 'delay 1' -e 'tell application "System Events" to tell process 
 "System Preferences" to tell window "Keyboard" to set value of slider "Key Repeat" of tab group 1 to 2'
 -e 'delay 1' -e 'tell application "System Events" to tell process "System Preferences" to tell window "Keyboard" to set value of slider "Delay Until Repeat" of tab group 1 to 15'
//osascript -e 'tell application "Google Chrome" to open location "http://localhost/VIKINGS.S4E12.1080p.mp4"'
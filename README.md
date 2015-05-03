# ScriptR
Simple Script Engine for the popular video game "Maple Story"
http://maplestory.nexon.net/

![My image](http://i.imgur.com/jtpqamP.png)

Short Guide on writing scripts for this...


How to format scripts..
[Hotkey]
Command
..
..
 
[Hotkey]
Command
..
..
 
[Hotkey]
Command
...
..
..
 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
http://msdn.microsoft.com/en-us/library/windows/desktop/dd375731(v=vs.85).aspx
Hotkeys: use the 0x values from the link. Example: F1 is 0x70
 
so to define a script that will be toggled with the F1 key..
[0x70]
Command
Command
Command
..
..
..
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
REFER TO THE LINK FOR ALL KEYS
 
Current Commands available:
autohp:
        Usage: auto heals your hp when it reaches a specified value
        Format: autohp potion_key hp_value_to_pot_at
        Example usage:
                An example of a command that will..
                Hotkey(on/off) toggle: F1 (refer to vk link for 0x value)
                potion_key : delete key (refer to vk link for 0x value)
                hp_value_to_pot_at: 7000
               
                [0x70]
                autohp 0x2E 7000
========================================================================================================================
autohp:
        Usage: auto heals your mana when it reaches a specified value
        Format: automp potion_key hp_value_to_pot_at
        Example usage:
                samething as hp variant.. just change the values around for your mana key and mana value...
========================================================================================================================
key spammer:
        Usage: Spams a key every specified seconds. Can be used for auto looting, auto attacking, auto buffs
        Format: key_to_spam delay #_times_spammed
        Example usage:
                An example of a command that will use keyspammer as an auto looter(assuming your loot key is on z)..
                Hotkey(on/off) toggle: F1 (refer to vk link for 0x value)
                key_to_spam : z (refer to vk link for 0x value)
                delay : 100 (leave it at 100 if you just want a key to be spammed a lot)
                #_times_spammed: 1 (Spams z once every 100 ms)
               
                [0x70]
                0x5A 100 1
========================================================================================================================
left/right:
        Usage: moves the character left or right for a specified amount of time
        Format: left time
                right time
        Example usage:
                An example of a command that will move your character to the left for 500ms and then right for 500ms..
                Hotkey(on/off) toggle: F1 (refer to vk link for 0x value)
                time : 100
               
                [0x70]
                left 500
                right 500
========================================================================================================================
sleep:
        Usage: waits for a specified amount of times before running next command
        Format: sleep time
        Example usage:
                An example of a script that will move your character to the left for 500ms and then right for 500ms, then wait
                for 5000ms before repeating..
                Hotkey(on/off) toggle: F1 (refer to vk link for 0x value)
                time : 100
               
                [0x70]
                left 500
                right 500
                sleep 5000
========================================================================================================================
autochat:
        Usage: spams a string of text
        Format: autochat text
        Example usage:
                An example of a script that will spam "b>pots@@@@@" every 1000ms
                Hotkey(on/off) toggle: F1 (refer to vk link for 0x value)
                text: b>pots@@@@@
               
                [0x70]
                autochat b>pots@@@@@
                sleep 1000
========================================================================================================================
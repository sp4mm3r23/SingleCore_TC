SET @ENTRY := 94015;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;

DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,0,11,0,100,255,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Supreme Lord Kazzak - On Respawn - Say Line 0"),
(@ENTRY,0,1,0,4,0,100,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,"Supreme Lord Kazzak - On Aggro - Say Line 1"),
(@ENTRY,0,2,0,6,0,100,0,0,0,0,0,1,2,0,0,0,0,0,0,0,0,0,0,0,0,0,"Supreme Lord Kazzak - On Just Died - Say Line 2"),
(@ENTRY,0,3,0,5,0,100,0,1000,1000,0,0,1,4,0,0,0,0,0,0,0,0,0,0,0,0,0,"Supreme Lord Kazzak - On Killed Unit - Say Line 4"),
(@ENTRY,0,4,0,0,0,100,0,5000,5000,25000,25000,122,187664,1,800,0,0,0,1,0,0,0,0,0,0,0,"Supreme Lord Kazzak - On Aggro - Cast Fel Breath"),
(@ENTRY,0,5,0,31,0,100,0,187664,0,10000,10000,1,3,0,0,0,0,0,0,0,0,0,0,0,0,0,"Supreme Lord Kazzak - On Target Spellhit Fel Breath - Say Line 3"),
(@ENTRY,0,6,0,0,0,100,0,20000,20000,25000,30000,122,187466,0,50000,0,0,0,1,0,0,0,0,0,0,0,"Supreme Lord Kazzak - In Combat - Cast Supreme Doom."),
(@ENTRY,0,7,0,31,0,100,0,187466,0,10000,10000,1,5,0,0,0,0,0,0,0,0,0,0,0,0,0,"Supreme Lord Kazzak - On Target Supreme Doom - Say Line 3"),
(@ENTRY,0,8,0,2,0,100,1,0,66,0,0,11,187702,1,0,0,0,0,1,0,0,0,0,0,0,0,"Supreme Lord Kazzak - On Health 0-66 - Cast Twisting Mirror."),
(@ENTRY,0,9,0,2,0,100,1,0,66,0,0,1,7,0,0,0,0,0,1,0,0,0,0,0,0,0,"Supreme Lord Kazzak - On Health 0-66 - Say Line 7."),
(@ENTRY,0,10,0,2,0,100,1,0,33,0,0,11,187702,1,0,0,0,0,1,0,0,0,0,0,0,0,"Supreme Lord Kazzak - On Health 0-33 - Cast Twisting Mirror."),
(@ENTRY,0,11,0,2,0,100,1,0,33,0,0,1,7,0,0,0,0,0,1,0,0,0,0,0,0,0,"Supreme Lord Kazzak - On Health 0-33 - Say Line 7."),
(@ENTRY,0,12,0,0,0,100,0,20000,20000,25000,30000,122,187667,0,50000,0,0,0,1,0,0,0,0,0,0,0,"Supreme Lord Kazzak - In Combat - Cast Supreme Doom.");

-- Twisted Reflection SAI
SET @ENTRY := 94712;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,0,25,0,100,0,0,0,0,0,11,187709,0,0,0,0,0,1,0,0,0,0,0,0,0,"Twisted Reflection - On Reset - Cast Twisting Reflection."),
(@ENTRY,0,1,0,25,0,100,0,0,0,0,0,8,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Twisted Reflection - On Reset - Set Reactstate Passive"),
(@ENTRY,0,2,0,25,0,100,0,0,0,0,0,86,165486,2,21,100,0,0,1,0,0,0,0,0,0,0,"Twisted Reflection - On Reset - Cross Cast Madness Flash"),
(@ENTRY,0,3,0,54,0,100,0,0,0,0,0,69,0,0,0,0,0,0,19,94015,100,0,0,0,0,0,"Twisted Reflection - On Just Summoned - Move To Closest Creature 'Supreme Lord Kazzak'"),
(@ENTRY,0,4,0,75,0,100,1,0,94015,5,0,121,187711,0,20,0,0,0,7,0,0,0,0,0,0,0,"Twisted Reflection - On Distance To Creature - Cast Twisting Reflection"),
(@ENTRY,0,5,0,75,0,100,1,0,94015,5,0,41,1000,0,0,0,0,0,1,0,0,0,0,0,0,0,"Twisted Reflection - On Distance To Creature - Despawn In 2000 ms"),
(@ENTRY,0,6,0,25,0,100,0,0,0,0,0,128,402000,6,0,0,0,0,1,0,0,0,0,0,0,0,"Twisted Reflection - On Reset - Set Max Health."),
(@ENTRY,0,7,0,25,0,100,0,0,0,0,0,128,402000,7,0,0,0,0,1,0,0,0,0,0,0,0,"Twisted Reflection - On Reset - Set Health.");

DELETE FROM `creature_text` WHERE `entry` = 94015;
insert into `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `BroadcastTextId`, `TextRange`, `comment`) values
('94015','0','0','The Burning Legion comes!','14','0','0','0','0','50328','95732','3','Supreme Lord Kazzak - INTRO.'),
('94015','1','0','You face the might of the Burning Legion!','14','0','0','0','0','50326','95734','3','Supreme Lord Kazzak - AGGRO.'),
('94015','2','0','Oblivion... comes...','14','0','0','0','0','50327','95743','3','Supreme Lord Kazzak - DEATH.'),
('94015','3','0','All life will be eradicated!','14','0','0','0','0','50329','95741','3','Supreme Lord Kazzak - ERADICATE_1.'),
('94015','4','0','Pathetic wretch!','14','0','0','0','0','50330','95742','3','Supreme Lord Kazzak - ERADICATE_2.'),
('94015','5','0','Burn!','14','0','0','0','0','50331','95736','3','Supreme Lord Kazzak - ERADICATE_3.'),
('94015','5','1','Your life ends now!','14','0','0','0','0','50332','95737','3','Supreme Lord Kazzak - ERADICATE_4.'),
('94015','6','0','Suffer my wrath!','14','0','0','0','0','50333','95738','3','Supreme Lord Kazzak - ERADICATE_5.'),
('94015','7','0','My power grows!','14','0','0','0','0','50334','95739','3','Supreme Lord Kazzak - ERADICATE_6.'),
('94015','7','1','Feel the legion\'s power!','14','0','0','0','0','50335','95740','3','Supreme Lord Kazzak - ERADICATE_7.');
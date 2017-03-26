UPDATE `creature_template` SET `unit_flags` = '49920' WHERE `entry` = '50047';
UPDATE `creature_template` SET `lootid` = '49874' WHERE `entry` = '49874';

UPDATE `creature_queststarter` SET `id` = '197' WHERE `id` = '63258' AND `quest` = '31143';
UPDATE `creature_queststarter` SET `id` = '197' WHERE `id` = '911' AND `quest` = '28789';
DELETE FROM `creature_queststarter` WHERE `id` = 197 AND `quest` = 31139;
INSERT INTO `creature_queststarter` (`id`, `quest`) VALUES ('197', '31139');
DELETE FROM `creature_queststarter` WHERE `id` = 197 AND `quest` = 31140;
INSERT INTO `creature_queststarter` (`id`, `quest`) VALUES ('197', '31140');
UPDATE `creature_queststarter` SET `id` = '197' WHERE `id` = '43278' AND `quest` = '28780';
UPDATE `creature_queststarter` SET `id` = '197' WHERE `id` = '459' AND `quest` = '28788';
UPDATE `creature_queststarter` SET `id` = '197' WHERE `id` = '925' AND `quest` = '28785';
UPDATE `creature_queststarter` SET `id` = '197' WHERE `id` = '915' AND `quest` = '28787';
UPDATE `creature_queststarter` SET `id` = '197' WHERE `id` = '375' AND `quest` = '28786';
UPDATE `creature_queststarter` SET `id` = '197' WHERE `id` = '198' AND `quest` = '28784';

DELETE FROM `creature_loot_template` WHERE `Entry`=97 AND `Item`=1307;
DELETE FROM `creature_loot_template` WHERE `Entry`=100 AND `Item`=1307;
DELETE FROM `creature_loot_template` WHERE `Entry`=478 AND `Item`=1307;
DELETE FROM `creature_loot_template` WHERE `Entry` = 13159 AND `Item` = 1307;
INSERT INTO `creature_loot_template` (`Entry`, `Item`, `Chance`) VALUES ('13159', '1307', '88.49');

DELETE FROM `quest_template_addon` WHERE `PrevQuestID`=39 AND `ID`=46;
DELETE FROM `quest_template_addon` WHERE `PrevQuestID`=239 AND `ID`=11;
DELETE FROM `quest_template_addon` WHERE `ID` IN (46, 11);
INSERT INTO `quest_template_addon` (`ID`) VALUES ('46');
INSERT INTO `quest_template_addon` (`ID`) VALUES ('11');

DELETE FROM `quest_template_addon` WHERE `ID` = 239 AND `PrevQuestID` = 76;
INSERT INTO `quest_template_addon` (`ID`, `PrevQuestID`) VALUES ('239', '76');

DELETE FROM `quest_template_addon` WHERE `ID` = 26150 AND `PrevQuestID` = 60;
INSERT INTO `quest_template_addon` (`ID`, `PrevQuestID`) VALUES ('26150', '60');

UPDATE `quest_template_addon` SET `NextQuestID` = '26389' WHERE `ID` = '28823';
UPDATE `quest_template_addon` SET `NextQuestID` = '26389' WHERE `ID` = '28819';
UPDATE `quest_template_addon` SET `NextQuestID` = '26389' WHERE `ID` = '28817'; -- Hunter
UPDATE `quest_template_addon` SET `NextQuestID` = '26389' WHERE `ID` = '28821'; -- Rogue
UPDATE `quest_template_addon` SET `NextQuestID` = '26389' WHERE `ID` = '28820'; -- Priest
UPDATE `quest_template_addon` SET `NextQuestID` = '26389' WHERE `ID` = '28818'; -- Mage
UPDATE `quest_template_addon` SET `NextQuestID` = '26389' WHERE `ID` = '28822'; -- Warlock
UPDATE `quest_template_addon` SET `NextQuestID` = '28789' WHERE `ID` = '28774';
UPDATE `quest_template_addon` SET `PrevQuestID` = '28774' , `NextQuestID` = '0' WHERE `ID` = '28789';

DELETE FROM `quest_template_addon` WHERE `ID`=31142 AND `NextQuestID`=31143; 
DELETE FROM `quest_template_addon` WHERE `ID`=31140 AND `PrevQuestID`=31139;
DELETE FROM `quest_template_addon` WHERE `ID`=31143 AND `PrevQuestID`=31140;
DELETE FROM `quest_template_addon` WHERE `ID`=31144 AND `PrevQuestID`=31143;
DELETE FROM `quest_template_addon` WHERE `ID`=31145 AND `PrevQuestID`=31144 AND `NextQuestID`=26389;
DELETE FROM `quest_template_addon` WHERE `ID`=31139 AND `AllowableClasses`=512;
INSERT INTO `quest_template_addon` (`ID`, `PrevQuestID`) VALUES ('31140', '31139');
INSERT INTO `quest_template_addon` (`ID`, `PrevQuestID`) VALUES ('31143', '31140');
INSERT INTO `quest_template_addon` (`ID`, `PrevQuestID`) VALUES ('31144', '31143');
INSERT INTO `quest_template_addon` (`ID`, `PrevQuestID`, `NextQuestID`) VALUES ('31145', '31144', '26389');
INSERT INTO `quest_template_addon` (`ID`, `AllowableClasses`) VALUES ('31139', '512');
UPDATE `quest_template_addon` SET `NextQuestID` = '28785' WHERE `ID` = '28770';
UPDATE `quest_template_addon` SET `PrevQuestID` = '28770' , `NextQuestID` = '0' WHERE `ID` = '28785';
UPDATE `quest_template_addon` SET `NextQuestID` = '28780' WHERE `ID` = '28759';
UPDATE `quest_template_addon` SET `PrevQuestID` = '28759' , `NextQuestID` = '0' WHERE `ID` = '28780';
UPDATE `quest_template_addon` SET `NextQuestID` = '28787' WHERE `ID` = '28772';
UPDATE `quest_template_addon` SET `PrevQuestID` = '28772' , `NextQuestID` = '0' WHERE `ID` = '28787';
UPDATE `quest_template_addon` SET `NextQuestID` = '28786' WHERE `ID` = '28771';
UPDATE `quest_template_addon` SET `PrevQuestID` = '28771' , `NextQuestID` = '0' WHERE `ID` = '28786';
UPDATE `quest_template_addon` SET `NextQuestID` = '28784' WHERE `ID` = '28769';
UPDATE `quest_template_addon` SET `PrevQuestID` = '28769' , `NextQuestID` = '0' WHERE `ID` = '28784';
UPDATE `quest_template_addon` SET `NextQuestID` = '28788' WHERE `ID` = '28773';
UPDATE `quest_template_addon` SET `PrevQuestID` = '28773' , `NextQuestID` = '0' WHERE `ID` = '28788';

DELETE FROM `creature_queststarter` WHERE `id`=823 AND `quest`=31144;
DELETE FROM `creature_queststarter` WHERE `id`=823 AND `quest`=31145;
INSERT INTO `creature_queststarter` (`id`, `quest`) VALUES ('823', '31144');
INSERT INTO `creature_queststarter` (`id`, `quest`) VALUES ('823', '31145');

UPDATE `quest_template_addon` SET `PrevQuestID` = '-26389' WHERE `ID` = '26391';
UPDATE `creature_queststarter` SET `quest` = '37112' WHERE `id` = '6774' AND `quest` = '2158';

DELETE FROM `disables` WHERE `sourceType` = 1 AND `entry` IN (3100, 3101, 26910, 3102, 3103, 3104, 3105);
INSERT INTO `disables` (`sourceType`, `entry`, `comment`) VALUES
('1', '3100', 'Deprecated quest: Simple Letter'),
('1', '3101', 'Deprecated quest: Consecrated Letter'),
('1', '26910', 'Deprecated quest: Etched Letter'),
('1', '3102', 'Deprecated quest: Encrypted Letter'),
('1', '3103', 'Deprecated quest: Hallowed Letter'),
('1', '3104', 'Deprecated quest: Glyphic Letter'),
('1', '3105', 'Deprecated quest: Tainted Letter');

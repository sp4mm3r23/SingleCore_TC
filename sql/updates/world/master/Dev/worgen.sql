
-- Gilneas spell area
DELETE FROM `spell_area` WHERE `area` IN (4714,4725,4726,4727,4728,4729,4730,4731,4732,4733,4734,4735,4736,4755,4756,4757,4758,4759,4761,4762,4774,4786,4787,
4788,4789,4790,4791,4792,4793,4794,4806,4807,4808,4810,4811,4817,4818,4841,4842,4858,4868,4869,5432,5720);


DELETE FROM `creature_text` WHERE `entry` IN  (34851, 34864, 44086);
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `BroadcastTextId`, `comment`) VALUES
(34851, 1, 0, 'What... what are those things on the rooftops?', 12, 0, 100, 0, 0, 0, 39313, ''),
(34864, 1, 1, 'Everyone keep calm! We are controlling the situation.', 12, 0, 100, 0, 0, 0, 44375, ''),
(34864, 1, 2, 'Gate is closed. Order from Prince Liam!', 12, 0, 100, 0, 0, 0, 44373, ''),
(34864, 1, 3, 'In the woods is something strange going on, and to go beyond the city is unsafe.', 12, 0, 100, 0, 0, 0, 44374, ''),
(44086, 1, 1, 'You can''t do this to us!', 12, 0, 100, 0, 0, 0, 44377, ''),
(44086, 1, 2, 'What''s going on? I think I saw something on the roof.', 12, 0, 100, 0, 0, 0, 44379, ''),
(44086, 1, 3, 'My children stayed in Wuthering Heights, quite alone! I shall return to them!', 12, 0, 100, 0, 0, 0, 44378, '');

UPDATE `creature_template` SET `ScriptName`='' WHERE `entry` =34863;
UPDATE `creature_template` SET `AIName`='', `ScriptName`='' WHERE `entry` =34864;
UPDATE `creature_template` SET `AIName`='', `ScriptName`='' WHERE `entry` =44086;
UPDATE `creature_template` SET `AIName`='', `ScriptName`='' WHERE `entry` =34850;
DELETE FROM `creature_addon` WHERE `guid` =314725;

SET @CGUID := 4000000;

DELETE FROM `creature` WHERE `map` = 654;
DELETE FROM `gameobject` WHERE `map` = 654;

DELETE FROM `creature_queststarter` WHERE `id` = 34884; -- remove quest qiver flag
UPDATE `gameobject` SET `PhaseGroup` = 368 WHERE `map` = 654;
 
DELETE FROM `creature_addon` WHERE `guid` BETWEEN @CGUID+114 AND @CGUID+125;


DELETE FROM `creature_template_addon` WHERE `entry` IN (34936, 34913, 35840, 35874, 35869, 35871, 35873, 35839, 35112, 35115, 50371, 35077, 35124, 35369);


DELETE FROM `gossip_menu` WHERE `entry` IN (11070, 10676, 10697, 10702, 10699, 10700, 10696, 10698, 10694); 
INSERT INTO `gossip_menu` (`entry`, `text_id`) VALUES
(11070, 15395),
(10676, 14798),
(10697, 14839),
(10697, 14840),
(10702, 14847),
(10702, 14848),
(10699, 14842),
(10699, 14843),
(10700, 14844),
(10700, 14845),
(10696, 14834),
(10696, 14836),
(10698, 14840),
(10698, 14841),
(10694, 14831),
(10694, 14832);

DELETE FROM `creature_text` WHERE `entry` IN (34913, 35836, 35112, 35115, 35369, 50415);
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `BroadcastTextId`, `comment`) VALUES
(34913, 1, 1, 'Stand your ground, men!', 14, 0, 100, 0, 0, 19617, 35166, ''),
(34913, 1, 2, 'Defeat these foul beasts!', 14, 0, 100, 0, 0, 19618, 35167, ''),
(34913, 1, 3, 'Protect the civilians!', 14, 0, 100, 0, 0, 19619, 35168, ''),
(34913, 1, 4, 'Push them back!', 14, 0, 100, 0, 0, 19620, 35169, ''),
(34913, 1, 5, 'Take heart, men! We must protect our city!', 14, 0, 100, 0, 0, 19621, 35170, ''),
(35836, 1, 1, 'I don''t want to die!', 14, 0, 100, 0, 0, 0, 36041, ''),
(35836, 1, 2, 'He is right behind me!', 14, 0, 100, 430, 0, 0, 36037, ''),
(35836, 1, 3, 'They''re after me!', 14, 0, 100, 430, 0, 0, 36039, ''),
(35836, 1, 4, 'Help!', 14, 0, 100, 430, 0, 0, 36040, ''),
(35836, 1, 5, 'Help me please!', 14, 0, 100, 430, 0, 0, 0, ''),
(35836, 1, 6, 'Please save me!', 14, 0, 100, 430, 0, 0, 0, ''),
(35836, 1, 7, 'One of them is right behind me!', 14, 0, 100, 430, 0, 0, 0, ''),
(35112, 1, 0, 'Enough! I''ve made up my mind. We need Crowley on our side.', 12, 0, 100, 0, 0, 0, 38602, ''),
(35115, 1, 0, 'We ought to leave Crowley in prison with those beasts!', 12, 0, 100, 0, 0, 0, 38601, ''),
(35369, 1, 1, 'I can''t fight it!', 14, 0, 100, 0, 0, 0, 38741, ''),
(35369, 1, 2, 'Make it stop!', 14, 0, 100, 0, 0, 0, 38740, ''),
(35369, 1, 3, 'My face! What''s wrong with my face!', 14, 0, 100, 0, 0, 0, 38737, ''),
(35369, 1, 4, 'My hands... don''t look at my hands!!', 14, 0, 100, 0, 0, 0, 38738, ''),
(35369, 1, 5, 'The pain is unbearable!', 14, 0, 100, 0, 0, 0, 38739, ''),
(35369, 1, 6, 'What''s wrong with me??!', 14, 0, 100, 0, 0, 0, 0, ''),
(50415, 1, 0, 'You''ve been bitten by a worgen. It''s probably nothing, but it sure stings a little.$B$B|TInterface\\Icons\\INV_Misc_monsterfang_02.blp:32|t', 41, 0, 100, 0, 0, 0, 0, '');

DELETE FROM `creature_text` WHERE `entry` IN (35118, 47091);
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(35118, 1, 0, "%s becomes enraged!", 16, 0, 100, 0, 0, 0, "Bloodfang Worgen - combat Enrage");




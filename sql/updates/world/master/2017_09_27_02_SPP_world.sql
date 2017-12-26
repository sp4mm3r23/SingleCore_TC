DELETE FROM `spell_proc` WHERE `SpellId` = 203555;
INSERT INTO `spell_proc` (`SpellId`, `SchoolMask`, `SpellFamilyName`, `SpellFamilyMask0`, `SpellFamilyMask1`, `SpellFamilyMask2`, `spellFamilyMask3`, `procFlags`, `SpellPhaseMask`, `ProcsPerMinute`, `Chance`, `Cooldown`) VALUES ('203555', '0', '107', '0', '0', '0', '0', '4', '0', '0', '75', '0');

DELETE FROM `spell_proc` WHERE `SpellId`= 197125;
INSERT INTO `spell_proc` (`SpellId`, `SchoolMask`, `SpellFamilyName`, `SpellFamilyMask0`, `SpellFamilyMask1`, `SpellFamilyMask2`, `spellFamilyMask3`, `procFlags`, `SpellPhaseMask`, `ProcsPerMinute`, `Chance`, `Cooldown`) VALUES('197125', '0', '107', '16448', '2048', '0', '0', '0', '2', '0', '0', '0');

DELETE FROM `spell_script_names` WHERE `spell_id` IN (197125, 195072, 197923, 203555);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(197125, 'spell_dh_chaos_strike'),
(195072, 'spell_dh_fel_rush'),
(197923, 'spell_dh_fel_rush_aura'),
(203555, 'spell_dh_demon_blades');

DELETE FROM `spell_script_names` WHERE `spell_id` IN (118009, 49184, 108199, 119975, 116888, 114556);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(118009,'spell_dk_desecrated_ground'),
(49184,'spell_dk_howling_blast'),
(108199,'spell_dk_gorefiends_grasp'),
(119975,'spell_dk_conversion'),
(116888,'spell_dk_purgatory'),
(114556,'spell_dk_purgatory_absorb');

UPDATE `creature_template` SET `AIName` = '', `ScriptName` = 'boss_drahga_shadowburner' WHERE `entry` = 40319;

DELETE FROM `creature_text` WHERE `entry` = 40319;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`, `BroadcastTextID`) VALUES
('40319','0','0','I will burn you from the inside out!','14','0','0','0','0','18610','VO_GB_Drahga_Engage','47760'),
('40319','1','0','INCINERATE THEM, MINIONS!','14','0','0','0','0','18616','VO_GB_Drahga_Sum_Spirits1','47765'),
('40319','1','1','BY FIRE BE... BURNED!','14','0','0','0','0','18619','VO_GB_Drahga_Sum_Spirits2','47766'),
('40319','2','0','Dragon, you will do as I command! Catch me!','14','0','0','0','0','18621','VO_GB_Drahga_Sum_Valiona','47767'),
('40319','3','0','You should have come better prepared!','14','0','0','0','0','18612','VO_GB_Drahga_Slay01','47762'),
('40319','3','1','An easy kill!','14','0','0','0','0','18611','VO_GB_Drahga_Slay02','47761'),
('40319','4','0','Valiona, finish them! Avenge me!','14','0','0','0','0','18614','VO_GB_Drahga_Death','47764');

DELETE FROM `creature_loot_template` WHERE `Entry` = 40319;
INSERT INTO `creature_loot_template` (`Entry`, `Item`, `Reference`, `Chance`, `QuestRequired`, `LootMode`, `GroupId`, `MinCount`, `MaxCount`, `Comment`) VALUES
('40319','21525','0','27','0','1','0','1','1',NULL),
('40319','56123','0','0','0','1','1','1','1',NULL),
('40319','56124','0','0','0','1','1','1','1',NULL),
('40319','56125','0','0','0','1','1','1','1',NULL),
('40319','56126','0','0','0','1','1','1','1',NULL),
('40319','56127','0','0','0','1','1','1','1',NULL),
('40319','66927','0','100','1','1','0','1','1',NULL);


UPDATE `creature_template` SET `AIName` = '', `ScriptName` = 'npc_drahga_valiona' WHERE `entry` = 40320;
UPDATE `creature_template` SET `AIName` = '', `ScriptName` = 'npc_invocation_of_flame_stalker' WHERE `entry` = 40355;
UPDATE `creature_template` SET `AIName` = '', `ScriptName` = 'npc_invoked_flaming_spirit' WHERE `entry` = 40357;
UPDATE `creature_template` SET `AIName` = '', `ScriptName` = 'npc_seeping_twilight' WHERE `entry` = 40365;

DELETE FROM `spell_script_names` WHERE `ScriptName` = 'spell_drahga_supernova';
INSERT INTO `spell_script_names` VALUES
(75238, 'spell_drahga_supernova');

UPDATE `creature_template` SET `AIName` = '', `ScriptName` = 'boss_erudax' WHERE `entry` = 40484;

DELETE FROM `creature_text` WHERE `entry` = 40484;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`, `BroadcastTextID`) VALUES
('40484','0','0','The darkest days are still ahead!','14','0','0','0','0','18638','VO_GB_Erudax_Engage','47770'),
('40484','1','0','Flakh ghet! The shadows hunger cannot be sated!','14','0','0','0','0','18644','VO_GB_Erudax_Shad_Gale','0'),
('40484','2','0','Come, suffering... Enter, chaos!','14','0','0','0','0','18646','VO_GB_Erudax_Guardian','40612'),
('40484','3','0','Flesh for the offering!','14','0','0','0','0','18640','VO_GB_Erudax_Slay02','0'),
('40484','3','1','Erudax cackles maniacally.','16','0','0','0','0','18639','VO_GB_Erudax_Slay01','0'),
('40484','4','0','Ywaq maq oou; ywaq maq ssaggh. Yawq ma shg fhn.','14','0','0','0','0','18641','VO_GB_Erudax_Death','0');

DELETE FROM `creature_loot_template` WHERE `Entry` = 40484;
INSERT INTO `creature_loot_template` (`Entry`, `Item`, `Reference`, `Chance`, `QuestRequired`, `LootMode`, `GroupId`, `MinCount`, `MaxCount`, `Comment`) VALUES
('40484','56128','0','0','0','1','1','1','1',NULL),
('40484','56129','0','0','0','1','1','1','1',NULL),
('40484','56130','0','0','0','1','1','1','1',NULL),
('40484','56131','0','0','0','1','1','1','1',NULL),
('40484','56132','0','0','0','1','1','1','1',NULL),
('40484','56133','0','0','0','1','2','1','1',NULL),
('40484','56135','0','0','0','1','2','1','1',NULL),
('40484','56136','0','0','0','1','2','1','1',NULL),
('40484','56137','0','0','0','1','2','1','1',NULL),
('40484','56138','0','0','0','1','2','1','1',NULL);


UPDATE `creature_template` SET `AIName` = '', `ScriptName` = 'npc_erudax_faceless_corruptor' WHERE `entry` = 39392;
UPDATE `creature_template` SET `AIName` = '', `ScriptName` = 'npc_alexstrasza_egg' WHERE `entry` = 40486;
UPDATE `creature_template` SET `AIName` = '', `ScriptName` = 'npc_shadow_gale_stalker' WHERE `entry` = 40567;
UPDATE `creature_template` SET `AIName` = '', `ScriptName` = 'npc_erudax_twilight_hatchling' WHERE `entry` = 39388;

DELETE FROM `spell_script_names` WHERE `ScriptName` = 'spell_erudax_shadow_gale';
INSERT INTO `spell_script_names` VALUES
(75664, 'spell_erudax_shadow_gale');

UPDATE `creature_template` SET `AIName` = '', `ScriptName` = 'boss_forgemaster_throngus' WHERE `entry` = 40177;

DELETE FROM `creature_text` WHERE `entry` = 40177;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`, `BroadcastTextID`) VALUES
('40177','0','0','NO! Throngus get whipped again if he no finish!','14','0','0','0','0','18943','VO_GB_Throngus_Engage','47777'),
('40177','1','0','You not get through defenses!','14','0','0','0','0','18947','VO_GB_Throngus_Shield','47781'),
('40177','2','0','Throngus SLICE you up!','14','0','0','0','0','18948','VO_GB_Throngus_Sword','47783'),
('40177','3','0','Oh, this is gonna HURT!','14','0','0','0','0','18949','VO_GB_Throngus_Mace','0'),
('40177','4','0','You break easy!','14','0','0','0','0','18945','VO_GB_Throngus_Slay01','47779'),
('40177','4','1','Throngus use your corpse on body. Somewhere...','14','0','0','0','0','18944','VO_GB_Throngus_Slay02','47778'),
('40177','5','0','Death... Good choice. Not best choice maybe, but better than fail and live.','14','0','0','0','0','18946','VO_GB_Throngus_Death','47780');

DELETE FROM `creature_loot_template` WHERE `Entry` = 40177;
INSERT INTO `creature_loot_template` (`Entry`, `Item`, `Reference`, `Chance`, `QuestRequired`, `LootMode`, `GroupId`, `MinCount`, `MaxCount`, `Comment`) VALUES
('40177','56118','0','0','0','1','1','1','1',NULL),
('40177','56119','0','0','0','1','1','1','1',NULL),
('40177','56120','0','0','0','1','1','1','1',NULL),
('40177','56121','0','0','0','1','1','1','1',NULL),
('40177','56122','0','0','0','1','1','1','1',NULL),
('40177','71638','0','1.1','1','1','0','1','1','');


UPDATE `creature_template` SET `AIName` = '', `ScriptName` = 'npc_throngus_fire_patch' WHERE `entry` = 48711;

DELETE FROM `spell_script_names` WHERE `ScriptName` = 'spell_throngus_pick_weapon';
INSERT INTO `spell_script_names` VALUES
(75000, 'spell_throngus_pick_weapon');


UPDATE `creature_template` SET `AIName` = '', `ScriptName` = 'boss_general_umbriss' WHERE `entry` = 39625;

DELETE FROM `creature_text` WHERE `entry` = 39625;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`, `BroadcastTextID`) VALUES
('39625','0','0','A million more await my orders. What chance you do have?','14','0','0','0','0','18530','umbriss - SAY_AGGRO','0'),
('39625','1','0','Cover the rear! Alexstraza\'s brood decimate our rank!','14','0','0','0','0','18537','umbriss - SAY_BOMBING_1','0'),
('39625','2','0','Reinforce the front! We are being invaded!','14','0','0','0','0','18536','umbriss - SAY_BOMBING_2','40640'),
('39625','3','0','Attack you cowardly vermin!','14','0','0','0','0','18535','umbriss - SAY_SUMMON','47799'),
('39625','3','1','Cover the rear! Alexstrazas brood decimate our rank!','14','0','0','0','0','18537','VO_GB_Umbriss_Bomb2','0'),
('39625','4','0','Messy...','14','0','0','0','0','18532','umbriss - SAY_KILL_1','47786'),
('39625','4','1','Vermin, your dinner awaits!','14','0','0','0','0','18531','VO_GB_Umbriss_Slay02','47785'),
('39625','5','0','Vermin, your dinner awaits!','14','0','0','0','0','18531','umbriss - SAY_KILL_2','47785'),
('39625','6','0','General Umbriss sets his eyes on $N and begins to cast blitz.','41','0','0','0','0','18533','umbriss - EMOTE_BLITZ','0'),
('39625','7','0','General Umbriss begins to cast ground siege.','41','0','0','0','0','18534','umbriss - EMOTE_GROUND_SIEGE','0');

DELETE FROM `creature_loot_template` WHERE `Entry` = 39625;
INSERT INTO `creature_loot_template` (`Entry`, `Item`, `Reference`, `Chance`, `QuestRequired`, `LootMode`, `GroupId`, `MinCount`, `MaxCount`, `Comment`) VALUES
('39625','56112','0','0','0','1','1','1','1',NULL),
('39625','56113','0','0','0','1','1','1','1',NULL),
('39625','56114','0','0','0','1','1','1','1',NULL),
('39625','56115','0','0','0','1','1','1','1',NULL),
('39625','56116','0','0','0','1','1','1','1',NULL);


UPDATE `creature_template` SET `AIName` = '', `ScriptName` = 'npc_malignant_trogg' WHERE `entry` = 39984;
UPDATE `creature_template` SET `AIName` = '', `ScriptName` = 'npc_umbriss_trogg_dweller' WHERE `entry` = 45467;

UPDATE `instance_template` SET `script` = 'instance_grim_batol' WHERE `map` = 670;

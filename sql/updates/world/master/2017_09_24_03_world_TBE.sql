DELETE FROM `spell_script_names` WHERE `ScriptName` IN ('spell_pri_surge_of_light_aura', 'spell_pri_surge_of_light');
INSERT INTO `spell_script_names` VALUE 
(109186, 'spell_pri_surge_of_light_aura'),
(114255, 'spell_pri_surge_of_light');

DELETE FROM `spell_script_names` WHERE `spell_id` IN (34433, 123040);
INSERT INTO `spell_script_names` VALUES
(34433, 'spell_pri_shadowfiend'),
(123040, 'spell_pri_shadowfiend');

UPDATE gameobject_template SET ScriptName = 'legion_instance_teleporter' WHERE entry = 252145;

DELETE FROM `game_tele` WHERE id IN (2000, 2001);
INSERT INTO `game_tele` VALUES
(2000, 3915.909912, 4536.861816, 86.404472, 5.707454, 1456, 'EyeOfAzshara Dungeon'),
(2001, 7186.82, 7319.46, 23.5025, 6.18178, 1492, 'MawOfSouls Dungeon');

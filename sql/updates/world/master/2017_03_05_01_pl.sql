-- Dalaran no Phase need Use dynamic spawn + specific spawn
-- Dalaran DH zone 3 phase need use spell
-- No use creature_addon more problem

DELETE FROM `creature` WHERE `PhaseID` IN (4598, 4707, 5011, 5132, 5162, 5177, 5188, 5196, 5243, 5380, 5544, 5548,
5556, 5781, 5896, 6117, 6216, 6518, 6763, 6764, 6765, 7340, 7595, 7807, 7870, 8357) AND `map` = 1220;
UPDATE `creature` SET `PhaseGroup` = 0 WHERE `PhaseGroup` = 942 AND `map` = 1220;

DELETE FROM `gameobject` WHERE `PhaseID` IN (4598, 4707, 5011, 5132, 5162, 5177, 5188, 5196, 5243, 5380, 5544, 5548,
5556, 5781, 5896, 6117, 6216, 6518, 6763, 6764, 6765, 7340, 7595, 7807, 7870, 8357) AND `map` = 1220;
UPDATE `gameobject` SET `PhaseGroup` = 0 WHERE `PhaseGroup` = 942 AND `map` = 1220;

SET @CGUID := 7020000;

DELETE FROM `creature_addon` WHERE `guid` BETWEEN @CGUID+0 AND @CGUID+20000;

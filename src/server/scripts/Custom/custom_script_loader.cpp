/*
 * Copyright (C) 2008-2017 TrinityCore <http://www.trinitycore.org/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

// This is where scripts' loading functions should be declared:
void AddSC_solocraft();
void AddSC_FireWorksOnLevelUp();
void AddSC_Player_Boa();
void AddSC_gon_playerscripts();
void AddSC_login_script();
void AddSC_announce_login();
void AddSC_Boss_Announcer();
void AddSC_XpWeekend();
void AddSC_RandomEnchantsScript();

// The name of this function should match:
// void Add${NameOfDirectory}Scripts()

// spell
void AddSC_priest_spell_scripts_pl();
void AddSC_demon_hunter_spell_scripts_pl();
void AddSC_deathknight_spell_scripts_pl();
void AddSC_monk_spell_scripts_pl();
void AddSC_hunter_spell_scripts_pl();

// zone
void AddSC_gilneas_pl();
void AddSC_zone_mardum_pl();

// dungeon
void AddSC_instance_eye_of_azshara();

// GrimBatol
void AddSC_instance_grim_batol();
void AddSC_boss_general_umbriss();
void AddSC_boss_forgemaster_throngus();
void AddSC_boss_erudax();
void AddSC_boss_drahga_shadowburner();
void AddSC_grim_batol();

// Throne Of The Tides
void AddSC_throne_of_the_tides();
void AddSC_instance_throne_of_the_tides();
void AddSC_boss_ozumat();
void AddSC_boss_lady_nazjar();
void AddSC_boss_erunak_stonespeaker();
void AddSC_boss_commander_ulthok();

// other
void AddSC_dungeon_portal_pl();

void AddCustomScripts()
{
    // SingleCore
	AddSC_solocraft();
	AddSC_FireWorksOnLevelUp();
	AddSC_Player_Boa();
	AddSC_gon_playerscripts();
	AddSC_login_script();
	AddSC_announce_login();
	AddSC_Boss_Announcer();
	AddSC_XpWeekend();
	AddSC_RandomEnchantsScript();


    // spell
    AddSC_priest_spell_scripts_pl();
    AddSC_demon_hunter_spell_scripts_pl();
    AddSC_deathknight_spell_scripts_pl();
    AddSC_monk_spell_scripts_pl();
    AddSC_hunter_spell_scripts_pl();

    // zone
    AddSC_gilneas_pl();
    AddSC_zone_mardum_pl();

    //dungeon
    AddSC_instance_eye_of_azshara();

    // GrimBatol
    AddSC_instance_grim_batol();
    AddSC_boss_general_umbriss();
    AddSC_boss_forgemaster_throngus();
    AddSC_boss_erudax();
    AddSC_boss_drahga_shadowburner();
    AddSC_grim_batol();

    // Throne Of The Tides
    AddSC_throne_of_the_tides();
    AddSC_instance_throne_of_the_tides();
    AddSC_boss_ozumat();
    AddSC_boss_lady_nazjar();
    AddSC_boss_erunak_stonespeaker();
    AddSC_boss_commander_ulthok();

    // other
    AddSC_dungeon_portal_pl();
}

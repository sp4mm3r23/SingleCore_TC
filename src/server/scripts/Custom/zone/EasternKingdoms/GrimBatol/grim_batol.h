/*
 * Copyright (C) 2016-2017 ProjectLegion <https://bitbucket.org/projectlegion/projectlegion/>
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

#ifndef GRIMBATOL_H_
#define GRIMBATOL_H_

#define GBScriptName "instance_grim_batol"
#define DataHeader "GB"

uint32 const EncounterCount = 4;

enum GBDataTypes
{
    DATA_GENERAL_UMBRISS             = 0,
    DATA_FORGEMASTER_THRONGUS        = 1,
    DATA_DRAHGA_SHADOWBURNER         = 2,
    DATA_ERUDAX                      = 3
};

enum GBCreatureIds
{
    NPC_GENERAL_UMBRISS              = 39625,
    NPC_FORGEMASTER_THRONGUS         = 40177,
    NPC_DRAHGA_SHADOWBURNER          = 40319,
    NPC_ERUDAX                       = 40484
};

enum GBAchievements
{
    ACHIEV_UMBRAGE_FOR_UMBRISS       = 5297
};

template<class AI>
AI* GetGrimBatolAI(Creature* creature)
{
    return GetInstanceAI<AI>(creature, GBScriptName);
}

#endif // GRIMBATOL_H_

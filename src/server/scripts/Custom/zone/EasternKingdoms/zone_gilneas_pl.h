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

#ifndef DEF_GILNEAS_PL_H
#define DEF_GILNEAS_PL_H

enum GilneasQuests
{
    QUEST_LOCKDOWN = 14078
};

enum GilneasCreatureIds
{
    NPC_PANICKED_CITIZEN_GATE = 44086,
    NPC_PANICKED_CITIZEN      = 34851
};

enum GilneasEvents
{
    EVENT_START_TALK_WITH_CITIZEN,
    EVENT_TALK_WITH_CITIZEN_1,
    EVENT_TALK_WITH_CITIZEN_2,
    EVENT_TALK_WITH_CITIZEN_3,
    EVENT_START_TALK_TO_GUARD,
    EVENT_TALK_TO_GUARD_1,
    EVENT_TALK_TO_GUARD_2,
    EVENT_TALK_TO_GUARD_3,
    EVENT_COUNT_COOLDOWN,
    EVENT_MASTER_RESET
};

enum GilneasSpells
{
    SPELL_PHASE_QUEST_ZONE_SPECIFIC_01           = 59073,

    SPELL_UPDATE_PHASE_SHIFT                     = 82238
};

#endif

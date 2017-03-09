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

#include "grim_batol.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "InstanceScript.h"

class instance_grim_batol : public InstanceMapScript
{
    public:
        instance_grim_batol() : InstanceMapScript(GBScriptName, 670) { }

        struct instance_grim_batol_InstanceMapScript : public InstanceScript
        {
            instance_grim_batol_InstanceMapScript(Map* map) : InstanceScript(map)
            {
                SetHeaders(DataHeader);
                SetBossNumber(EncounterCount);
            }

            void OnCreatureCreate(Creature* creature) override
            {
                switch(creature->GetEntry())
                {
                    case NPC_GENERAL_UMBRISS:
                        GeneralUmbrissGUID = creature->GetGUID();
                        break;
                    case NPC_FORGEMASTER_THRONGUS:
                        ForgemasterThrongusGUID = creature->GetGUID();
                        break;
                    case NPC_DRAHGA_SHADOWBURNER:
                        DrahgaShadowburnerGUID = creature->GetGUID();
                        break;
                    case NPC_ERUDAX:
                        ErudaxGUID = creature->GetGUID();
                        break;
                }
            }
 
            ObjectGuid GetGuidData(uint32 type) const override
            {
                switch(type)
                {
                    case DATA_GENERAL_UMBRISS:
                        return GeneralUmbrissGUID;
                    case DATA_FORGEMASTER_THRONGUS:
                        return ForgemasterThrongusGUID;
                    case DATA_DRAHGA_SHADOWBURNER:
                        return DrahgaShadowburnerGUID;
                    case DATA_ERUDAX:
                        return ErudaxGUID;
                    default:
                        break;
                }

                return ObjectGuid::Empty;
            }

        private:
            ObjectGuid GeneralUmbrissGUID;
            ObjectGuid ForgemasterThrongusGUID;
            ObjectGuid DrahgaShadowburnerGUID;
            ObjectGuid ErudaxGUID;
        };

        InstanceScript* GetInstanceScript(InstanceMap* map) const override
        {
            return new instance_grim_batol_InstanceMapScript(map);
        }
};

void AddSC_instance_grim_batol()
{
    new instance_grim_batol();
}

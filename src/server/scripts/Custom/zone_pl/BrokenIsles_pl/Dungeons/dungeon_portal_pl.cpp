/*
 * Copyright (C) 2016-2017 ProjectLegion <http://www.projectlegion.net/>
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

#include "ScriptMgr.h"
#include "Cell.h"
#include "CellImpl.h"
#include "GridNotifiersImpl.h"
#include "GridNotifiers.h"

static const float radius = 3.0f;

class legion_instance_teleporter : public GameObjectScript
{
 public:
    legion_instance_teleporter() : GameObjectScript("legion_instance_teleporter") { }

    void OnUpdate(GameObject* me, uint32 diff) override
    {
        std::list<Player*> playersList;
        Trinity::AnyPlayerInObjectRangeCheck checker(me, radius);
        Trinity::PlayerListSearcher<Trinity::AnyPlayerInObjectRangeCheck> searcher(me, playersList, checker);
        me->VisitNearbyWorldObject(radius, searcher);

        if(playersList.empty())
            return;

        switch(me->GetMapId())
        {

            case 1220: // Broken Isles
                switch(me->GetZoneId())
                {
                    case 7541:
                        switch(me->GetAreaId())
                        {
                            case 7927:
                                for(std::list<Player*>::const_iterator iter = playersList.begin(); iter != playersList.end(); ++iter)
                                    (*iter)->TeleportTo(1492, 7180.229f, 7319.378f, 22.709f, 6.209f);
                                break;
                        }
                        break;
                    case 7334: 
                        switch(me->GetAreaId())
                        {
                            case 7365:
                                for(std::list<Player*>::const_iterator iter = playersList.begin(); iter != playersList.end(); ++iter)
                                    (*iter)->TeleportTo(1456, 3915.909f, 4536.861f, 86.404f, 5.707f);
                                break;
                        }
                        break;
                }
                break;

            case 1492: 
                for(std::list<Player*>::const_iterator iter = playersList.begin(); iter != playersList.end(); ++iter)
                    (*iter)->TeleportTo(1220, 3401.617f, 1992.599f, 11.790f, 3.149f);
                break;
            case 1456:
                for(std::list<Player*>::const_iterator iter = playersList.begin(); iter != playersList.end(); ++iter)
                    (*iter)->TeleportTo(1220, 0.784f, 5800.117f, 2.217f, 1.409f);
                break;
        }
    }

};

void AddSC_dungeon_portal_pl()
{
    new legion_instance_teleporter();
}

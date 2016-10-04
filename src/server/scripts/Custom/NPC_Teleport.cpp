/*
**
** www.wowcore.com.br
**
*/

using namespace std;

struct TeleData
{
    uint32 menu;
    uint32 submenu;
    uint32 icon;
    string name;
    uint32 cost;
    uint32 faction;
    uint32 map;
    double x, y, z, o;
};

vector<TeleData> tData;

void LoadTeleData()
{
    tData.clear();

    QueryResult result = LoginDatabase.PQuery("SELECT menu, submenu, icon, name, cost, faction, map, x, y, z, o FROM npc_teleport");

    if (result)
    {
        Field * fields = NULL;

        do
        {
            fields = result->Fetch();

            TeleData data;
            data.menu = fields[0].GetUInt32();
            data.submenu = fields[1].GetUInt32();
            data.icon = fields[2].GetUInt32();
            data.name = fields[3].GetString();
            data.cost = fields[4].GetUInt32();
            data.faction = fields[5].GetUInt32();
            data.map = fields[6].GetUInt32();
            data.x = fields[7].GetDouble();
            data.y = fields[8].GetDouble();
            data.z = fields[9].GetDouble();
            data.o = fields[10].GetDouble();

            tData.push_back(data);

        } while (result->NextRow());
    }
}

bool CheckFaction(uint32 playerFaction, uint32 teleFaction)
{
    if (teleFaction == uint32(TEAM_OTHER) || teleFaction == playerFaction)
        return true;
    else
        return false;
}

class NPC_Teleport : public CreatureScript
{
public:
    NPC_Teleport() : CreatureScript("NPC_Teleport") { }

    void GetMenu(Player* player, Creature* creature, uint32 menuId)
    {
        for (uint32 i = 0; i < tData.size(); i++)
        {
            if (!CheckFaction(player->GetTeam(), tData[i].faction))
                continue;

            if (tData[i].menu == menuId)
                player->ADD_GOSSIP_ITEM(tData[i].icon, tData[i].name, GOSSIP_SENDER_MAIN, i);
        }

        player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
    }

    bool OnGossipHello(Player* player, Creature* creature)
    {
        LoadTeleData();

        GetMenu(player, creature, 1);
        return true;
    }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 sender, uint32 action)
    {
        player->PlayerTalkClass->ClearMenus();

        uint32 menuData = tData[action].submenu;

        if (menuData == 0)
        {
            uint32 copper = (tData[action].cost % GOLD) % SILVER;
            uint32 silver = (tData[action].cost % GOLD) / SILVER;
            uint32 gold = tData[action].cost / GOLD;

            if (player->GetMoney() < tData[action].cost)
                ChatHandler(player->GetSession()).PSendSysMessage("Failure! Not enough money. Costs: %ug %us %uc", gold, silver, copper);
            else
            {
                ChatHandler(player->GetSession()).PSendSysMessage("Success! You was teleported.");
                player->TeleportTo(tData[action].map, tData[action].x, tData[action].y, tData[action].z, tData[action].o);
                player->ModifyMoney(int32(-tData[action].cost));
            }

            menuData = tData[action].menu;
        }

        GetMenu(player, creature, menuData);
        return true;
    }
};

void AddSC_NPC_Teleport()
{
    new NPC_Teleport();
}

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

#include "Player.h"
#include "ScriptMgr.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"
#include "SpellHistory.h"
#include "Containers.h"

enum DruidSpells_pl
{
    SPELL_DRUID_CAT_FORM           = 768,
    SPELL_DRUID_PROWL              = 5215
};


// Called by Prowl - 5215, Displacer Beast - 102280, Dash - 1850
// Should activate the cat form if not in cat form
class spell_dru_prowl : public SpellScriptLoader
{
    public:
        spell_dru_prowl() : SpellScriptLoader("spell_dru_prowl") { }

        class spell_dru_prowl_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_prowl_SpellScript);

            bool Validate(SpellInfo const* /*spellEntry*/) override
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_DRUID_PROWL))
                    return false;
                return true;
            }

            void HandleOnHit()
            {
                // This spell activate the cat form
                if (Player* _player = GetCaster()->ToPlayer())
                    _player->CastSpell(_player, SPELL_DRUID_CAT_FORM, true);
            }

            void Register() override
            {
                OnHit += SpellHitFn(spell_dru_prowl_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_dru_prowl_SpellScript();
        }
};

void AddSC_druid_spell_scripts_pl()
{
    new spell_dru_prowl();
}

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

#include "ScriptMgr.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"
#include "GridNotifiers.h"
#include "GridNotifiersImpl.h"
#include "Cell.h"
#include "CellImpl.h"

enum MonkSpells
{
    SPELL_MONK_CHI_WAVE_HEAL = 132463
};

// Chi Wave (healing bolt) - 173545
class spell_monk_chi_wave_healing_bolt: public SpellScriptLoader
{
    public:
        spell_monk_chi_wave_healing_bolt() : SpellScriptLoader("spell_monk_chi_wave_healing_bolt") { }

        class spell_monk_chi_wave_healing_bolt_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_monk_chi_wave_healing_bolt_SpellScript);

            bool Validate(SpellInfo const* /*spellInfo*/) override
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_MONK_CHI_WAVE_HEAL))
                    return false;
                return true;
            }

            void HandleOnHit()
            {
                if (!GetOriginalCaster())
                    return;

                if (Player* player = GetOriginalCaster()->ToPlayer())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        if (!target->IsValidAttackTarget(player))
                            player->CastSpell(target, SPELL_MONK_CHI_WAVE_HEAL, true);
                    }
                }
            }

            void Register() override
            {
                OnHit += SpellHitFn(spell_monk_chi_wave_healing_bolt_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_monk_chi_wave_healing_bolt_SpellScript();
        }
};

void AddSC_monk_spell_scripts_pl()
{
    new spell_monk_chi_wave_healing_bolt();
}

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
#include "Cell.h"
#include "CellImpl.h"
#include "GridNotifiers.h"
#include "GridNotifiersImpl.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"

// Burrow Attack  - 95714
// 7.x.x
class spell_hun_burrow_attack : public SpellScriptLoader
{
    public:
        spell_hun_burrow_attack() : SpellScriptLoader("spell_hun_burrow_attack") { }

        class spell_hun_burrow_attack_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_hun_burrow_attack_SpellScript);

            void HandleDamage(SpellEffIndex /*effIndex*/)
            {
                SetHitDamage((int32)(GetHitDamage() + ((GetCaster()->GetTotalAttackPowerValue(RANGED_ATTACK) * 0.60f) * 0.096) * 8));
            }

            void Register() override
            {
                OnEffectHitTarget += SpellEffectFn(spell_hun_burrow_attack_SpellScript::HandleDamage, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_hun_burrow_attack_SpellScript();
        }
};

void AddSC_hunter_spell_scripts_pl()
{
    new spell_hun_burrow_attack();
}

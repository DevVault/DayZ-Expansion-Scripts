/**
 * ExpansionActionCrackContainer.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionActionCrackContainer : ExpansionActionDestroyBase
{
	override void Setup( PlayerBase player, ActionTarget target, ItemBase item )
	{
		m_Time = GetExpansionSettings().GetRaid().ContainerRaidToolTimeSeconds;
		m_Cycles = GetExpansionSettings().GetRaid().ContainerRaidToolCycles;
		m_MinHealth01 = 0.01;  //! 1% health
		m_ToolDamagePercent = GetExpansionSettings().GetRaid().ContainerRaidToolDamagePercent;
	}

	override string GetText()
	{
		return "#STR_EXPANSION_UA_CRACK_Container";
	}

	override bool DestroyCondition( PlayerBase player, ActionTarget target, ItemBase item, bool camera_check )
	{
		if ( !super.DestroyCondition( player, target, item, camera_check ) )
			return false;

		Container_Base Container = Container_Base.Cast( target.GetObject() );

		if ( !Container || !Container.ExpansionIsLocked() )
			return false;

		ExpansionSafeBase safe = ExpansionSafeBase.Cast( target.GetObject() );

		if ( safe )
			return false;

		if ( GetExpansionSettings().GetRaid().ContainerRaidTools.Find( item.GetType() ) == -1 )
			return false;

		return true;
	}

	override bool CanBeDestroyed( Object targetObject )
	{
		return GetExpansionSettings().GetRaid().CanRaidContainers;
	}
}

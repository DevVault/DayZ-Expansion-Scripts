class ExpansionActionPickVehicleLockBase: ExpansionActionToolBase 
{
	void ExpansionActionPickVehicleLockBase()
	{
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_INTERACT;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH;
	}

	override void Setup( PlayerBase player, ActionTarget target, ItemBase item )
	{
		CarScript car = CarScript.Cast( target.GetParentOrObject() );
		float lockComplexity = 1.0;
		if ( car )
			lockComplexity = car.Expansion_LockComplexity();

		m_Time = GetExpansionSettings().GetVehicle().PickLockTimeSeconds * lockComplexity;
		m_Cycles = 1;
		m_ToolDamagePercent = GetExpansionSettings().GetVehicle().PickLockToolDamagePercent;
	}

	override void CreateConditionComponents()
	{
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTNone;
	}

	override string GetText()
	{
		return "#STR_EXPANSION_UA_PICK_VEHICLE_LOCK";
	}
}

class ExpansionActionPickVehicleLock: ExpansionActionPickVehicleLockBase 
{
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		if ( !super.ActionCondition( player, target, item ) )
			return false;

		if ( player.IsInSafeZone() )
			return false;

		CarScript carScript = CarScript.Cast( target.GetParentOrObject() );

		if ( carScript && carScript.IsLocked() && GetExpansionSettings().GetVehicle().CanPickLock )
		{
			if ( GetExpansionSettings().GetVehicle().PickLockTools.Find( item.GetType() ) > -1 )
				return true;
		}

		return false;
	}

	override void OnFinishProgressServer( ActionData action_data )
	{
		CarScript carScript = CarScript.Cast( action_data.m_Target.GetParentOrObject() );

		if ( carScript )
		{
			float pickLockChancePercent = GetExpansionSettings().GetVehicle().PickLockChancePercent;

			#ifdef EXPANSIONMODVEHICLE
			if ( GetExpansionSettings().GetLog().VehicleLockPicking )
				GetExpansionSettings().GetLog().PrintLog("[VehicleLockPick] Player \"" + action_data.m_Player.GetIdentity().GetName() + "\" (id=" + action_data.m_Player.GetIdentity().GetId() + " pos=" + action_data.m_Player.GetPosition() + ")" + " is attempting to pick lock on " + carScript.GetType() + " (" + pickLockChancePercent + "% chance) with " + action_data.m_MainItem.GetType() );
			#endif

			if ( Math.RandomFloat( 0, 100 ) < pickLockChancePercent )
			{
				carScript.UnlockCarWithoutKey();

				#ifdef EXPANSIONMODVEHICLE
				if ( GetExpansionSettings().GetLog().VehicleLockPicking )
					GetExpansionSettings().GetLog().PrintLog( "[VehicleLockPick] Player \"" + action_data.m_Player.GetIdentity().GetName() + "\" (id=" + action_data.m_Player.GetIdentity().GetId() + " pos=" + action_data.m_Player.GetPosition() + ")" + " picked lock on " + carScript.GetType() + " with " + action_data.m_MainItem.GetType() );
				#endif
			}
			super.OnFinishProgressServer( action_data );
		}
	}
}

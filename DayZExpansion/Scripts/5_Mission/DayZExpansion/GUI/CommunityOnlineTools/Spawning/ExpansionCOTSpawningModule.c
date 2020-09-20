/**
 * ExpansionCOTSpawningModule.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionCOTSpawningModule: ExpansionCOTModuleBase
{
	void ExpansionCOTSpawningModule()
	{
		GetPermissionsManager().RegisterPermission( "Expansion.Spawning.View" );
		GetPermissionsManager().RegisterPermission( "Expansion.Spawning.Update" );
	}

	override bool HasAccess()
	{
		return GetPermissionsManager().HasPermission( "Expansion.Spawning.View" );
	}

	override string GetLayoutRoot()
	{
		return "DayZExpansion/GUI/layouts/COT/spawning/form.layout";
	}

	override string GetTitle()
	{
		return "[EX] Spawning Settings";
	}

	override string GetIconName()
	{
		return "S";
	}

	override bool ImageIsIcon()
	{
		return false;
	}

	override string GetWebhookTitle()
	{
		return "Expansion Spawn Settings";
	}

	override ref ExpansionSettingBase GetSettingsInstance()
	{
		return GetExpansionSettings().GetSpawn();
	}

	override void OnRegisterSettings()
	{
		CreateToggle( "StartingGear.UsingUpperGear", "Using Upper Gear", "", "" );
		CreateToggle( "StartingGear.UsingPantsGear", "Using Pants Gear", "", "" );
		CreateToggle( "StartingGear.UsingBackpackGear", "Using Backpack Gear", "", "" );
		CreateToggle( "UseCustomClothing", "Use Custom Clothing", "", "" );
	}

	override int GetRPCMin()
	{
		return ExpansionCOTSpawnModuleRPC.INVALID;
	}

	override int GetRPCMax()
	{
		return ExpansionCOTSpawnModuleRPC.COUNT;
	}

	override int GetRPCUpdate()
	{
		return ExpansionCOTSpawnModuleRPC.Update;
	}

	override void OnSend( ref ExpansionSettingBase setting )
	{
		if ( GetGame().IsClient() )
		{
			ScriptRPC rpc = new ScriptRPC();
			setting.OnSend( rpc );
			rpc.Send( NULL, ExpansionCOTSpawnModuleRPC.Update, true, NULL );
		} else if ( IsMissionOffline() )
		{
			GetSettingsInstance().Update( setting );
		}
	}

	override void OnSend_RPC( ref ParamsReadContext ctx, PlayerIdentity senderRPC, Object target )
	{
		if ( !IsMissionHost() )
			return;

		ref ExpansionSpawnSettings setting = new ExpansionSpawnSettings();
		if ( !setting.OnRecieve( ctx ) )
			return;

		JMPlayerInstance inst;
		if ( !GetPermissionsManager().HasPermission( "Expansion.Spawn.Update", senderRPC, inst ) )
			return;

		UpdateServer( setting, inst );
	}
};
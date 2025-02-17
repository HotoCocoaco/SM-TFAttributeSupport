/**
 * Provides a number of TF2-specific utilities that depend on gamedata to function without
 * potentially breaking on future updates.  An assortment of things that don't really belong in
 * their own specifically categorized library.
 * 
 * This includes calls to game functions and accessing various memory locations, as well as
 * provides helper functions for pointers.
 * 
 * This excludes things that require extensions not bundled with SourceMod
 * (such as DHooks, Source Scramble).
 */

#if defined __nosoop_tf2_utils_included
	#endinput
#endif

#define __nosoop_tf2_utils_included

#include <tf2_stocks>

#define TAKEHEALTH_IGNORE_MAXHEALTH		(DMG_BULLET) // Player can be overhealed with this bit set.
#define TAKEHEALTH_IGNORE_DEBUFFS		(DMG_SLASH)  // Currently unused?  Looks like it was originally for Pyro's FT healing debuff mechanism

/**
 * Attempts to heal the player for the specified amount.  This does not provide any indicators;
 * you will need to fire any related events yourself.
 * 
 * @param flHealth        Amount of health to heal.
 * @param bitsHealType    TakeHealth flags.  See TAKEHEALTH_* constants.
 * 
 * @return Amount of health granted after modifiers.
 */
native int TF2Util_TakeHealth(int client, float flHealth, int bitsHealType = 0);

/**
 * Returns the maximum amount of ammo a player can hold for the given ammo type.
 * 
 * @param iAmmoIndex      Ammo type.
 * @param playerClass     Player class.  If TFClass_Unknown, use the player's current class.
 * 
 * @return Maximum amount of ammo.
 */
native int TF2Util_GetPlayerMaxAmmo(int client, int iAmmoIndex,
		TFClassType playerClass = TFClass_Unknown);

/**
 * Returns the current maximum allowed health of an entity.
 */
native int TF2Util_GetEntityMaxHealth(int entity);

/**
 * Returns the maximum allowed overhealed health of a player, taking into account the amount
 * it can gain through overhealing by mediguns.
 * 
 * @param bIgnoreAttributes    
 * @param bIgnoreOverheal      Ignores excessive overheal.
 */
native int TF2Util_GetPlayerMaxHealthBoost(int client, bool bIgnoreAttributes = false,
		bool bIgnoreOverheal = false);

/**
 * Returns the number of conditions currently available in the game.  This value will be one
 * higher than the last valid condition.
 */
native int TF2Util_GetConditionCount();

/**
 * Returns the last valid condition.  Convenience function intended for loop termination.
 */
stock TFCond TF2Util_GetLastCondition() {
	return view_as<TFCond>(TF2Util_GetConditionCount() - 1);
}

/**
 * Given a valid condition index, stores its name in the given buffer.
 * 
 * @param cond         Condition index.
 * @param buffer       Buffer to store name.
 * @param maxlen       Maximum length of buffer.
 * @return             Number of bytes written.
 * @error              Condition index is invalid.
 */
native int TF2Util_GetConditionName(TFCond cond, char[] buffer, int maxlen);

/**
 * Returns the remaining time on a player condition, or 0.0 if the condition is not active.
 * 
 * This is just a wrapper around the internal condition list; some conditions may not accurately
 * report their duration here (such as burning).
 */
native float TF2Util_GetPlayerConditionDuration(int client, TFCond cond);

/**
 * Sets the remaining time on a player condition without reapplying the condition.
 * 
 * @error Condition is not active.
 */
native void TF2Util_SetPlayerConditionDuration(int client, TFCond cond, float duration);

/**
 * Returns the entity providing a player condition, or INVALID_ENT_REFERENCE if the condition is
 * not active.  0 is returned for "no inflictor".
 * 
 * (This is the entity described as the "inflictor" for `TF2_AddCondition`.)
 */
native int TF2Util_GetPlayerConditionProvider(int client, TFCond cond);

/**
 * Sets the entity providing a player condition without reapplying the condition.
 * 
 * (This is the entity described as the "inflictor" for `TF2_AddCondition`.)
 * 
 * @error Condition is not active.
 */
native void TF2Util_SetPlayerConditionProvider(int client, TFCond cond, int inflictor);

/**
 * Returns the remaining burn time on a given player.  By default this decreases at a fixed rate
 * over time; the rate may change based on fire resistances (such as while cloaked).
 * 
 * If the player is not burning, 0.0 is returned.
 */
native float TF2Util_GetPlayerBurnDuration(int client);

/**
 * Updates the remaining burn time on a given player.  This does not ignite players.  This
 * bypasses the 10 second maximum burn duration limitation added in Jungle Inferno; however,
 * this will be clamped the next time the game needs to update the burn duration.
 * 
 * @error Player is not already burning.
 */
native void TF2Util_SetPlayerBurnDuration(int client, float duration);

/**
 * Returns the player's shoot position.
 */
native void TF2Util_GetPlayerShootPosition(int client, float result[3]);

/**
 * Returns the healer entity index at the given position in the player's healer array.
 * To get the number of healers available, use the m_nNumHealers netprop.
 * 
 * @return Healer entity index at the given index.
 */
native int TF2Util_GetPlayerHealer(int client, int index);

/**
 * Returns whether or not the given entity is a weapon.
 * 
 * @error Entity is not valid.
 */
native bool TF2Util_IsEntityWeapon(int entity);

/**
 * Returns whether or not the given entity is a wearable.
 * 
 * @error Entity is not valid.
 */
native bool TF2Util_IsEntityWearable(int entity);

/**
 * Returns the slot for the given weapon entity.  This corresponds to the TFWeaponSlot_*
 * constants in tf2_stocks.
 * 
 * @error Entity is not valid or not a weapon.
 */
native int TF2Util_GetWeaponSlot(int entity);

/**
 * Returns the weapon entity's weapon ID.  This corresponds to the TF_WEAPON_* constants in
 * tf2_stocks, *not* the item definition index.
 * 
 * @error Entity is not valid or not a weapon.
 */
native int TF2Util_GetWeaponID(int entity);

/**
 * Returns the weapon entity's maximum clip (that is, the maximum number of times it can be
 * fired before reloading).
 * 
 * @error Entity is not valid or not a weapon.
 */
native int TF2Util_GetWeaponMaxClip(int entity);

/**
 * @return Client's wearable entity at the given index.
 * @error Index is negative or out of bounds.
 */
native int TF2Util_GetPlayerWearable(int client, int index);

/**
 * @return Number of items in the player's wearable array.
 */
native int TF2Util_GetPlayerWearableCount(int client);

/**
 * If `includeWearableWeapons` is true, weapon slots (primary, secondary, melee, utility,
 * building, pda, pda2) are also checked for wearable items.  Otherwise, wearables in those
 * slots (Sniper / Demoman shields) are ignored.
 * 
 * Note that the game validates that the player is using a class that can equip the given item;
 * any items not applicable to the class are ignored.
 * 
 * This also does not differentiate between different cosmetic slots (all cosmetics will match
 * on the first loadout slot).  If you want to get cosmetic entities on a given player, you will
 * instead want to use `TF2Util_GetPlayerWearable` / `TF2Util_GetPlayerWearableCount`.
 * Identifying specific occupied cosmetic loadout slots is currently not supported.
 * 
 * @return Entity on the given client occupying the given loadout slot.
 */
native int TF2Util_GetPlayerLoadoutEntity(int client, int loadoutSlot,
		bool includeWearableWeapons = true);

/**
 * Equips the given wearable entity onto the given client.
 * 
 * @error Client is invalid, entity is not a wearable, wearable was not spawned, or assertion
 *        failed to verify that the wearable was successfully attached (outdated gamedata).
 */
native void TF2Util_EquipPlayerWearable(int client, int wearable);

/**
 * Forces an update to the given player's speed.
 * 
 * @param immediate    Whether or not the speed is updated immediately.  If this is set to
 *                     false, the call will be defered and batched so all non-immediate calls
 *                     will only trigger one update to the player's speed at most per frame.
 *                     (This is designed to accomodate plugins using detours on player speed
 *                     calculations.)
 */
native void TF2Util_UpdatePlayerSpeed(int client, bool immediate = false);

/**
 * Returns whether or not a position / entity is in an active respawn room.  If an entity is
 * passed in, this also checks if the entity is touching an active respawn room, instead of just
 * the given position.
 * 
 * @param position               Position to check; should be the center / origin of an entity.
 * @param entity                 An optional entity to check.
 * @param bRestrictToSameTeam    Whether or not the respawn room must either match the entity's
 *                               team, or not be assigned to a team.  Always treated as true if
 *                               the position is in an active spawn room.  Has no effect if no
 *                               entity is provided.
 */
native bool TF2Util_IsPointInRespawnRoom(const float[3] position,
		int entity = INVALID_ENT_REFERENCE, bool bRestrictToSameTeam = false);

/**
 * Returns the current maximum health of the player.
 */
#pragma deprecated Misnamed; use TF2Util_GetEntityMaxHealth or TF2Util_GetPlayerMaxHealthBoost.
native int TF2Util_GetPlayerMaxHealth(int client, bool bIgnoreAttributes = false,
		bool bIgnoreOverheal = false);

// compile-time compatibility shim for tf2wearables natives
#if defined USE_TF2WEARABLE_FUNCTION_SHIMS
	#define TF2_GetPlayerLoadoutSlot    TF2Util_GetPlayerLoadoutEntity
	#define TF2_EquipPlayerWearable     TF2Util_EquipPlayerWearable
	#define TF2_IsWearable              TF2Util_IsEntityWearable
#endif

public SharedPlugin __pl_tf2utils = {
	name = "nosoop_tf2utils",
	file = "tf2utils.smx",
#if defined REQUIRE_PLUGIN
	required = 1,
#else
	required = 0,
#endif
};

# <cc>v2.0.0</c>
- <c-c4ffc4><cg>**Added**</c> The Forgotten Gauntlets!</c>
    - Finally we no longer need to rely on RobTop to add new Gauntlets, as Better Gauntlets' **Gauntlet Managers** can now create official-quality *custom Gauntlets* with ease! These new Gauntlets can be mixed in gameplay type to create a unique Gauntlet experience!
    - To celebrate the release of Better Gauntlets v2.0.0 and the capabilities for new Gauntlets, the **Star** and **Moon** Gauntlets are now available to play! Select the star next to the layer title ("The Lost Gauntlets") to see them!
- <c-c4ffc4><cg>**Added**</c> Slider navigation to the Gauntlets!</c>
    - All gauntlets now load on one page instead of three per-page, and this page is able to scroll horizontally. Use either dragging or the scroll bar at the bottom of the screen to navigate the Gauntlets.
- <c-c4ffc4><cg>**Added**</c> a new collectable, "Crystals"!</c>
    - A new form of collectable granted by completing custom Gauntlet levels.
- <c-c4ffc4><cg>**Added**</c> a new collectable, "Gauntlet Coins"!</c>
    - A new form of collectable granted by completing custom Gauntlets.
- <c-c4ffc4><cg>**Added**</c>a Leaderboard!</c>
    - Check your Crystal and Gauntlet Coin count and compare them to other Better Gauntlets players around the world!
- <c-c4ffc4><cg>**Added**</c> Profile Badges!</c>
    - Using the Badgified API, badges can now be set on managers' profiles to indicate manager status.
- <c-ffffc4><cy>**Updated**</c>: `GauntletSelectLayer` now uses the `BetterGauntletSelectLayer` class!</c>
- <c-ffffc4><cy>**Updated**</c>: `GauntletLayer` now uses the `BetterGauntletLayer` class!</c>
    - With these class changes, Better Gauntlets no longer relies on RobTop's vanilla Gauntlet layers, and can be easily updated when a new version of Geode and/or Geometry Dash releases!
- <c-ffffc4><cy>**Updated**</c> Gauntlet Info to be paged.</c>
    - Difference between "The Lost Gauntlets" and "The Forgotten Gauntlets" info will be separated by pages.
- <c-ffffc4><cy>**Updated**</c> Reload button.</c>
    - The reload button now affects the visible Gauntlet list as to not cause rate limiting on RobTop's servers.
- <c-ffffc4><cy>**Updated**</c> "The Lost Gauntlets" title sprite.</c>
- <c-ffffc4><cy>**Updated**</c> GauntletNode completion visuals.</c>
- <c-ffffc4><cy>**Updated**</c> GauntletNode BG colors.</c>
- <c-ffffc4><cy>**Updated**</c> `CreatorLayer` to show a new Gauntlet button texture!</c>
- <c-ffffc4><cy>**Updated**</c> Gauntlet Levels to show a new custom animation!</c>
- <c-ffffc4><cy>**Updated**</c> Gauntlet rewards to be claimed automatically upon completion!</c>
- <c-ffffc4><cy>**Updated**</c> support for the following Gauntlets:</c>
  - `Water, Monster, Discord`
- <c-c4ebff><cl>**Fixed**</c> Gauntlet Level fields (name, author, stars) not appearing on unlock.</c>
- <c-c4ebff><cl>**Fixed**</c> GDPS previews being locked, and/or crashing on Gauntlet preview.</c>
- <c-ffc4c4><cr>**Removed**</c> Rescale Loading Circle setting.</c>

## Known Issues:
- The new Gauntlets added by Better Gauntlets may not show up all the time, this is due to rate limiting. If you would like to support this mod and help fund the server's upkeep, please consider donating to my Kofi or Boosty!
- GDUtils' "New Gauntlet Design" setting **does not work** with this update, and that is intentional. Measures have been taken to make sure the setting does not apply due to the modifications made to `GauntletNode`. Does this count as an incompatability? Maybe. Do I care? No :P

---

## <c-ffffff>v1.7.2
- <c-ffffc4><cy>**Updated**</c> support for the following Gauntlets:</c>
  - `Split`
- <c-c4ebff><cl>**Fixed**</c> particles not appearing in the <c-f9e6aa>**Power**</c> Gauntlet.</c>

---

## <c-ffffff>v1.7.1
- <c-ffffc4><cy>**Updated**</c> support for the following Gauntlets:</c>
  - `Bonus, Time, Doom, Death, Forest, Rune, Force, Spooky, Water, Power, Toxic, Treasure, Random, Chance, Love, Cinema, Future, Utopia`
- <c-c4ebff><cl>**Fixed**</c> unintentional particles in the <c-31ff00>**Monster**</c> Gauntlet.</c>

<c-33333b>*The Gauntlet Keeper awaits your arrival...*</c>

---
 
## <c-ffffff>v1.7.0
- <c-c4ffff><cj>**Bump**</c> to Geode v5.0.0 and GD 2.2081</c>
- <c-c4ffc4><cg>**Added**</c> iOS Jit-less support!</c>
- <c-c4ffc4><cg>**Added**</c> a Level Safe!</c>
  - Shows every level found within the Gauntlet, unlocked or not.
    - Inspired by Jacob375's "Gauntlet Level Vault" mod!
- <c-ffffc4><cy>**Updated**</c> the following Gauntlets' visuals:</c>
  - `Ice, Chaos, Time, Force, Random, Chance, Love, Cinema, Future, Utopia`
- <c-c4ebff><cl>**Fixed**</c> incorrect UI placement on `GauntletSelectLayer`</c>
- <c-ffc4c4><cr>**Removed**</c> Custom Keybinds functionality</c>
- <c-ffc4c4><cr>**Removed**</c> Path Opacity setting</c>
- <c-ffc4c4><cr>**Removed**</c> Disconnect Button setting</c>
- <c-ffc4c4><cr>**Removed**</c> Toggle Paths setting</c>
- <c-ffc4c4><cr>**Removed**</c> Settings button in `GauntletSelectLayer`</c>
- <c-ffc4c4><cr>**Removed**</c> Custom Reload button (this was added to vanilla)</c>

---

## <c-ffffff>v1.6.11
- <c-c4ebff><cl>**Fixed**</c> a layering issue with GDUtils' "New Gauntlets Design" setting enabled, for real this time.</c>

---

## <c-ffffff>v1.6.1
- <c-c4ebff><cl>**Fixed**</c> a layering issue with GDUtils' "New Gauntlets Design" setting enabled.</c>
    - Reported by `AlphaQuata`. Thank you!

---

## <c-ffffff>v1.6.0
- <c-c4ffff><cj>**Bump**</c> to Geode 4.5.0.</c>
- <c-c4ffc4><cg>**Added**</c> new settings:</c>
    - "Path Blending"
        - Toggles blending between Default (No blending) and Additive.
    - "Disconnect Button"
        - Adds a disconnect button to `GauntletLayer` to simulate a disconnection from the servers.
        - This is a quick and useful way for developers to access `m_tryAgainText`.
- <c-c4ffc4><cg>**Added**</c> "Rescale Loading Circle" setting compatibility to `GauntletSelectLayer`.</c>
- <c-c4ffc4><cg>**Added**</c> new particles to `GauntletSelectLayer`!</c>
- <c-c4ffc4><cg>**Added**</c> level authors to gauntletlayer!</c>
- <c-c4ffc4><cg>**Added**</c> reactivity to the skull icon on the final level of gauntlets (finished level has darkened skull).</c>
- <c-ffffc4><cy>**Updated**</c> `GauntletSelectLayer`'s layout!</c>
- <c-ffffc4><cy>**Updated**</c> Settings' categorization (categorized by game layer).</c>
- <c-ffffc4><cy>**Updated**</c> particles settings to affect `GauntletSelectLayer`.</c>
- <c-ffffc4><cy>**Updated**</c> `m_tryAgainText` (the text that pops up when youre disconnected from the servers)</c>
- <c-ffffc4><cy>**Updated**</c> Gauntlet Credits to have newlines if a name is too long.</c>
- <c-c4ebff><cl>**Fixed**</c> a grammatical issue with the "Rescale Loading Circle" setting.</c>
- <c-c4ebff><cl>**Fixed**</c> the Settings Button on `GauntletSelectLayer` being larger than the sprite itself.</c>

---

## <c-ffffff>v1.5.2
- <c-c4ffc4><cg>**Added**</c> Path Opacity Editing!</c>
    - Found in Experimental Settings, you can change the opacity of your Gauntlet Path Dots.
- <c-c4ffc4><cg>**Added**</c> a <cg>settings button</c> in the Gauntlet Select menu!</c>
    - Change Better Gauntlet's settings right within the menu itself!</c>
- <c-c4ffc4><cg>**Added**</c> Gauntlet Description credits!</c>
    - Most of the descriptions are written by <cj>GitHub Copilot</c>. Let me know if you'd like to write one!
- <c-ffffc4><cy>**Updated**</c>  the following Gauntlets' visuals:</c>
    - `Crystal, Lava, Death, Forest, Portal`
- <c-c4ebff><cl>**Fixed**</c> Gauntlet Level Vault's info button placement on the Gauntlet Select menu.</c>

---

## <c-ffffff>v1.5.1
- <c-c4ebff><cl>**Fixed**</c> an error in the Changelog.</c>

---

## <c-ffffff>v1.5.0
- <c-c4ffff><cj>**Bump**</c> to Geode 4.4.0.</c>
- <c-c4ffc4><cg>**Added**</c> iOS Support!</c>
- <c-c4ffc4><cg>**Added**</c> support to the following Gauntlets:</c>
    - `Space, Cosmos`
- <c-ffffc4><cy>**Updated**</c>  the following Gauntlets' particles:</c>
    - `Fire, Poison, Demon, Death, Christmas`
- <c-ffc4c4><cr>**Removed**</c> the Gauntlet Level Vault __dependency__. It it still recommended you install, though!</c>
- <c-ffc4c4><cr>**Removed**</c> the Custom Keybinds dependency for iOS as it's not available yet.</c>


---

## <c-ffffff>v1.4.2
- April Fools update :)</c>

---

## <c-ffffff>v1.4.1
- <c-c4ffff><cj>**Bump**</c> to Geode 4.3.0.</c>
- <c-c4ebff><cl>**Fixed**</c> the background in the <c-f9aaf4>**Split**</c> Gauntlet from being properly loaded.</c>

---

## <c-ffffff> v1.4.0
- <c-c4ffc4><cg>**Added**</c> Gauntlet information popups.</c>
- <c-c4ffc4><cg>**Added**</c> particles to the following Gauntlets:</c>
    - `Water, Treasure, Portal, NCS 1, NCS 2`
- <c-ffffc4><cy>**Updated**</c>  the following Gauntlets' particles:</c>
    - `Fire, Poison, Shadow, Bonus, Chaos, Demon, Spike, Forest`

---

## <c-ffffff>v1.3.0
- <c-c4ffff><cf>Integrated</c> [**Gauntlet Level Vault**](mod:jacob375.gauntletlevelvault) by Jacob375!</c>
    - This mod is recommended you download!
- <c-c4ffc4><cg>**Added**</c> a fourth chain to `GauntletSelectLayer`.</c>
- <c-c4ffc4><cg>**Added**</c> a new setting: "Islands Hover".</c>
- <c-c4ffc4><cg>**Added**</c> a new setting: "Background Particles".</c>
- <c-c4ffc4><cg>**Added**</c> a new setting: "Enable Gauntlet Paths".</c>
- <c-c4ffc4><cg>**Added**</c> a new setting: "Toggle Vault Tooltip".</c>
- <c-c4ffc4><cg>**Added**</c> an old setting: "Rescale Loading Circle".</c>
- <c-ffffc4><cy>**Updated**</c> <c-fcee76>**Better Gauntlets**</c>' Node IDs.</c>
- <c-ffffc4><cy>**Updated**</c> the following Gauntlets' visuals:</c>
    - `Fire, Ice, Shadow, Lava, Bonus, Chaos, Demon, Time, Crystal, Magic, Spike, Doom, Forest, Haunted, Power, Halloween, Christmas`
- <c-ffffc4><cy>**Updated**</c> the <c-6199ff>**Discord**</c> Gauntlet sprite.</c>
- <c-ffffc4><cy>**Updated**</c> the Gauntlet floor sprite.</c>
- <c-ffffc4><cy>**Updated**</c> all `.md` files' formatting.</c>
- <c-c4ebff><cl>**Fixed**</c> unintentional particles in the <c-1dffff>**NCS I**</c> Gauntlet.</c>
- <c-c4ebff><cl>**Fixed**</c> another crash when using a keybind to enter a level before the layer finished loading.</c>
- <c-ffc4c4><cr>**Removed**</c> the Discord Link from `support.md` since it's already in `mod.json`</c>
- <c-ffc4c4><cr>**Removed**</c> the "Update Discord Sprite" setting.</c>


---

## <c-ffffff>v1.2.1
- <c-c4ebff><cl>**Fixed**</c> platform requirements for Custom Keybinds.</c>

---

## <c-ffffff>v1.2.0
- <c-c4ffff><cj>**Bump**</c> to Geode 4.2.0.</c>
- <c-c4ffff><cj>**Bump**</c> Node IDs dependency from <cr>v1.15.0</c> -> <cg>**v1.19.0**</c>
- <c-c4ffc4><cg>**Added**</c> keybind support on all platforms!</c>
- <c-c4ffc4><cg>**Added**</c> a fallback function for unsupported/custom Gauntlets.</c>
- <c-c4ffc4><cg>**Added**</c> hover movements to the Gauntlet levels.</c>
- <c-ffffc4><cy>**Updated**</c> the mod logo to unify this mod with the vanilla Gauntlets' style.</c>
- <c-ffffc4><cy>**Updated**</c> a LOT of backend/file stuff. Thank you so so SO much, [Capeling]((user:18226543))!</c>
- <c-ffffc4><cy>**Updated**</c> the following Gauntlets' visuals:</c>
    - `Fire, Ice, Poison, Shadow, Lava, Bonus`
- <c-ffffc4><cy>**Updated**</c>  `GauntletLayer` keybind names and descriptions.</c>
- <c-c4ebff><cl>**Fixed**</c> `support.md` showing an incorrect Ko-fi hyprlink.</c>
- <c-c4ebff><cl>**Fixed**</c> the back controller button sprite being behind the UI on `GauntletSelectLayer`.</c>
- <c-c4ebff><cl>**Fixed**</c> a bug where re-loading `GauntletLayer` would disable it's keybinds after one use.</c>

---

## <c-ffffff>v1.1.3
- <cr>jaydasshu</c> -> <cg>**DasshuDEV**</c>
- <c-c4ffc4><cg>**Added**</c> Corner UI to `GauntletLayer`, responsive to the selected Gauntlets' header text color.</c>
- <c-ffffc4><cy>**Updated**</c> the following Gauntlets' visuals:</c>
    - `Fire, Ice`
- <c-ffffc4><cy>**Updated**</c> the "The Lost Gauntlets" title to be a part of the UI.</c>
- <c-ffffc4><cy>**Updated**</c> Gauntlet spacing a bit when using GDUtils' "New gauntlets design" setting.</c>
- <c-ffffc4><cy>**Updated**</c> some text and background colors.</c>
- <c-ffffc4><cy>**Updated**</c> the Gauntlets' customizations to apply before the levels load.</c>
- <c-c4ebff><cl>**Fixed**</c> a crash when using a keybind to enter a level before it's finished loading.</c>

---

## <c-ffffff>v1.1.0 - v1.1.2
- <c-c4ffc4><cg>**Added**</c> particles around the stars of completed Gauntlet levels.</c>
- <c-c4ffc4><cg>**Added**</c> a new setting: "Update Discord Sprite".</c>
    - Changed the sprite of the <c-6199ff>**Discord**</c> Gauntlet to match with the company's current logo.
- <c-c4ffc4><cg>**Added**</c> all background changes to the UI, meaning all extra stuff will load even if the gauntlets themselves don't.</c>
- <c-c4ffc4><cg>**Added**</c> support for the <c-31ffff>**Space**</c> and <c-f9c800>**Cosmos**</c> Gauntlets.</c>
- <c-ffffc4><cy>**Updated**</c> the <c-00ffff>**Time**</c> Gauntlet's background (07 -> 17).</c>
- <c-ffffc4><cy>**Updated**</c> the opacity of the floor texture from 255 -> 128.</c>
- <c-ffffc4><cy>**Updated**</c> the particles in `GauntletSelectLayer` to add atmosphere to the space.</c>
- <c-ffffc4><cy>**Updated**</c> the background to add to the atmosphere.</c>
- <c-c4ebff><cl>**Fixed**</c> a crash on Android when entering `GauntletSelectLayer`.</c>
- <c-c4ebff><cl>**Fixed**</c> an issue with Gauntlet placement.</c>
- <c-c4ebff><cl>**Fixed**</c> overlapping textures when using GDUtils' "New gauntlets design" setting.</c>
- <c-ffc4c4><cr>**Removed**</c> some unnecessary files.</c>

---

## <c-ffffff>v1.0.0
- Initial release of <cc>**Better Gauntlets**</c>!
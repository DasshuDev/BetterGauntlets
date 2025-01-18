## <cg>v1.1.31</c> | <cy>(xx/xx/2025)</c>
- Updated the mod logo slightly.
- Updated a LOT of backend/file stuff.
- Updated Node IDs dependency from <cr>v1.15.0</c> -> <cg>**v1.19.0**</c>
- Fixed the back controller button sprite being behind the corner UI on `GauntletSelectLayer`.

## <cg>v1.1.3</c> | <cy>(1/11/2025)</c>
- <cr>jaydasshu</c> -> <cg>**DasshuDEV**</c>
- Added Corner UI to `GauntletLayer`, responsive to the selected Gauntlets' header text.
- Added particles to the <c-f9c800>Fire</c> and <c-92ffff>Ice</c> Gauntlets.
- Added loading text to the loading circle.
- Added a new setting: "Re-scale Loading Circle".
    - Allows for scaling of the loading circle from 0.75 -> 1.25
- Updated the "The Lost Gauntlets" title to be a part of the UI.
- Updated Gauntlet spacing a bit when using GDUtils' "New gauntlets design" setting.
- Updated some text and background colors.
- Updated the Gauntlets' customizations to apply before the levels load.
- Fixed a crash when using a keybind to enter a level before it's finished loading.
- ~~<cr>~~There is a known visual bug when a controller is connected. __This is a Node IDs bug__ that I will try to work around in a future update!~~
    - <cg>This has been fixed in Node IDs v1.19.0!</c>

## <cg>v1.1.2</c> | <cy>(12/20/2024)</c>
- Updated the particles in `GauntletSelectLayer` to add atmosphere to the space.
- Updated the background to add to the atmosphere.
- Fixed an issue with Gauntlet placement.
- Removed some unnecessary files.

## <cg>v1.1.1</c> | <cy>(N/A)</c>
- Updated the opacity of the floor texture from 255 -> 128.
- Fixed overlapping textures when using GDUtils' "New gauntlets design" setting.

## <cg>v1.1.0</c> | <cy>(N/A)</c>
- Added particles around the stars of completed Gauntlet levels.
- Added a new setting: "Update Discord Sprite".
    - Changes the sprite of the <c-6199ff>Discord</c> Gauntlet to match with the company's current logo.
- Added all background changes to the UI, meaning all extra stuff will load even if the gauntlets themselves don't.
- Added support for the <c-31ffff>Space</c> and <c-f9c800>Cosmos</c> Gauntlets.
- Updated the <c-00ffff>Time</c> Gauntlet's background (07 -> 17).
- Fixed a crash on Android when entering `GauntletSelectLayer`. Thank you so much, [Jasmine (hiimjustin000)](user:7466002)!!

## <cg>v1.0.0</c> | <cy>(12/12/2024)</c>
- Initial release of <cs>**Better Gauntlets**</c>!
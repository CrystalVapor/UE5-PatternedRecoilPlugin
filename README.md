# Crystal Recoil

## Introduction

This plugin is inspired by RecoilSystem which was written by a Russian Programmer, and is **a complete rewrite** of the original plugin.

Crystal's Recoil allows you to add templated recoil effects to your shooting game, and you can use a GUI interface to adjust the recoil template and other detailed parameters.

![Editor Screenshot](Resources/Screenshot.png)

## Features

- Patterned Recoil
- Full GUI Interface
- Easy-to-use Code Interface
- Blueprints Fully Accessible
- Support Recoil Recovery/Compensation (Not Finished)

## Installation

1. Get `CrystalRecoil.zip` from the [releases](https://github.com/Solessfir/CrystalRecoil/releases)
2. Extract it into your project's `Plugins` folder.
3. Enjoy!

## Usage

1. Add the component `UCRRecoilComponent` to an Actor (Player Pawn, FireArm or other Actor)
2. Implement the interface `ICRRecoilInterface` for the Actor that has the `CRRecoilComponent`
3. Override the function `ICRRecoilInterface::GetRecoilComponent` to return the corresponding recoil component
4. Override the function `ICRRecoilInterface::GetTargetController` to return the player controller that needs to apply the recoil
5. Create an asset `CRRecoilPattern` and set a suitable recoil behavior in the editor
6. Call `UCRRecoilComponent::SetRecoilPattern` to set the recoil pattern for the recoil component at the appropriate time
7. Call `ICRRecoilInterface::StartShooting` at the beginning of each burst (Usually every LMB pressed), and call `ICRRecoilInterface::ApplyShot` after each shot

## Recoil Pattern Editor Shortcuts

- **N**: Add Unit
- **Del**: Remove
- **Ctrl+A**: Select All
- **Ctrl+C/V**: Copy/Paste
- **Shift+S**: Snap
- **S**: Scale
- **R**: Arrange
- **O**: Reset View
- **H**: Toggle Shortcuts

## Recoil Implementation Details

The way the plugin simulates recoil is:

First, determine the vector distance the reticle needs to move based on the coordinates in the template

Then calculate the initial velocity of the reticle required using the reticle rise time and braking acceleration written in the recoil template

Then convert the initial velocity to angular momentum and apply it, gradually applying the braking acceleration until the reticle reaches the predetermined position

## Additional Information

The plugin comes with a `CRSpreadRecoilComponent`, which can be used to add random spread effects when shooting.

The spread effect is based on three `FRichCurve` curves:

`ShotToHeatCurve` - Defines the heat added by each shot

`HeatToSpreadAngleCurve` - Defines the spread angle corresponding to the current heat

`HeatToCooldownPerSecondCurve` - Defines the heat that cools down per second under the current heat

After setting the three curves, you can call `UCRSpreadRecoilComponent::GetCurrentSpreadAngle` to get the current spread angle when calculating the ray before shooting

## License

This plugin is licensed under the MIT License
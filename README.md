# Crystal Recoil

## 介绍 | Introduction

本插件是由一位俄罗斯程序员编写的RecoilSystem插件所启发的，是原始插件的**完全重写**。
This plugin is inspired by RecoilSystem which was wrote by a Russian Programmer, and is **a complete rewrite** of the original plugin.

Crystal's Recoil 允许你为自己的射击游戏添加模板化的后坐力效果，并且可以使用一个GUI界面来调整后坐力模板和其他细节参数。
Crystal's Recoil allows you to add templated recoil effects to your shooting game, and you can use a GUI interface to adjust the recoil template and other detailed parameters.

## 特性 | Features

- 模板化后坐力  | Patterned Recoil
- 完全的GUI界面  | Full GUI Interface
- 易于使用的代码接口  | Easy-to-use Code Interface
- 完全的蓝图支持 | Blueprints Fully Accessible
- 支持后坐力回复/补偿(未完成)  | Support Recoil Recovery/Compensation (Not Finished)

## 安装 | Installation

1. 下载最新的插件文件  | Download the latest plugin file
2. 将插件文件放入您的项目的`Plugins`文件夹中  | Put the plugin file into the `Plugins` folder of your project
3. 向你的项目文件中添加插件依赖  | Add the plugin dependency to your project file
4. 启动Unreal Editor并启用插件  | Launch Unreal Editor and enable the plugin
5. 享受使用  | Enjoy using

## 依赖/额外安装条件 | Dependencies/Extra Installation Conditions

- 无  | None

## 使用 | Usage

1. 为Actor添加组件`UCRRecoilComponent`(玩家Pawn, 枪械或者其他Actor都可以) | Add the component `UCRRecoilComponent` to an Actor (Player Pawn, FireArm or other Actor)
2. 为拥有`CRRecoilComponent`的Actor实现接口`ICRRecoilInterface` | Implement the interface `ICRRecoilInterface` for the Actor that has the `CRRecoilComponent`
3. 覆写函数`ICRRecoilInterface::GetRecoilComponent`，使其返回相应的后坐力组件 | Override the function `ICRRecoilInterface::GetRecoilComponent` to return the corresponding recoil component
4. 覆写函数`ICRRecoilInterface::GetTargetController`，使其返回需要应用后坐力的玩家控制器 | Override the function `ICRRecoilInterface::GetTargetController` to return the player controller that needs to apply the recoil
5. 创建资产`CRRecoilPattern`，并使用编辑器设置一个合适的后坐力行为 | Create an asset `CRRecoilPattern` and set a suitable recoil behavior in the editor
6. 在合适的时候调用`UCRRecoilComponent::SetRecoilPattern`为后坐力组件设置后坐力模板 | Call `UCRRecoilComponent::SetRecoilPattern` to set the recoil pattern for the recoil component at the appropriate time
7. 在每一次连射的开始调用`ICRRecoilInterface::StartShooting`，在每一枪射击后调用`ICRRecoilInterface::ApplyShot` | Call `ICRRecoilInterface::StartShooting` at the beginning of each burst(Usually every LMB pressed), and call `ICRRecoilInterface::ApplyShot` after each shot

## 后坐力实现 | Recoil Implement Details

插件模拟后坐力的方式是： | The way the plugin simulates recoil is:

首先根据模板中的坐标确定准星需要移动的矢量路程 | First, determine the vector distance the reticle needs to move based on the coordinates in the template

然后使用在后坐力模板中写入的准星抬升时间和制动加速度计算所需的准星初速度 | Then calculate the initial velocity of the reticle required using the reticle rise time and braking acceleration written in the recoil template

随后将初速度转换为角动量并施加，在过程中逐渐应用制动加速度直到准星到达预定位置 | Then convert the initial velocity to angular momentum and apply it, gradually applying the braking acceleration until the reticle reaches the predetermined position

## 额外信息 | Additional Information

插件附带一个SpreadRecoilComponent，它可以用于在射击时添加随机的扩散效果。  | The plugin comes with a SpreadRecoilComponent, which can be used to add random spread effects when shooting.

扩散效果基于三条`FRichCurve`曲线 | The spread effect is based on three `FRichCurve` curves

`ShotToHeatCurve` - 定义了每一枪射击增加的热量 | Defines the heat added by each shot

`HeatToSpreadAngleCurve` - 定义了当前热量对应的扩散角度 | Defines the spread angle corresponding to the current heat

`HeatToCooldownPerSecondCurve` - 定义了当前热量下每秒冷却的热量 | Defines the heat that cools down per second under the current heat

三条曲线设置完成后，可以在开枪前计算射线时调用`UCRSpreadRecoilComponent::GetCurrentSpreadAngle`来获取当前的扩散角度 | After setting the three curves, you can call `UCRSpreadRecoilComponent::GetCurrentSpreadAngle` to get the current spread angle when calculating the ray before shooting

## 许可证 | License

本插件使用MIT许可证  | This plugin is licensed under the MIT License

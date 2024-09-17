# Crystal Recoil

## Introduction | 介绍

本插件是由一位俄罗斯程序员编写的RecoilSystem插件所启发的，是原始插件的**完全重写**。
This plugin is inspired by RecoilSystem which was wrote by a Russian Programmer, and is **a complete rewrite** of the original plugin.

Crystal's Recoil 允许你为自己的射击游戏添加模板化的后坐力效果，并且可以使用一个GUI界面来调整后坐力模板和其他细节参数。
Crystal's Recoil allows you to add templated recoil effects to your shooting game, and you can use a GUI interface to adjust the recoil template and other detailed parameters.

## Features | 特性

- 模板化后坐力  | Templated Recoil
- 完全的GUI界面  | Full GUI Interface
- 易于使用的代码接口  | Easy-to-use Code Interface
- 支持后坐力回复/补偿(未完成)  | Support Recoil Recovery/Compensation (Not Finished)

## Installation | 安装

1. 下载最新的插件文件  | Download the latest plugin file
2. 将插件文件放入您的项目的`Plugins`文件夹中  | Put the plugin file into the `Plugins` folder of your project
3. 向你的项目文件中添加插件依赖  | Add the plugin dependency to your project file
4. 启动Unreal Editor并启用插件  | Launch Unreal Editor and enable the plugin
5. 享受使用  | Enjoy using

## 依赖/额外安装条件 | Dependencies/Extra Installation Conditions

- 无  | None

## Usage | 使用

### C++ Interface | C++接口

1. 为你的玩家Pawn类继承接口 `ICRRecoilInterface` (必须在C++内继承)  | Inherit the interface `ICRRecoilInterface` for your player Pawn class (must be done in C++)
2. 为你的玩家Pawn类添加组件 `UCRRecoilComponent`  | Add the component `UCRRecoilComponent` for your player Pawn class
3. 在玩家Pawn类中实现接口函数 `ICRRecoilInterface::GetRecoilComponent`  和 `GetTargetController`  | Implement the interface functions `ICRRecoilInterface::GetRecoilComponent` and `GetTargetController` in your player Pawn class
4. 在合适的时候调用 `UCRRecoilComponent::SetRecoilPattern` 为玩家Pawn设置后坐力模板  | Call `UCRRecoilComponent::SetRecoilPattern` to set the recoil template for the player Pawn at the right time
5. 在合适的时候调用 `ICRRecoilInterface::StartShooting` 和 `StopShooting`  来开始和停止射击（这两个函数是为了决定何时开始后坐力恢复）  | Call `ICRRecoilInterface::StartShooting` and `StopShooting` at the right time to start and stop shooting (these two functions are to determine when to start recoil recovery)
6. 在调用了 `StartShooting` 之后，在每一枪射击后调用 `ICRRecoilInterface::ApplyShot`  来应用后坐力  | After calling `StartShooting`, call `ICRRecoilInterface::ApplyShot` after each shot to apply recoil

### Blueprint configuration | 蓝图配置

1. 创建数据资产 `RecoilPattern`  | Create an DataAsset `RecoilPattern`
2. 打开数据资产并配置后坐力模板  | Open the DataAsset and configure the recoil template
3. 在合适的时候为玩家的后座力组件设置后座力模板  | Set the recoil template for the player's recoil component at the right time

## 额外信息 | Additional Information

插件附带一个SpreadRecoilComponent，它可以用于在射击时添加随机的扩散效果。  | The plugin comes with a SpreadRecoilComponent, which can be used to add random spread effects when shooting.

## License | 许可证

本插件使用MIT许可证  | This plugin is licensed under the MIT License

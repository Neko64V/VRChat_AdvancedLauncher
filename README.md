## 概要
VRChatの起動オプションをGUIベースで変更可能にしたちょっと便利なランチャー。  
いちいちSteamやショートカットを編集しなくても簡単に起動構成がいじれるのでいろいろ捗ります。
![image](https://github.com/user-attachments/assets/a9959b92-e88d-43b5-a6fa-3213ce16cf4c)

## 使い方
1. setup -> setup.batを実行
2. VRChatAdvancedLauncher.exeを実行

## Restarter
### 概要
VRChatのTimeoutを検出したら自動で元居たワールドに戻ります。 
元居た位置には戻れません。ワールド側で対応していると普通に戻れます。  

#### 注意
* 複数のクライアントが起動中の場合、最後に起動されたクライアントを監視します。

## 機能について
GUIメニューの各機能について軽く説明します。
### Display
* DesktopMode : デスクトップモードで起動する
* FullScreen  : フルスクリーンで起動する
* FPS Limit   : FPSを制限する
* Monitor     : VRChatを表示するモニター。モニターのIDについては設定を参照。

### Test
* Offline : オフラインモードで起動する
* Avatar Test : ローカルテスト中のアバターがアップデートされた場合に自動で再度読み込みを行う。
* World Test  : ローカルテスト中のワールドがアップデートされた場合に自動で再度読み込みを行う。

### Misc
* Profile : アカウントを切り替えることができる。
* CCX Option
```
一部のRyzen CPUユーザー限定。  
CCXを超えての処理が行われなくなるのでレイテンシが減りパフォーマンスが向上する（らしい）
```

## Restarter
### AutoRestarter
* AutoRestarter : VRChatのタイムアウトを検出すると自動で再起動を行う。全ての起動オプションが無視される。

## Path
* VRChat Path : VRChatのインストール先。基本的にいじらなくていいはず？

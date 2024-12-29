## 概要
VRChatの起動オプションをGUIベースで変更可能にしたちょっと便利なランチャー。  
いちいちSteamやショートカットを編集しなくても簡単に起動構成がいじれるのでアバター改変が捗ります。

## 使い方
1. setup -> setup.batを実行します。
2. VRChatAdvancedLauncher.exeを実行します。
3. 以上です。

## 機能
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

### Restarter
## AutoRestarter
* AutoRestarter : VRChatのタイムアウトを検出すると自動で再起動を行う。全ての起動オプションが無視される。

## Path
* VRChat Path : VRChatのインストール先

## Restarter
* VRChatのクライアントでTimeoutを検出したら自動で元居たワールドに戻ります。
* 複数のクライアントが起動中の場合、最後に起動されたクライアントを監視します。
* ここで起動されるのは、起動オプション無しのVRChatのクライアントです。
* 誤動作防止の為にAppData内のVRChatフォルダにあるログファイルを全て削除します。必要に応じてバックアップを取っておいてください。

## config.json
AppData->Local->VRChatAdvancedLauncher 内にあります。  
VRChatのインストール先やLauncherの設定が保存されており、自動でセーブ/ロードされます。

## 概要
VRChatの起動オプションをGUIベースで変更可能にしたちょっと便利なランチャー。  
いちいちSteamやショートカットを編集しなくても簡単に起動構成がいじれるのでアバター改変が捗ります。

## 使い方
1. setup -> setup.batを実行します。
2. VRChatAdvancedLauncher.exeを実行します。
3. 以上です。

## Restarter
* VRChatのクライアントでTimeoutを検出したら自動で元居たワールドに戻ります。
* 複数のクライアントが起動中の場合、最後に起動されたクライアントを監視します。
* ここで起動されるのは、起動オプション無しのVRChatのクライアントです。
* 誤動作防止の為にAppData内のVRChatフォルダにあるログファイルを全て削除します。必要に応じてバックアップを取っておいてください。

## config.json
AppData->Local->VRChatAdvancedLauncher 内にあります。  
VRChatのインストール先やLauncherの設定が保存されており、自動でセーブ/ロードされます。

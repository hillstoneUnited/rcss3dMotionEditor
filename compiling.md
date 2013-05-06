---
layout: layout
title: compiling
---
##コンパイルと実行方法
ソースコードをダウンロードして下さい。  
プロジェクトページから圧縮ファイルをダウンロードして解凍するか  
`$ git clone git://github.com/hillstoneUnited/rcss3dMotionEditor.git`  
コマンドを利用して下さい。


コンパイルをするためにQtをインストールして下さい。  
GUIを作成するのに利用しているライブラリです。
Qt5.0.2にて動作確認をしています。
Qtを利用するのに便利な統合開発環境であるQtCreatorも同時にインストールされます。


<http://qt-project.org/downloads>  
からインストールに必要なファイルをダウンロード

端末から  
`$ chmod +x qt-linux-opensource-5.0.2-（以下略）`  
で実行権限を付与

`./（上で権限を付けたファイル）`  
で実行

windowが出てくるので場所を指定してインストールするものを選択  
使用に関する注意を確認したらインストール

QtCreatorを起動し、ファイル→ファイル/プロジェクトを開くから  
rcss3dMotionEditor.pro
を選択します。

ロボカップサーバーを起動してから実行します（必要に応じてコンパイルも行われます）。  


※  
rcssnet/tcpsocket.hppなどの、ロボカップサーバーと同時にインストールされるライブラリを利用します。  
それらのファイルが見つからないと言われた場合は、rcss3dMotionEditor.proファイルの  
INCLUDEPATH、DEPENDPATHをお使いの環境に合わせて書きなおして下さい。  
（apt-getコマンドでインストールした場合は、/usr/(それぞれincludeとlib)/simsparkに入っていると思います。）

# 7z_expl
## Linux(Ubuntu)の場合
[Gist](https://gist.github.com/cc5efd7b0/3b4e9a863d24871d50179cc04b5c7a44)に置いてみるなど。  
Windows用と若干機能が違うかもしれない。  
23行目の`/home/cc5e/7z/7zz`を7zzがある場所に変更して-std=c++17オプションをつけてコンパイルします。
あとzenityをapt installしておきます。  
使えることを確認したらzipとかをバイナリに関連付け。
## 注意
レジストリをいじります。
## これは何
zipファイルなどをダブルクリックすれば勝手に展開し、エクスプローラで展開ディレクトリを開きます。  
[https://qiita.com/YMT2520/items/5a0811e5ccee922b7192](https://qiita.com/YMT2520/items/5a0811e5ccee922b7192)とほぼ同じですが...。  
ChatGPTに書かせただけですが...。
C++で作りたかったから作った、ただそれだけです(~~作らせた、の間違いでは~~)。
## 使う
### 下拵え?
前準備として、7-Zipをインストールします(あたりまえ)。  
インストール場所は`C:\Program Files\7-Zip`。  
既に入っていて、インストール場所が違うならソースコードを書き換えてビルドし直すか、上記の場所へインストールし直します。  
7-Zip File Manager(7zFM.exe)を管理者権限で開き、オプション→システムを開きます。  
`7z`,`bz2`,`bzip2`,`gz`,`gzip`,`rar`,`tbz`,`tbz2`,`tgz`,`txz`,`xz`,`zip`,`zst`のすべてのユーザーのところをクリックし、7-Zipとします。  
### ファイルの配置など
Releaseからzipを落として展開。  
`7z_expl.exe`を`C:\7z_expl`フォルダに配置します。フォルダは当然存在しないので作成しておきます。  
次に、同梱のbundle.regを実行します。  
bundle.reg内には
```
[HKEY_CLASSES_ROOT\7-Zip.7z\shell\open\command]
@="\"C:\\7z_expl\\7z_expl.exe\" \"%1\""
```
みたいなのがたくさんあると思います。必要に応じて削除なり追加なりします。

## ソースからビルド
`src/main.cpp`の1行目にコンパイルコマンドがあります。  
そういえばReleaseバイナリはClang++でビルドしています。g++はなんかいや。

# ニンテンドースイッチ用の吉里吉里

![吉里吉里アイコン](icon.jpg)

Please read [README.en_US.md](https://github.com/uyjulian/krkrs/blob/master/README.en_US.md) for English documentation.  

このリポジトリには、ニンテンドースイッチに移植された吉里吉里互換のコアが含まれています。  
吉里吉里は、ほとんど常に排他的に[Kirikiri Adventure Game 3 （KAG3）](https://github.com/krkrz/kag3)ビジュアルノベルエンジン（またはその派生物）で使用されますが、[E-mote](https://emote.mtwo.co.jp/)などのデスクトップアプリケーションおよびユーティリティ（[Electron](https://www.electronjs.org/)の使用例と同様）にも使用できます。

## 建物

`dkp-pacman`を使用して前提条件のライブラリをインストールした後、単純な`mkdir b && cd b && cmake .. && make`で`krkrs.nro`を生成します。  
エンジンのコンパイル中に「ファイルが見つかりません」または「ライブラリが見つかりません」エラーが発生した場合は、 `dkp-pacman`から不足しているライブラリをインストールしてください。

## RomFS統合

ゲームを1つの`nro`ファイルに統合するには、次のようにします。
* 「Makefile」の43行目をコメント解除します
* ゲームファイルを「romfs」という名前のディレクトリに配置します
* すべてのファイルとフォルダの名前を小文字に変更します
* 「ビルド」セクションの説明に従ってビルドします。

ファイルとフォルダ名が小文字に変換されていない場合、ファイルが見つからないときにエラーが発生することに注意してください。  
また、ファイル名が7ビットセーフでない場合、ファイルにアクセスできませんのでご注意ください。 その場合は、XP3アーカイブをご利用ください。

## プロジェクトディレクトリの選択

RomFSに`startup.tjs`が存在する場合、プロジェクトディレクトリと現在のディレクトリは`romfs:/`に設定されます。  
上記の条件が満たされない場合、プロジェクトディレクトリは現在のディレクトリに設定されます（通常、 `krkrs.nro`があるディレクトリと同じディレクトリ）。  

## プラグイン

現在、外部プラグインの使用はサポートされていません。  
次の内部「プラグイン」がサポートされています。
* [csvParser](https://github.com/wtnbgo/csvParser)
* dirlist
* [fftgraph](https://github.com/krkrz/fftgraph)
* [getSample](https://github.com/wtnbgo/getSample)
* layerExPerspective
* [saveStruct](https://github.com/wtnbgo/saveStruct)
* [varfile](https://github.com/wtnbgo/varfile)
* [win32dialog](https://github.com/wtnbgo/win32dialog)
* wutcwf

内部プラグインの機能を使用するには、プラグインの名前で `Plugins.link`を呼び出す必要があります。 例：
```js
Plugins.link("csvParser.dll");
```
その後、プラグインの機能を使用できます。
```js
var x = new CSVParser();
```

## 書体

フォントを使用するには、 `Font.addFont`を使用して追加する必要があります。
使用例：
```js
Font.addFont("fonts/meiryo.ttf");
```

## テキストファイルのエンコード

BOMを使用したUTF-16LEエンコードが推奨されます。これは、再エンコードや解凍を行わなくてもストリーミングできるためです。  
エンコード検出のルールは次のとおりです。  
最初のバイトが0xfe 0xfeの場合、ファイルはデコード/解読され、エンコードの検出が続行されます。  
最初のバイトが0xff 0xfe（UTF-16LE BOM）の場合、ファイルはUTF-16LEとしてロードされます。  
最初のバイトが0xef 0xbb 0xbf（UTF-8 BOM）の場合、ファイルはUTF-8としてロードされます。  
それ以外の場合、エンコーディングは `-readencoding`コマンドラインオプションを使用して設定されます。  
TVP_TEXT_READ_ANSI_MBCSが定義されている場合、デフォルト値はShift_JISになります。  
それ以外の場合は、UTF-8になります。 有効なオプションは、 `""`、 `"UTF-8"`、 `"Shift_JIS"`、 `"GBK"`です。  
オプションが `""`の場合、デコードは `"Shift_JIS"`、 `"UTF-8"`、 `"GBK"`の順に試行されます。  
デコードに失敗すると、 `TJSNarrowToWideConversionError`がスローされます。  

## アーカイブのサポート

このエンジンを使用してサポートされるアーカイブは、暗号化されていないXP3アーカイブのみです。  
アーカイブは、SDカードファイルシステムまたはRomFSに配置できます。  
アーカイブを使用すると、ファイル名のエンコーディングとファイル名の大文字と小文字の問題を軽減できます。  
アーカイブには、アーカイブ区切り文字を使用してアクセスできます（デフォルトは`>`で、`-arcdelim`コマンドライン引数で設定できます）。  
使用例：  
```js
Scripts.execStorage("archive.xp3>file.tjs");
```

## サードパーティプロジェクトの使用

次のプロジェクトのコードが使用されました：
* [libnx](https://github.com/switchbrew/libnx)
* [ffmpeg](http://ffmpeg.org/)
* [zlib](https://www.zlib.net/)
* [libpng](http://www.libpng.org/)
* [libjpeg-turbo](https://libjpeg-turbo.org/)
* [libwebp](https://developers.google.com/speed/webp/download)
* [libass](https://github.com/libass/libass)
* [fribidi](http://fribidi.org/)
* [Opus](http://opus-codec.org/)
* [Vorbis](https://xiph.org/vorbis/)
* [Simple DirectMedia Layer (SDL) 2](https://www.libsdl.org/)
* [Freetype](https://www.freetype.org/)
* [Oniguruma](https://github.com/kkos/oniguruma)
* [bzip2](https://www.sourceware.org/bzip2/)
* [A C-program for MT19937](http://www.math.sci.hiroshima-u.ac.jp/~m-mat/MT/VERSIONS/C-LANG/mt19937-64.c)
* [吉里吉里2](https://github.com/krkrz/krkr2)
* [吉里吉里Z](http://krkrz.github.io/)
* [Kirikiroid2](https://github.com/zeas2/Kirikiroid2)
* [mkrkr](https://github.com/zhangguof/mkrkr)
* [Kirikiri SDL2](https://github.com/uyjulian/krkrsdl2)

## ライセンス

このプロジェクトは、修正されたBSDライセンスの下でライセンスされています。 詳しくは `LICENSE`ファイルを読んでください。  
複数のサードパーティプロジェクトが使用されています。 詳しくは `LICENSE.3rdparty`ファイルをお読みください。  

# Si4735 ラジオモジュールのサンプルソール

ATMEL ATMEGA328P マイコンチップと Si4735 ラジオモジュールを I2C 接続し制御したサンプルプログラムです。

[Si4735 ラジオモジュールの Blog 記事一覧](http://mkusunoki.net/?page_id=2276)


* Si4735モジュールとの間は i2c 接続
* AM/FM の2バンド
* 電源切断時に ATMEGA328P の内蔵 EEPROM に直前の設定を書き込みスリープ
* 電源復旧時は内蔵 EEPROM より以前の状態を復元する。
* 開発環境は Atmel Studio 6.0 にて行なっております。


楠 昌浩(Masahiro Kusunoki)

http://mkusunoki.net

masahiro.kusunoki @ gmail.com
Twitter: ngc6589
Facebook: ngc6589

